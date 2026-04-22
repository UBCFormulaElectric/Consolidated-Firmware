#include "app_canTx.hpp"
#include "driving_algorithm.hpp"
#include "torque_vectoring/datatypes/torque_limits.hpp"
#include "util_utils.hpp"

namespace app::tv::algo
{
namespace
{
void run_vanilla(const Inputs &inputs)
{
    (void)inputs;

    // Equal torques.
    // TODO: implement
}

void run_optimal_tv(const Inputs &inputs)
{
    (void)inputs;

    // Optimizer-based algorithm.
    // TODO: implement
}

// Send torque through optimal algo once launch switch is high.
void run_launch(const Inputs &inputs)
{
    if (inputs.launch_switch == SwitchState::SWITCH_OFF)
    {
        send_speed(0.0f, 0.0f, 0.0f, 0.0f);
        send_torque(0.0f, 0.0f, 0.0f, 0.0f);
    }

    run_optimal_tv(inputs);
}

} // namespace

// TODO: replace both with wheel set later
void send_torque(float fl, float fr, float rl, float rr)
{
    app::can_tx::VC_INVFLTorqueSetpoint_set(app::tv::datatypes::torque_limits::TORQUE_REQUEST(fl));
    app::can_tx::VC_INVFRTorqueSetpoint_set(app::tv::datatypes::torque_limits::TORQUE_REQUEST(fr));
    app::can_tx::VC_INVRLTorqueSetpoint_set(app::tv::datatypes::torque_limits::TORQUE_REQUEST(rl));
    app::can_tx::VC_INVRRTorqueSetpoint_set(app::tv::datatypes::torque_limits::TORQUE_REQUEST(rr));
}

void send_speed(float fl, float fr, float rl, float rr)
{
    // TODO: implement
    UNUSED(fl);
    UNUSED(fr);
    UNUSED(rl);
    UNUSED(rr);
    return;
}

void run(const DriveMode mode, const Inputs &inputs)
{
    if (inputs.tv_switch == SwitchState::SWITCH_ON)
    {
        run_vanilla(inputs);
        return;
    }

    // TODO: adjust apps if regen enabled

    switch (mode)
    {
        case DriveMode::VANILLA:
            run_vanilla(inputs);
            break;
        case DriveMode::TV:
            run_optimal_tv(inputs);
            break;
        case DriveMode::LAUNCH:
            run_launch(inputs);
            break;
        case DriveMode::COUNT:
        default:
            run_vanilla(inputs);
            break;
    }
}
} // namespace app::tv::algo
