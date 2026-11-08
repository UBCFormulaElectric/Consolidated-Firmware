#include "controls/controls.hpp"
#include "controls/controls_matlab.h" // this is just for matlab interface syncing

#include "controls/datatypes/vehicle_state.hpp"
#include "controls/estimation/state_estimator.hpp"
#include "controls/reference/yaw_rate_controller.hpp"
#include "controls/reference/accel_request.hpp"
#include "controls/allocation/optimizer.hpp"
#include "controls/actuation/axle_limit.hpp"
#include "controls/params.hpp"
#include "controls/torque_limits.hpp"

#include "controls/estimation/steering_model.hpp"

tv_debug tv_debug_data{};

namespace app::controls
{
using namespace params;
using namespace torque_limits;

ControlOutput update(const VehicleState &state, const ControlsConfig config)
{
    //------------------------------------- REFERENCE ----------------------------------------//

    const reference::accel_request::AccelRequest ax_request =
        reference::accel_request::compute(state.apps, state.brake, state.v_x_mps, config.regen_enabled);

    // Direct yaw rate control: corrective yaw moment
    const float mz_setpoint_Nm = reference::yaw_rate::computeYawMoment(
        state.yaw_rate_radps, (state.wheel_angle_rad.fl + state.wheel_angle_rad.fr) / 2, state.v_x_mps);

    tv_debug_data.veh_state.beta_rad = state.est_beta_rad();

    //------------------------------------- ALLOCATION ---------------------------------------//

    // Compute the low-speed blend once at the orchestration layer and pass it down explicitly.
    // This keeps the low-speed force-availability heuristic visible in one place instead of
    // recomputing it independently inside the optimizer.
    // const float vehicle_speed_mps = std::hypot(estimated_state.v_x_mps, estimated_state.v_y_mps);
    // const float low_speed_blend   = velocityBlend(vehicle_speed_mps);

    // ReSharper disable once CppUseStructuredBinding
    const wheel_set<float> kappa_opt = allocation::optimize(state, ax_request.normalized * MAX_AX_MPS2, mz_setpoint_Nm);

    //------------------------------------- POWER LIMITER -----------------------------------//
    // TODO: slip_ratio_opt -> slipRatioToWheelAngularVelocity() -> power limiter -> torque request

    const float motor_torque_min_Nm = MAX_REGEN_TORQUE_Nm * ax_request.regen_derate_factor;
    // const float motor_torque_max_Nm = 21 * ax_request.brake_derate_factor * ax_request.low_pedal_derate_factor;
    const float motor_torque_max_Nm = MAX_TORQUE_REQUEST_Nm;

    const wheel_set<float> torque_max_Nm = { motor_torque_max_Nm, motor_torque_max_Nm, motor_torque_max_Nm,
                                             motor_torque_max_Nm };
    const wheel_set<float> torque_min_Nm = { motor_torque_min_Nm, motor_torque_min_Nm, motor_torque_min_Nm,
                                             motor_torque_min_Nm };
    torque_max_Nm.copyTo(tv_debug_data.accel_req.torque_max_Nm);
    torque_min_Nm.copyTo(tv_debug_data.accel_req.torque_min_Nm);

    return { kappa_opt, torque_max_Nm, torque_min_Nm };
}

ControlOutputAutonomous update_autonomous(const VehicleState &state, const ControlsConfig config)
{
    (void)state;
    (void)config;
    // TODO inshallah one day
    return {};
}

//------------------------------------- KAPPA UPDATE --------------------------------------//
namespace
{
    /**
     * Given a slip ratio setpoint, gives motor angular velocity setpoints to achieve that slip ratio at each wheel's
     * current speed
     * @param kappas slip ratio setpoints for each wheel, where kappa = (wheel_speed - wheel_ground_speed) /
     * wheel_ground_speed and wheel_ground_speed is the wheel centre's speed along its rolling direction
     * @param v_wheel_mps per-wheel velocities in each tire's frame from VehicleState::v_in_tire_frame(), in m/s. In a
     * corner the outside wheels roll faster than the CG and the inside wheels slower (~7% each on an 8.5 m radius), so
     * a setpoint from the CG speed alone brakes the outside wheels and drives the inside ones.
     * @param fz_N per-wheel normal loads (static weight + aero + load transfer) from VehicleState::est_Fz_N()
     * @return motor speed setpoints in rad/s
     */
    wheel_set<float> motor_speed_request(
        const wheel_set<float>       &kappas,
        const wheel_set<Pair<float>> &v_wheel_mps,
        const wheel_set<float>       &fz_N)
    {
        // Slip is defined against the effective rolling radius, not the unloaded radius. Re depends on load, and load
        // transfer moves it by ~0.4% at 1.5 g (~13% of a 0.03 slip target), so the loads include it.
        const auto motor_speed_radps = [](const float kappa, const float fz, const float v_x_mps)
        {
            const float v_regularized_mps = std::max(std::abs(v_x_mps), SLIP_REGULARIZATION_SPEED_MPS);
            // Reverse is not comp legal but highkey would be useful for testing so we dont have to push the car
            // could be integrated in a different manner though, not through the algorithm.
            const float wheel_surface_speed_mps = std::max(v_x_mps + kappa * v_regularized_mps, 0.0f);
            const float rolling_radius_m = estimation::tire_model.effectiveRollingRadius_m(fz, std::abs(v_x_mps));
            return GEAR_RATIO * wheel_surface_speed_mps / rolling_radius_m;
        };

        return {
            .fl = motor_speed_radps(kappas.fl, fz_N.fl, v_wheel_mps.fl.x),
            .fr = motor_speed_radps(kappas.fr, fz_N.fr, v_wheel_mps.fr.x),
            .rl = motor_speed_radps(kappas.rl, fz_N.rl, v_wheel_mps.rl.x),
            .rr = motor_speed_radps(kappas.rr, fz_N.rr, v_wheel_mps.rr.x),
        };
    }
} // namespace

wheel_set<float> kappa_update(const ControlOutput &control, const VehicleState &state, const ControlsConfig config)
{
    // Stopped: nothing to track, so the integrators must not wind up (e.g. against the brakes at standstill)
    const auto stopped_speed_mps = actuation::speed_controller::STOPPED_SPEED_MPS;
    if (std::abs(state.v_x_mps) < stopped_speed_mps && std::abs(state.v_y_mps) < stopped_speed_mps)
    {
        actuation::speed_controller::reset();
    }

    // The mechanical brakes slow the wheels below their speed setpoints, so the speed controllers would push positive
    // torque against them, and their integrators would wind up and push again when the brake is released. While the
    // driver brakes, hold the integrators at zero and do not allow a net positive torque.
    const bool braking = state.brake > BRAKE_ACTIVE_THRESHOLD;
    if (braking && not config.regen_enabled)
    {
        actuation::speed_controller::reset();
    }

    // Wheel states. kappa_update works in motor speeds (what the inverters report on the car).
    const wheel_set<float>       motor_omega_radps = state.motor_omegas();
    const wheel_set<float>       alpha_rad         = state.alphas();
    const wheel_set<Pair<float>> v_wheel_mps       = state.v_in_tire_frame();
    const wheel_set<float> v_x_wheel_mps = { v_wheel_mps.fl.x, v_wheel_mps.fr.x, v_wheel_mps.rl.x, v_wheel_mps.rr.x };
    const wheel_set<float> fz_N          = state.est_Fz_N(); // shared by the speed setpoints and the tire model

    // Tire model at the slip ratio setpoints (same operating point as the allocator), for the tire feedforward
    const wheel_set<Pair<float>> tire_forces_N = estimation::tireForces_N(fz_N, alpha_rad, control.kappas);
    const wheel_set<float> fx_N  = { tire_forces_N.fl.x, tire_forces_N.fr.x, tire_forces_N.rl.x, tire_forces_N.rr.x };
    const wheel_set<float> my_Nm = estimation::tireMy_Nm(fx_N, fz_N, v_x_wheel_mps);
    const wheel_set<float> radial_deflection_m =
        estimation::radial_deflection_estimate(fz_N, motor_omega_radps, tire_forces_N);

    // Speed controller: a PID per wheel that tracks the motor speed setpoints, with the tire feedforward, limited to
    // the per-wheel torque limits.
    wheel_set<float> torque_Nm = actuation::speed_controller::computeTorque(
        motor_speed_request(control.kappas, v_wheel_mps, fz_N), motor_omega_radps, fx_N, radial_deflection_m, my_Nm,
        control.torque_max_Nm, control.torque_min_Nm);

    if (braking)
    {
        torque_Nm = actuation::limitAxleTorque(torque_Nm, control.torque_min_Nm, 0.0f);
    }

    // The battery power limits come last: they are the hard limit from the BMS.
    return actuation::power_limit::limitPower(
        motor_omega_radps, torque_Nm, control.torque_max_Nm, control.torque_min_Nm, state.discharge_limit_W,
        state.charge_limit_W);
}

} // namespace app::controls

