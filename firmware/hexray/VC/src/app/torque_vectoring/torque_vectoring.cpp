#include "torque_vectoring.hpp"

#include "torque_vectoring/estimation/vehicle_state_estimator.hpp"
#include "torque_vectoring/controllers/yaw_rate_control/controllers_dyrc.hpp"
#include "torque_vectoring/controllers/torque_allocator/torque_allocator.hpp"
#include "torque_vectoring/shared_datatypes/constants.hpp"
#include "torque_vectoring/shared_datatypes/datatypes.hpp"

namespace app::tv
{
using namespace shared_datatypes;
using namespace vd_constants;

//------------------------------------- TORQUE VECTORING UPDATE CYCLE -------------------------//
ControlOutput update(const estimation::Measurements &measurement)
{
    //------------------------------------- STATE ESTIMATION  --------------------------------//
    // Fuse the measured chassis state with the previous tire-force estimate to obtain the
    // filtered vehicle state used consistently by all downstream control blocks this tick.
    const VehicleState state = estimation::VehicleStateEstimator::estimate(measurement);

    //------------------------------------- HIGH LEVEL CONTROLLER ----------------------------//

    const float ax_mps2_setpoint = MAX_AX * measurement.apps;
    // Direct yaw rate control: corrective yaw moment
    const float omegadot_radps2_setpoint =
        controllers::dyrc::computeYawMoment(state.yaw_rate_radps, state.steer_ang_rad, state.v_x_mps);

    //------------------------------------- LOW LEVEL CONTROLLER -----------------------------//

    // Compute the low-speed blend once at the orchestration layer and pass it down explicitly.
    // This keeps the low-speed force-availability heuristic visible in one place instead of
    // recomputing it independently inside the optimizer.
    // const float vehicle_speed_mps = std::hypot(estimated_state.v_x_mps, estimated_state.v_y_mps);
    // const float low_speed_blend   = shared_datatypes::velocityBlend(vehicle_speed_mps);

    // ReSharper disable once CppUseStructuredBinding
    const wheel_set<float> kappa_opt =
        controllers::TorqueAllocator::optimize(state, ax_mps2_setpoint, omegadot_radps2_setpoint);

    //------------------------------------- POWER LIMITER -----------------------------------//
    // TODO: slip_ratio_opt -> slipRatioToWheelAngularVelocity() -> power limiter -> torque request

    return {
        .fl_kappa   = kappa_opt.fl,
        .fr_kappa   = kappa_opt.fr,
        .rl_kappa   = kappa_opt.rl,
        .rr_kappa   = kappa_opt.rr,
        .max_torque = 0,
        .min_torque = 0,
    };
}

ControlOutputAutonomous
    update_autonomous(const estimation::Measurements &measurement, const float ax, const float omega_dot)
{
    (void)measurement;
    (void)ax;
    (void)omega_dot;
    // TODO inshallah one day
    return {};
}
} // namespace app::tv
