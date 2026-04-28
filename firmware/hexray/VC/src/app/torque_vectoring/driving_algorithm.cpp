#include "app_canTx.hpp"
#include "app_canRx.hpp"
#include "app_imu.hpp"
#include "app_sbgEllipse.hpp"
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
        if (inputs.launch_sw == SwitchState::SWITCH_OFF)
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

static SensorStatus sensor_checks()
{
    // TODO: In the steering out of range case, does it saturate or overflow or something else?
    return SensorStatus{ .imu_ok      = imus::getAllInitsFailed(),
                         .steering_ok = app::can_rx::FSM_Info_SteeringAngleOCSC_get(),
                         .gps_ok      = app::sbgEllipse::sbgInitOk() };
}

void run(float apps)
{
    // TODO: after CRIT Switches MR is merged change the switch getters
    SwitchState tv_sw     = static_cast<SwitchState>(app::can_rx::CRIT_TorqueVectoringSwitch_get());
    SwitchState regen_sw  = static_cast<SwitchState>(app::can_rx::CRIT_TorqueVectoringSwitch_get());
    SwitchState launch_sw = static_cast<SwitchState>(app::can_rx::CRIT_TorqueVectoringSwitch_get());

    if (regen_sw == SwitchState::SWITCH_ON)
        apps = app::tv::datatypes::torque_limits::REGEN_PEDAL_MAP(apps);

    // First check sensors, then grab inputs needed to run any algorithm
    SensorStatus sc = sensor_checks();

    const Inputs inputs = {
        .apps = apps, .steering = app::can_rx::FSM_SteeringAngle_get(), .regen_sw = regen_sw, .launch_sw = launch_sw
    };

    if (tv_sw == SwitchState::SWITCH_ON)
    {
        run_vanilla(inputs);
        return;
    }

    // TODO: adjust apps if regen enabled
    // We intentionally fallthrough as if other modes are added and if the sensor checks
    // for one mode fail, we can go to the next best mode. Currently this logic does
    // not really make sense because we dont have any fallback modes.
    switch (app::can_rx::CRIT_DriveMode_get())
    {
        case DriveMode::VANILLA:
            run_vanilla(inputs);
            break;
        case DriveMode::TV:
            if (sc.allSensorsOk())
            {
                run_optimal_tv(inputs);
                break;
            }
            [[fallthrough]];
        case DriveMode::LAUNCH:
            if (sc.allSensorsOk())
            {
                run_launch(inputs);
                break;
            }
            [[fallthrough]];
        case DriveMode::COUNT:
        default:
            run_vanilla(inputs);
            break;
    }
}
} // namespace app::tv::algo
