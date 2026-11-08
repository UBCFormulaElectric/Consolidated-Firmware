#include "torque_vectoring.hpp"
#include "torque_vectoring_matlab.h" // this is just for matlab interface syncing

#include "shared_datatypes/vehicle_state_estimator.hpp"
#include "torque_vectoring/estimation/vehicle_state_estimator.hpp"
#include "torque_vectoring/controllers/controllers_dyrc.hpp"
#include "torque_vectoring/controllers/accel_request.hpp"
#include "torque_vectoring/controllers/torque_allocator.hpp"
#include "torque_vectoring/shared_datatypes/constants.hpp"
#include "torque_vectoring/shared_datatypes/torque_limits.hpp"

#include "torque_vectoring/estimation/steering_model.hpp"

using namespace app::tv::shared_datatypes::torque_limits;
using namespace app::tv::shared_datatypes;
using namespace app::tv::controllers;
using namespace vd_constants;

tv_debug tv_debug_data{};

template <Decimal T> ControlOutput<T> update(const VehicleState<T> &state, const ControlsConfig config)
{
    //------------------------------------- HIGH LEVEL CONTROLLER ----------------------------//

    // TODO: add regen enabled into state, as an argument into update, or in some other struct
    const accel_request::AccelRequest<T> ax_request =
        accel_request::compute(state.apps, state.brake, state.v_x_mps, config.regen_enabled);

    // Direct yaw rate control: corrective yaw moment
    const T mz_setpoint_Nm =
        dyrc::computeYawMoment(state.yaw_rate_radps, (state.delta.fl + state.delta.fr) / 2, state.v_x_mps);

    tv_debug_data.veh_state.beta_rad = state.est_beta_rad();

    //------------------------------------- LOW LEVEL CONTROLLER -----------------------------//

    // Compute the low-speed blend once at the orchestration layer and pass it down explicitly.
    // This keeps the low-speed force-availability heuristic visible in one place instead of
    // recomputing it independently inside the optimizer.
    // const float vehicle_speed_mps = std::hypot(estimated_state.v_x_mps, estimated_state.v_y_mps);
    // const float low_speed_blend   = shared_datatypes::velocityBlend(vehicle_speed_mps);

    // ReSharper disable once CppUseStructuredBinding
    const wheel_set<T> kappa_opt = allocator::optimize(state, ax_request.percentage * MAX_AX_MPS2, mz_setpoint_Nm);

    //------------------------------------- POWER LIMITER -----------------------------------//
    // TODO: slip_ratio_opt -> slipRatioToWheelAngularVelocity() -> power limiter -> torque request

    T torque_lower_bound = MAX_REGEN_TORQUE_Nm * ax_request.regen_derate_factor;
    // T torque_upper_bound = 21 * ax_request.brake_derate_factor * ax_request.low_pedal_derate_factor;
    T torque_upper_bound = MAX_TORQUE_REQUEST_Nm;

    const wheel_set<T> torque_max = { torque_upper_bound, torque_upper_bound, torque_upper_bound, torque_upper_bound };
    const wheel_set<T> torque_min = { torque_lower_bound, torque_lower_bound, torque_lower_bound, torque_lower_bound };
    torque_max.copyTo(tv_debug_data.accel_req.torque_max_Nm);
    torque_min.copyTo(tv_debug_data.accel_req.torque_min_Nm);

    return { kappa_opt, torque_max, torque_min };
}
template ControlOutput<tv_real> update(const VehicleState<tv_real> &state, const ControlsConfig config);

template <Decimal T>
ControlOutputAutonomous<T> update_autonomous(const VehicleState<T> &state, const ControlsConfig config)
{
    (void)state;
    // TODO inshallah one day
    return {};
}
template ControlOutputAutonomous<tv_real>
    update_autonomous(const VehicleState<tv_real> &state, const ControlsConfig config);

