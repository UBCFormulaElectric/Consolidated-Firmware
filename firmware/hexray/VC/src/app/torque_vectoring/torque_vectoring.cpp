#include "torque_vectoring.hpp"
#include "torque_vectoring_matlab.h" // this is just for matlab interface syncing

#include "shared_datatypes/vehicle_state_estimator.hpp"
#include "torque_vectoring/controllers/controllers_dyrc.hpp"
#include "torque_vectoring/controllers/torque_allocator.hpp"
#include "torque_vectoring/shared_datatypes/constants.hpp"

using namespace app::tv::shared_datatypes;
using namespace vd_constants;

template <Decimal T> ControlOutput<T> update(const VehicleState<T> &state)
{
    //------------------------------------- HIGH LEVEL CONTROLLER ----------------------------//
    const T ax_mps2_setpoint = MAX_AX_MPS2 * state.apps;
    // Direct yaw rate control: corrective yaw moment
    const T omegadot_radps2_setpoint = app::tv::controllers::dyrc::computeYawMoment(
        state.yaw_rate_radps, (state.delta.fl + state.delta.fr) / 2, state.v_x_mps);

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

    return { { kappa_opt.fl, kappa_opt.fr, kappa_opt.rl, kappa_opt.rr },
             { 5, 5, 5, 5 },
             { -5, -5, -5, -5 } };
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
    const VehicleState state = { .v_x_mps        = v_x,
                                 .v_y_mps        = v_y,
                                 .yaw_rate_radps = yaw_rate,
                                 .a_x_mps2       = a_x,
                                 .a_y_mps2       = a_y,
                                 .apps           = apps,
                                 .delta          = {
                                              .fl = delta_fl,
                                              .fr = delta_fr,
                                              .rl = 0.0f,
                                              .rr = 0.0f,
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

template <Decimal T> ControlOutputAutonomous<T> update_autonomous(const VehicleState<T> &state)
{
    (void)state;
    // TODO inshallah one day
    return {};
}
template ControlOutputAutonomous<float>  update_autonomous(const VehicleState<float> &state);
template ControlOutputAutonomous<double> update_autonomous(const VehicleState<double> &state);

void kappa_update_matlab(double kappas[4], const double v_x, double out[4])
{
    const auto [fl, fr, rl, rr] =
        kappa_update({ .fl = kappas[0], .fr = kappas[1], .rl = kappas[2], .rr = kappas[3] }, v_x);
    out[0] = fl;
    out[1] = fr;
    out[2] = rl;
    out[3] = rr;
}
