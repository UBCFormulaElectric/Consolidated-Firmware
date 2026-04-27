#include "torque_vectoring.hpp"
#include "torque_vectoring_matlab.h" // this is just for matlab interface syncing

#include "shared_datatypes/vehicle_state_estimator.hpp"
#include "torque_vectoring/controllers/controllers_dyrc.hpp"
#include "torque_vectoring/controllers/torque_allocator.hpp"
#include "torque_vectoring/estimation/steering_model.hpp"
#include "torque_vectoring/shared_datatypes/constants.hpp"
#include "torque_vectoring/estimation/velocity_estimator.hpp"
// #include "torque_vectoring/estimation/vehicle_state_estimator.hpp"

using namespace app::tv::shared_datatypes;
using namespace app::tv::estimators;
using namespace vd_constants;

// TODO: should we input the actual sensor measurements or should we just grab them all in the function?
template <Decimal T>
VehicleState<T> estimate(
    const T            apps,
    const T            steer_ang_rad,
    const T            a_x_mps2,
    const T            a_y_mps2,
    const T            yaw_rate_rads,
    const wheel_set<T> rpm)
{
    VehicleState<T> state{};

    state.apps = apps;

    // TODO: replace these with actual sensor measurements
    state.a_body_mps2.x = a_x_mps2;
    state.a_body_mps2.y = a_y_mps2;
    state.yaw_rate_rads = yaw_rate_rads;

    // TODO: Replace with steering model once merged in
    state.delta = steering::wheel_steer_angles(steer_ang_rad);

    velocity_estimator::VelocityEstimatorInputs<T> inputs{};
    inputs.control.a_body_mps2 = state.a_body_mps2;
    inputs.control.r_rads      = state.yaw_rate_rads;
    inputs.rpm                 = rpm;
    inputs.steer_angles_rad    = state.delta;
    inputs.v_body_gps_mps      = Pair<T>{};
    state.v_body_mps           = velocity_estimator::estimate_body_velocity(inputs);

    return state;
}
template VehicleState<float> estimate(
    float            apps,
    float            steer_ang_rad,
    float            a_x_mps2,
    float            a_y_mps2,
    float            yaw_rate_rads,
    wheel_set<float> rpm);
template VehicleState<double> estimate(
    double            apps,
    double            steer_ang_rad,
    double            a_x_mps2,
    double            a_y_mps2,
    double            yaw_rate_rads,
    wheel_set<double> rpm);

template <Decimal T> ControlOutput<T> update(const VehicleState<T> &state)
{
    //------------------------------------- HIGH LEVEL CONTROLLER ----------------------------//
    const T ax_mps2_setpoint = MAX_AX_MPS2 * state.apps;
    // Direct yaw rate control: corrective yaw moment
    const T omegadot_radps2_setpoint = app::tv::controllers::dyrc::computeYawMoment(
        state.yaw_rate_rads, (state.delta.fl + state.delta.fr) / 2, state.v_body_mps.x);

    //------------------------------------- LOW LEVEL CONTROLLER -----------------------------//

    // Compute the low-speed blend once at the orchestration layer and pass it down explicitly.
    // This keeps the low-speed force-availability heuristic visible in one place instead of
    // recomputing it independently inside the optimizer.
    // const float vehicle_speed_mps = std::hypot(estimated_state.v_x_mps, estimated_state.v_y_mps);
    // const float low_speed_blend   = shared_datatypes::velocityBlend(vehicle_speed_mps);

    // ReSharper disable once CppUseStructuredBinding
    const wheel_set<T> kappa_opt =
        app::tv::controllers::allocator::optimize(state, ax_mps2_setpoint, omegadot_radps2_setpoint);

    //------------------------------------- POWER LIMITER -----------------------------------//
    // TODO: slip_ratio_opt -> slipRatioToWheelAngularVelocity() -> power limiter -> torque request

    return { { kappa_opt.fl, kappa_opt.fr, kappa_opt.rl, kappa_opt.rr }, { 21, 21, 21, 21 }, { -15, -15, -15, -15 } };
}
template ControlOutput<float>  update(const VehicleState<float> &state);
template ControlOutput<double> update(const VehicleState<double> &state);

extern "C" void update_matlab(
    const double v_x,
    const double v_y,
    const double yaw_rate,
    const double a_x,
    const double a_y,
    const double apps,
    const double delta_fl,
    const double delta_fr,
    double       kappas[4],
    double       torque_max[4],
    double       torque_min[4])
{
    const VehicleState<double> state = { .v_body_mps    = { .x = v_x, .y = v_y },
                                         .yaw_rate_rads = yaw_rate,
                                         .a_body_mps2   = { .x = a_x, .y = a_y },
                                         .apps          = apps,
                                         .delta         = {
                                                     .fl = delta_fl,
                                                     .fr = delta_fr,
                                                     .rl = 0.0,
                                                     .rr = 0.0,
                                         } };
    // bring it in
    const auto [k_kappas, k_torque_max, k_torque_min] = update(state);
    // std::cout << "DIH" << std::endl;
    // update
    kappas[0]     = k_kappas.fl;
    kappas[1]     = k_kappas.fr;
    kappas[2]     = k_kappas.rl;
    kappas[3]     = k_kappas.rr;
    torque_max[0] = k_torque_max.fl;
    torque_max[1] = k_torque_max.fr;
    torque_max[2] = k_torque_max.rl;
    torque_max[3] = k_torque_max.rr;
    torque_min[0] = k_torque_min.fl;
    torque_min[1] = k_torque_min.fr;
    torque_min[2] = k_torque_min.rl;
    torque_min[3] = k_torque_min.rr;
}

extern "C" void estimate_matlab(
    const double apps,
    const double steering_angle,
    const double a_x,
    const double a_y,
    const double yaw_rate,
    double       rpm[4],
    double       v_body_mps[2],
    double       delta[4])
{
    const VehicleState<double> state =
        estimate(apps, steering_angle, a_x, a_y, yaw_rate, wheel_set<double>{ rpm[0], rpm[1], rpm[2], rpm[3] });

    v_body_mps[0] = state.v_body_mps.x;
    v_body_mps[1] = state.v_body_mps.y;
    delta[0]      = state.delta.fl;
    delta[1]      = state.delta.fr;
    delta[2]      = state.delta.rl;
    delta[3]      = state.delta.rr;
}

template <Decimal T> ControlOutputAutonomous<T> update_autonomous(const VehicleState<T> &state)
{
    (void)state;
    // TODO inshallah one day
    return {};
}
template ControlOutputAutonomous<float>  update_autonomous(const VehicleState<float> &state);
template ControlOutputAutonomous<double> update_autonomous(const VehicleState<double> &state);

void kappa_update_matlab(double kappas[4], const double v_x, double oemgas[4])
{
    const auto [fl, fr, rl, rr] =
        kappa_update({ .fl = kappas[0], .fr = kappas[1], .rl = kappas[2], .rr = kappas[3] }, v_x);
    oemgas[0] = fl;
    oemgas[1] = fr;
    oemgas[2] = rl;
    oemgas[3] = rr;
}