//------------------------------------- KAPPA UPDATE --------------------------------------//
namespace
{
/**
 * Given a slip ratio setpoint, gives motor angular velocity setpoints to achieve that slip ratio at each wheel's
 * current speed
 * @param kappas slip ratio setpoints for each wheel, where kappa = (wheel_speed - wheel_ground_speed) /
 * wheel_ground_speed and wheel_ground_speed is the wheel centre's speed along its rolling direction
 * @param v_wheel_mps per-wheel velocities in each tire's frame from VehicleState::v_in_tire_frame(), in m/s. In a
 * corner the outside wheels roll faster than the CG and the inside wheels slower (~7% each on an 8.5 m radius), so a
 * setpoint from the CG speed alone brakes the outside wheels and drives the inside ones.
 * @param fz_N per-wheel normal loads (static weight + aero + load transfer) from VehicleState::est_Fz_N()
 * @return motor speed setpoints in rad/s
 */
template <Decimal T>
wheel_set<T>
    motor_speed_request(const wheel_set<T> &kappas, const wheel_set<Pair<T>> &v_wheel_mps, const wheel_set<T> &fz_N)
{
    // Slip is defined against the effective rolling radius, not the unloaded radius. Re depends on load, and load
    // transfer moves it by ~0.4% at 1.5 g (~13% of a 0.03 slip target), so the loads include it.
    const auto wheel_speed_request = [](const T kappa, const T fz, const T v_x_mps)
    {
        const T v_regularized = std::max(std::abs(v_x_mps), static_cast<T>(SLIP_REGULARIZATION_SPEED_MPS));
        // Reverse is not a supported mode: never request backwards wheel rotation.
        const T wheel_surface_speed_request = std::max(v_x_mps + kappa * v_regularized, T(0));
        const T rolling_radius_m =
            app::tv::estimation::tire_model.effectiveRollingRadius_m(static_cast<float>(fz), std::abs(v_x_mps));
        return static_cast<T>(GEAR_RATIO) * wheel_surface_speed_request / rolling_radius_m;
    };

    return {
        .fl = wheel_speed_request(kappas.fl, fz_N.fl, v_wheel_mps.fl.x),
        .fr = wheel_speed_request(kappas.fr, fz_N.fr, v_wheel_mps.fr.x),
        .rl = wheel_speed_request(kappas.rl, fz_N.rl, v_wheel_mps.rl.x),
        .rr = wheel_speed_request(kappas.rr, fz_N.rr, v_wheel_mps.rr.x),
    };
}

/**
 * Lowers the torques so the sum on each axle is at most max_axle_Nm, so torque does not move between the axles (brake
 * balance). Both wheels on an axle drop by the same amount (keeps the yaw split); if one goes below its limit, the
 * other wheel takes the rest. Assumes max_axle_Nm >= the sum of the axle's two lower limits.
 * @param torque_Nm per-wheel torque requests in Nm
 * @param torque_min_Nm per-wheel lower torque limits in Nm
 * @param max_axle_Nm upper limit for the sum of the two torques on each axle in Nm
 * @return the lowered per-wheel torque requests in Nm
 */
template <Decimal T>
wheel_set<T> limitAxleTorque(wheel_set<T> torque_Nm, const wheel_set<T> &torque_min_Nm, const T max_axle_Nm)
{
    const auto limit_axle = [max_axle_Nm](T &left_Nm, T &right_Nm, const T min_left_Nm, const T min_right_Nm)
    {
        const T excess_Nm = left_Nm + right_Nm - max_axle_Nm;
        if (excess_Nm <= T(0))
        {
            return;
        }

        left_Nm -= excess_Nm / T(2.0);
        right_Nm -= excess_Nm / T(2.0);

        if (left_Nm < min_left_Nm)
        {
            right_Nm -= min_left_Nm - left_Nm;
            left_Nm = min_left_Nm;
        }
        else if (right_Nm < min_right_Nm)
        {
            left_Nm -= min_right_Nm - right_Nm;
            right_Nm = min_right_Nm;
        }
    };

    limit_axle(torque_Nm.fl, torque_Nm.fr, torque_min_Nm.fl, torque_min_Nm.fr);
    limit_axle(torque_Nm.rl, torque_Nm.rr, torque_min_Nm.rl, torque_min_Nm.rr);
    return torque_Nm;
}
} // namespace

template <Decimal T>
wheel_set<T> kappa_update(const ControlOutput<T> &control, const VehicleState<T> &state, const ControlsConfig config)
{
    // Stopped: nothing to track, so the integrators must not wind up (e.g. against the brakes at standstill)
    const auto stopped_speed_mps = static_cast<T>(speed_controller::STOPPED_SPEED_MPS);
    if (std::abs(state.v_x_mps) < stopped_speed_mps && std::abs(state.v_y_mps) < stopped_speed_mps)
    {
        speed_controller::reset();
    }

    // The mechanical brakes slow the wheels below their speed setpoints, so the speed controllers would push positive
    // torque against them, and their integrators would wind up and push again when the brake is released. While the
    // driver brakes, hold the integrators at zero and do not allow a net positive torque.
    const bool braking = state.brake > static_cast<T>(BRAKE_ACTIVE_THRESHOLD);
    if (braking && not config.regen_enabled)
    {
        speed_controller::reset();
    }

    // Wheel states. kappa_update works in motor speeds (what the inverters report on the car).
    const wheel_set<T>       omega_radps  = state.motor_omegas();
    const wheel_set<T>       alpha_rad    = state.alphas();
    const wheel_set<Pair<T>> v_wheel_mps  = state.v_in_tire_frame();
    const wheel_set<T>       vx_wheel_mps = { v_wheel_mps.fl.x, v_wheel_mps.fr.x, v_wheel_mps.rl.x, v_wheel_mps.rr.x };
    const wheel_set<T>       Fz_N         = state.est_Fz_N(); // shared by the speed setpoints and the tire model

    // Tire model at the slip ratio setpoints (same operating point as the allocator), for the tire feedforward
    const wheel_set<Pair<T>> tire_F_N = app::tv::estimation::tireForces_N(Fz_N, alpha_rad, control.kappas);
    const wheel_set<T>       Fx_N     = { tire_F_N.fl.x, tire_F_N.fr.x, tire_F_N.rl.x, tire_F_N.rr.x };
    const wheel_set<T>       My_Nm    = app::tv::estimation::tireMy_Nm(Fx_N, Fz_N, vx_wheel_mps);
    const wheel_set<T>       radial_deflection_m =
        app::tv::estimation::radial_deflection_estimate(Fz_N, omega_radps, tire_F_N);

    // Speed controller: a PID per wheel that tracks the motor speed setpoints, with the tire feedforward, limited to
    // the per-wheel torque limits.
    wheel_set<T> torque_Nm = speed_controller::computeTorque(
        motor_speed_request(control.kappas, v_wheel_mps, Fz_N), omega_radps, Fx_N, radial_deflection_m, My_Nm,
        control.torque_max, control.torque_min);

    if (braking)
    {
        torque_Nm = limitAxleTorque(torque_Nm, control.torque_min, T(0));
    }

    // The battery power limits come last: they are the hard limit from the BMS.
    return power_limiting::limitPower(
        omega_radps, torque_Nm, control.torque_max, control.torque_min, state.discharge_pl, state.charge_pl);
}

