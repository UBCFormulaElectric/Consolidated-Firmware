#include "torque_vectoring.hpp"
#include "torque_vectoring_matlab.h" // this is just for matlab interface syncing

#include "shared_datatypes/vehicle_state_estimator.hpp"
#include "torque_vectoring/estimation/vehicle_state_estimator.hpp"
#include "torque_vectoring/controllers/controllers_dyrc.hpp"
#include "torque_vectoring/controllers/accel_request.hpp"
#include "torque_vectoring/controllers/torque_allocator.hpp"
#include "torque_vectoring/shared_datatypes/constants.hpp"

#include "torque_vectoring/estimation/steering_model.hpp"

using namespace app::tv::shared_datatypes;
using namespace app::tv::controllers;
using namespace vd_constants;

tv_debug tv_debug_data{};

template <Decimal T> ControlOutput<T> update(const VehicleState<T> &state)
{
    //------------------------------------- HIGH LEVEL CONTROLLER ----------------------------//

    // TODO: add regen enabled into state, as an argument into update, or in some other struct
    const accel_request::AccelRequest<T> ax_request =
        accel_request::compute(state.apps, state.brake, state.v_x_mps, true);

    // Direct yaw rate control: corrective yaw moment
    const T omegadot_radps2_setpoint =
        dyrc::computeYawMoment(state.yaw_rate_radps, (state.delta.fl + state.delta.fr) / 2, state.v_x_mps);

    tv_debug_data.veh_state.beta_rad = state.est_beta_rad();

    //------------------------------------- LOW LEVEL CONTROLLER -----------------------------//

    // Compute the low-speed blend once at the orchestration layer and pass it down explicitly.
    // This keeps the low-speed force-availability heuristic visible in one place instead of
    // recomputing it independently inside the optimizer.
    // const float vehicle_speed_mps = std::hypot(estimated_state.v_x_mps, estimated_state.v_y_mps);
    // const float low_speed_blend   = shared_datatypes::velocityBlend(vehicle_speed_mps);

    // ReSharper disable once CppUseStructuredBinding
    const wheel_set<T> kappa_opt =
        allocator::optimize(state, ax_request.percentage * MAX_AX_MPS2, omegadot_radps2_setpoint);

    //------------------------------------- POWER LIMITER -----------------------------------//
    // TODO: slip_ratio_opt -> slipRatioToWheelAngularVelocity() -> power limiter -> torque request

    T torque_lower_bound = -15 * ax_request.regen_derate_factor;
    T torque_upper_bound = 21 * ax_request.brake_derate_factor * ax_request.low_pedal_derate_factor;
    // T torque_upper_bound = 21;

    return { { kappa_opt.fl, kappa_opt.fr, kappa_opt.rl, kappa_opt.rr },
             { torque_upper_bound, torque_upper_bound, torque_upper_bound, torque_upper_bound },
             { torque_lower_bound, torque_lower_bound, torque_lower_bound, torque_lower_bound } };
}
template ControlOutput<tv_real> update(const VehicleState<tv_real> &state);

template <Decimal T> ControlOutputAutonomous<T> update_autonomous(const VehicleState<T> &state)
{
    (void)state;
    // TODO inshallah one day
    return {};
}
template ControlOutputAutonomous<tv_real> update_autonomous(const VehicleState<tv_real> &state);

// Simulink (MATLAB C Function block) interface only; never called on the car.
#ifndef TARGET_EMBEDDED
extern "C" void update_matlab(
    const double v_x,
    const double v_y,
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
    double       kappas[4],
    double       torque_max[4],
    double       torque_min[4],
    tv_debug    *debug)
{
    (void)v_x; // the estimator provides v_x / v_y now; the plant's true values are not used
    (void)v_y;

    // The Vehicle Dynamics Blockset plant reports body quantities in SAE J670 axes (x forward, y right, z down:
    // positive steer / yaw rate / lateral velocity / lateral accel = to the right). The estimator and controller use
    // ISO 8855 (x forward, y left, z up, positive steer / yaw rate = left), so every lateral quantity changes sign
    // here. Longitudinal quantities and the per-wheel arrays (fl, fr, rl, rr) are the same in both frames. Without this
    // the left/right is mirrored: the yaw-rate loop becomes positive feedback and torque vectoring acts backwards in
    // corners.
    const app::tv::estimation::Measurements meas = {
        .ax_mps2        = static_cast<tv_real>(a_x),
        .ay_mps2        = static_cast<tv_real>(-a_y),
        .yaw_rate_radps = static_cast<tv_real>(-yaw_rate),
        .omegas         = {
            .fl = static_cast<tv_real>(omegas[0]),
            .fr = static_cast<tv_real>(omegas[1]),
            .rl = static_cast<tv_real>(omegas[2]),
            .rr = static_cast<tv_real>(omegas[3]),
        },
        .delta      = static_cast<tv_real>(-steer),
        .apps = static_cast<tv_real>(apps),
        .brake = static_cast<tv_real>(brake),
        .gps_vx_mps = static_cast<tv_real>(gps_v_x),
        .gps_vy_mps = static_cast<tv_real>(-gps_v_y),
        .enable_gps = enable_gps,
    };
    VehicleState<tv_real> state = app::tv::estimation::VehicleStateEstimator::estimate(meas);
    // The estimator only fills in the vehicle state; the driver inputs come straight from Simulink.
    state.apps  = static_cast<tv_real>(apps);
    state.brake = static_cast<tv_real>(brake);

    // bring it in
    const auto [k_kappas, k_torque_max, k_torque_min] = update(state);
    // update
    kappas[0]     = static_cast<double>(k_kappas.fl);
    kappas[1]     = static_cast<double>(k_kappas.fr);
    kappas[2]     = static_cast<double>(k_kappas.rl);
    kappas[3]     = static_cast<double>(k_kappas.rr);
    torque_max[0] = static_cast<double>(k_torque_max.fl);
    torque_max[1] = static_cast<double>(k_torque_max.fr);
    torque_max[2] = static_cast<double>(k_torque_max.rl);
    torque_max[3] = static_cast<double>(k_torque_max.rr);
    torque_min[0] = static_cast<double>(k_torque_min.fl);
    torque_min[1] = static_cast<double>(k_torque_min.fr);
    torque_min[2] = static_cast<double>(k_torque_min.rl);
    torque_min[3] = static_cast<double>(k_torque_min.rr);
    *debug        = tv_debug_data;
}

