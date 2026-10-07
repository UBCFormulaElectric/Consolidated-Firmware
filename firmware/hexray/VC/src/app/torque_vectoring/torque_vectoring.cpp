#include "torque_vectoring.hpp"
#include "torque_vectoring_matlab.h" // this is just for matlab interface syncing

#include "shared_datatypes/vehicle_state_estimator.hpp"
#include "torque_vectoring/controllers/controllers_dyrc.hpp"
#include "torque_vectoring/controllers/accel_request.hpp"
#include "torque_vectoring/controllers/torque_allocator.hpp"
#include "torque_vectoring/shared_datatypes/constants.hpp"

#include "torque_vectoring/estimation/steering_model.hpp"

using namespace app::tv::shared_datatypes;
using namespace vd_constants;

tv_debug tv_debug_data{};

template <Decimal T> ControlOutput<T> update(const VehicleState<T> &state)
{
    //------------------------------------- HIGH LEVEL CONTROLLER ----------------------------//

    // TODO: add regen enabled into state, as an argument into update, or in some other struct
    const T ax_mps2_setpoint =
        app::tv::controllers::accel_request::compute_accel_request(state.apps, state.brake, state.v_x_mps, true);

    // Direct yaw rate control: corrective yaw moment
    const T omegadot_radps2_setpoint = app::tv::controllers::dyrc::computeYawMoment(
        state.yaw_rate_radps, (state.delta.fl + state.delta.fr) / 2, state.v_x_mps);

    tv_debug_data.veh_state.beta_rad = state.est_beta_rad();

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
    double       kappas[4],
    double       torque_max[4],
    double       torque_min[4],
    tv_debug    *debug)
{
    // The Vehicle Dynamics Blockset plant reports body quantities in SAE J670 axes (x forward, y right, z down:
    // positive steer / yaw rate / lateral velocity / lateral accel = to the right). The controller uses ISO 8855
    // (y left, z up, left wheels at +y), so every lateral quantity changes sign at this boundary. Without this the
    // controller's left/right is mirrored relative to the real wheels: the yaw-rate loop becomes positive feedback
    // and torque vectoring acts backwards in corners.
    // Simulink works in double; the controller runs in tv_real (float unless TV_DOUBLE_PRECISION is defined).
    const VehicleState<tv_real> state = {
        .v_x_mps        = static_cast<tv_real>(v_x),
        .v_y_mps        = static_cast<tv_real>(-v_y),
        .yaw_rate_radps = static_cast<tv_real>(-yaw_rate),
        .a_x_mps2       = static_cast<tv_real>(a_x),
        .a_y_mps2       = static_cast<tv_real>(-a_y),
        .apps           = static_cast<tv_real>(apps),
        .brake          = static_cast<tv_real>(brake),
        .delta          = app::tv::estimators::steering::wheel_steer_angles(static_cast<tv_real>(steer)),
    };
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

void kappa_update_matlab(double kappas[4], const double v_x, double oemgas[4])
{
    const auto [fl, fr, rl, rr] = kappa_update<tv_real>(
        { .fl = static_cast<tv_real>(kappas[0]),
          .fr = static_cast<tv_real>(kappas[1]),
          .rl = static_cast<tv_real>(kappas[2]),
          .rr = static_cast<tv_real>(kappas[3]) },
        static_cast<tv_real>(v_x));
    oemgas[0] = static_cast<double>(fl);
    oemgas[1] = static_cast<double>(fr);
    oemgas[2] = static_cast<double>(rl);
    oemgas[3] = static_cast<double>(rr);
}
#endif // TARGET_EMBEDDED