template wheel_set<tv_real> kappa_update(
    const ControlOutput<tv_real> &control,
    const VehicleState<tv_real>  &state,
    const ControlsConfig          config);

// Simulink (MATLAB C Function block) interface only; never called on the car.
#ifndef TARGET_EMBEDDED
namespace
{
// Simulink passes every per-wheel signal as a double[4] in fl, fr, rl, rr order
wheel_set<tv_real> toWheelSet(const double w[4])
{
    return { .fl = static_cast<tv_real>(w[0]),
             .fr = static_cast<tv_real>(w[1]),
             .rl = static_cast<tv_real>(w[2]),
             .rr = static_cast<tv_real>(w[3]) };
}

void toArray(const wheel_set<tv_real> &w, double out[4])
{
    out[0] = static_cast<double>(w.fl);
    out[1] = static_cast<double>(w.fr);
    out[2] = static_cast<double>(w.rl);
    out[3] = static_cast<double>(w.rr);
}

uint32_t               tick = 0;
ControlOutput<tv_real> control{};
} // namespace

extern "C" void update_matlab(
    const double yaw_rate,
    const double a_x,
    const double a_y,
    const double apps,
    const double brake,
    const double steer,
    const double gps_v_x,
    const double gps_v_y,
    const bool   enable_gps,
    const double omegas[4],
    const double discharge_pl,
    const double charge_pl,
    double       torque_request[4],
    tv_debug    *debug)
{
    // The Vehicle Dynamics Blockset plant reports body quantities in SAE J670 axes (x forward, y right, z down:
    // positive steer / yaw rate / lateral velocity / lateral accel = to the right). The estimator and controller use
    // ISO 8855 (x forward, y left, z up, positive steer / yaw rate = left), so every lateral quantity changes sign
    // here. Longitudinal quantities and the per-wheel arrays (fl, fr, rl, rr) are the same in both frames. Without this
    // the left/right is mirrored: the yaw-rate loop becomes positive feedback and torque vectoring acts backwards in
    // corners.
    const app::tv::estimation::Measurements meas = {
        .ax_mps2               = static_cast<tv_real>(a_x) * GRAVITY,
        .ay_mps2               = static_cast<tv_real>(-a_y) * GRAVITY,
        .yaw_rate_radps        = static_cast<tv_real>(-yaw_rate),
        .omegas                = toWheelSet(omegas),
        .delta                 = static_cast<tv_real>(-steer),
        .apps                  = static_cast<tv_real>(apps),
        .brake                 = static_cast<tv_real>(brake),
        .gps_vx_mps            = static_cast<tv_real>(gps_v_x),
        .gps_vy_mps            = static_cast<tv_real>(-gps_v_y),
        .enable_gps            = enable_gps,
        .discharge_power_limit = static_cast<tv_real>(discharge_pl),
        .charge_power_limit    = static_cast<tv_real>(charge_pl),
    };
    const VehicleState<tv_real> state = app::tv::estimation::VehicleStateEstimator::estimate(meas);

    const ControlsConfig config = { .regen_enabled = false };

    // High level controller and optimizer: slip ratio setpoints and torque limits
    if (tick++ % 5 == 0)
    {
        control = update(state, config);
    }

    // Speed controller, tire force feedforward and power limits. kappa_update works in motor speeds (what the
    // inverters report on the car).
    const wheel_set<tv_real> torque = kappa_update(control, state, config);

    toArray(torque, torque_request);
    *debug = tv_debug_data;
}
#endif // TARGET_EMBEDDED