// Simulink (MATLAB C Function block) interface only; never called on the car.
#ifndef TARGET_EMBEDDED
namespace
{
// Simulink passes every per-wheel signal as a double[4] in fl, fr, rl, rr order
app::controls::wheel_set<float> toWheelSet(const double w[4])
{
    return { .fl = static_cast<float>(w[0]),
             .fr = static_cast<float>(w[1]),
             .rl = static_cast<float>(w[2]),
             .rr = static_cast<float>(w[3]) };
}

void toArray(const app::controls::wheel_set<float> &w, double out[4])
{
    out[0] = static_cast<double>(w.fl);
    out[1] = static_cast<double>(w.fr);
    out[2] = static_cast<double>(w.rl);
    out[3] = static_cast<double>(w.rr);
}

uint32_t                     tick = 0;
app::controls::ControlOutput control{};
} // namespace

extern "C" void update_matlab(
    const double yaw_rate_radps,
    const double a_x_g,
    const double a_y_g,
    const double apps,
    const double brake,
    const double steering_wheel_angle_rad,
    const double gps_v_x_mps,
    const double gps_v_y_mps,
    const bool   enable_gps,
    const double wheel_omega_radps[4],
    const double discharge_limit_W,
    const double charge_limit_W,
    double       torque_request_Nm[4],
    tv_debug    *debug)
{
    // The Vehicle Dynamics Blockset plant reports body quantities in SAE J670 axes (x forward, y right, z down:
    // positive steer / yaw rate / lateral velocity / lateral accel = to the right). The estimator and controller use
    // ISO 8855 (x forward, y left, z up, positive steer / yaw rate = left), so every lateral quantity changes sign
    // here. Longitudinal quantities and the per-wheel arrays (fl, fr, rl, rr) are the same in both frames. Without this
    // the left/right is mirrored: the yaw-rate loop becomes positive feedback and torque vectoring acts backwards in
    // corners.
    using namespace app::controls;
    using params::GRAVITY;

    const estimation::Measurements meas = {
        .ax_mps2                  = static_cast<float>(a_x_g) * GRAVITY,
        .ay_mps2                  = static_cast<float>(-a_y_g) * GRAVITY,
        .yaw_rate_radps           = static_cast<float>(-yaw_rate_radps),
        .wheel_omega_radps        = toWheelSet(wheel_omega_radps),
        .steering_wheel_angle_rad = static_cast<float>(-steering_wheel_angle_rad),
        .apps                     = static_cast<float>(apps),
        .brake                    = static_cast<float>(brake),
        .gps_vx_mps               = static_cast<float>(gps_v_x_mps),
        .gps_vy_mps               = static_cast<float>(-gps_v_y_mps),
        .enable_gps               = enable_gps,
        .discharge_limit_W        = static_cast<float>(discharge_limit_W),
        .charge_limit_W           = static_cast<float>(charge_limit_W),
    };
    const VehicleState state = estimation::state_estimator::estimate(meas);

    const ControlsConfig config = { .regen_enabled = false };

    // High level controller and optimizer: slip ratio setpoints and torque limits
    if (tick++ % 5 == 0)
    {
        control = update(state, config);
    }

    // Speed controller, tire force feedforward and power limits. kappa_update works in motor speeds (what the
    // inverters report on the car).
    const wheel_set<float> torque_Nm = kappa_update(control, state, config);

    toArray(torque_Nm, torque_request_Nm);
    *debug = tv_debug_data;
}
#endif // TARGET_EMBEDDED