void kappa_update_matlab(
    double       kappas[4],
    const double v_x,
    const double omegas[4],
    const double Fx[4],
    const double Fy[4],
    const double My[4],
    const double torque_max[4],
    const double torque_min[4],
    double       torque_request[4],
    double       speed_request[4])
{
    const auto to_wheel_set = [](const double w[4]) -> wheel_set<tv_real>
    {
        return { .fl = static_cast<tv_real>(w[0]),
                 .fr = static_cast<tv_real>(w[1]),
                 .rl = static_cast<tv_real>(w[2]),
                 .rr = static_cast<tv_real>(w[3]) };
    };

    // The plant gives wheel speeds, but kappa_update works in motor speeds (what the inverters report on the car).
    const wheel_set<tv_real> wheel_omegas = to_wheel_set(omegas);
    const auto               gear_ratio   = static_cast<tv_real>(GEAR_RATIO);
    const wheel_set<tv_real> motor_omegas = {
        .fl = wheel_omegas.fl * gear_ratio,
        .fr = wheel_omegas.fr * gear_ratio,
        .rl = wheel_omegas.rl * gear_ratio,
        .rr = wheel_omegas.rr * gear_ratio,
    };

    const auto [fl, fr, rl, rr] = kappa_update<tv_real>(
        to_wheel_set(kappas), static_cast<tv_real>(v_x), motor_omegas, to_wheel_set(Fx), to_wheel_set(Fy),
        to_wheel_set(My), to_wheel_set(torque_max), to_wheel_set(torque_min));
    torque_request[0] = static_cast<double>(fl);
    torque_request[1] = static_cast<double>(fr);
    torque_request[2] = static_cast<double>(rl);
    torque_request[3] = static_cast<double>(rr);

    // Simulink only: the power limiter needs the motor speed setpoints, which kappa_update keeps internal.
    const auto [req_fl, req_fr, req_rl, req_rr] =
        motor_speed_request<tv_real>(to_wheel_set(kappas), static_cast<tv_real>(v_x));
    speed_request[0] = static_cast<double>(req_fl);
    speed_request[1] = static_cast<double>(req_fr);
    speed_request[2] = static_cast<double>(req_rl);
    speed_request[3] = static_cast<double>(req_rr);
}

void estimate_matlab(
    double    yaw_rate,
    double    a_x,
    double    a_y,
    double    omegas[4],
    double    steer,
    double    gps_v_x,
    double    gps_v_y,
    bool      enable_gps,
    double   *est_v_x,
    double   *est_v_y,
    tv_debug *debug)
{
    // The Vehicle Dynamics Blockset plant reports body quantities in SAE J670 axes (x forward, y right, z down:
    // positive steer / yaw rate / lateral velocity / lateral accel = to the right). The estimator and controller use
    // ISO 8855 (x forward, y left, z up, positive steer / yaw rate = left), so every lateral quantity changes sign
    // here. Longitudinal quantities and the per-wheel arrays (fl, fr, rl, rr) are the same in both frames. Without this
    // the left/right is mirrored: the yaw-rate loop becomes positive feedback and torque vectoring acts backwards in
    // corners.
    const app::tv::estimation::Measurements meas = {
        .ax_mps2        = static_cast<tv_real>(a_x),
        .ay_mps2        = static_cast<tv_real>(-a_y),
        .yaw_rate_radps = static_cast<tv_real>(-yaw_rate),
        .omegas         = {
            .fl = static_cast<tv_real>(omegas[0]),
            .fr = static_cast<tv_real>(omegas[1]),
            .rl = static_cast<tv_real>(omegas[2]),
            .rr = static_cast<tv_real>(omegas[3]),
        },
        .delta      = static_cast<tv_real>(-steer),
        .gps_vx_mps = static_cast<tv_real>(gps_v_x),
        .gps_vy_mps = static_cast<tv_real>(-gps_v_y),
        .enable_gps = enable_gps,
    };
    const VehicleState<tv_real> state = app::tv::estimation::VehicleStateEstimator::estimate(meas);

    // Back to SAE so the outputs compare directly with the plant's BdyFrm.Cg.Vel signals.
    *est_v_x = static_cast<double>(state.v_x_mps);
    *est_v_y = -static_cast<double>(state.v_y_mps);
    *debug   = tv_debug_data;
}
#endif // TARGET_EMBEDDED
