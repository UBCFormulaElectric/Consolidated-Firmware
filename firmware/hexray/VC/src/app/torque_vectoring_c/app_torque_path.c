#include "app_controls.h"

#include "app_canTx.h"
#include "app_canRx.h"
#include "app_canAlerts.h"
#include "app_regen.h"
#include "app_activeDifferential.h"
#include "app_vehicleDynamicsConstants.h"
#include "app_vehicleDynamics.h"
#include "app_powerLimiting.h"
#include "app_torqueVectoring.h"
#include "app_sbgEllipse.h"
#include "io_log.h"

#include <math.h>

static SensorStatus check_sensors(void)
{
    SensorStatus sensor_status;
    sensor_status.gpsOk =
        !(app_canAlerts_VC_Info_SbgInitFailed_get() || app_sbgEllipse_getEkfSolutionMode() != POSITION);
    sensor_status.imuOk      = !app_canAlerts_VC_Info_ImuInitFailed_get();
    sensor_status.steeringOk = !(app_canRx_FSM_SteeringAngleOCSC_get() || app_canRx_FSM_SteeringAngleOutOfRange_get());
    sensor_status.useTV      = sensor_status.gpsOk && sensor_status.imuOk && sensor_status.steeringOk;

    if (!sensor_status.gpsOk)
        LOG_WARN("Sbg Ellipse not ok.");
    if (!sensor_status.imuOk)
        LOG_WARN("Imu not ok.");
    if (!sensor_status.steeringOk)
        LOG_WARN("Steering not ok");
    return sensor_status;
}

TorqueAllocationOutputs app_controls_calculateTorqueOutput(void)
{
    const bool regen_enabled = app_canTx_VC_RegenEnabled_get(); // IMPORTANT: this must be calculated in state machine

    double apps_pedal_decimal = (double)app_canRx_FSM_PappsMappedPedalPercentage_get() * 0.01;
    if (regen_enabled)
    {
        apps_pedal_decimal = app_regen_pedalRemapping(apps_pedal_decimal);
    }
    app_canTx_VC_RegenMappedPedalPercentage_set((float)apps_pedal_decimal * 100.0f);

    /* TODO: Vehicle dynamics people need to make sure to do a check if sensor init failed
        or not before using closed loop features */
    // Update Regen + TV LEDs

    if (apps_pedal_decimal < 0.0)
    {
        // TODO incorperate negative torques into the algorithm
        return app_regen_run(apps_pedal_decimal);
    }

    if (SWITCH_ON == app_canRx_CRIT_VanillaOverrideSwitch_get())
    {
        // TODO vanilla handling
    }

    // REAL CONTROLS ALGO START HERE
    const SensorStatus sensor_status = check_sensors();

    // motor speed rpms
    // const float motor_speed_fr_rpm = fabsf((float)app_canRx_INVFR_ActualVelocity_get()),
    //             motor_speed_fl_rpm = fabsf((float)app_canRx_INVFL_ActualVelocity_get()),
    //             motor_speed_rr_rpm = fabsf((float)app_canRx_INVRR_ActualVelocity_get()),
    //             motor_speed_rl_rpm = fabsf((float)app_canRx_INVRL_ActualVelocity_get());

    const double wheel_angle = (double)app_canRx_FSM_SteeringAngle_get() * APPROX_STEERING_TO_WHEEL_ANGLE;
    const double power_limit = app_powerLimiting_computeMaxPower(false);

    TorqueAllocationOutputs out;

    switch (app_canRx_CRIT_DriveMode_get())
    {
        case DRIVE_MODE_POWER:
        {
            app_canAlerts_VC_Info_DriveModeOverride_set(false);

            /************************************** following torque distribution on page 57
             * *********************************/

            // below the formulas are written with respect to left hand turns.... this is opposite of what active
            // differential previously did I believe this will work better with the imu as a left hand turn induces a
            // counter clockwise yaw moment

            // Total torque is to be our torque request from pedal * 4. We will assume the pedal percentage applies to
            // percentage of max torque of motor not of the car

            // potential division by 0 if load_transfer_const = -1, not sure what values are expected here but should
            // add safety

            static const double F = TRACK_WIDTH_m / (WHEEL_DIAMETER_IN / 2.0 * 2.4) * GEAR_RATIO;

            const double load_transfer_const  = 1.0,
                         total_torque_request = apps_pedal_decimal * MAX_TORQUE_REQUEST_NM * 4, front_yaw_moment = 0.0,
                         rear_yaw_moment = 0.0;

            // Commented out can messages, there solely for logging purposes
            app_canTx_VC_RearYawMoment_set((float)rear_yaw_moment);
            app_canTx_VC_FrontYawMoment_set((float)front_yaw_moment);

            out.front_left_torque = total_torque_request * load_transfer_const / (2 * (load_transfer_const + 1)) -
                                    front_yaw_moment / (2 * F);
            out.front_right_torque = out.front_left_torque + front_yaw_moment / F;
            out.rear_left_torque =
                total_torque_request / 2 - total_torque_request * load_transfer_const / (2 * (load_transfer_const + 1));
            out.rear_right_torque = out.rear_left_torque + rear_yaw_moment / F;

            const double total_power    = app_totalPower(&out);
            const double power_limit_kw = app_powerLimiting_computeMaxPower(false);
            app_canTx_VC_RequestedPower_set((float)total_power);
            app_canTx_VC_TotalAllocatedPower_set((float)total_power);

            PowerLimitingInputs powerLimitingInputs = { .total_requestedPower = (float)total_power,
                                                        .power_limit          = (float)power_limit_kw,
                                                        .torqueToMotors       = &out,
                                                        .is_regen_mode        = false,
                                                        .derating_value       = 1.0f };
            app_powerLimiting_torqueReduction(&powerLimitingInputs);

            app_canTx_VC_VcDriveMode_set(DRIVE_MODE_POWER);
            LOG_INFO("DriveHandling: PowerLimit Mode Active");
            break;
        }
        case DRIVE_MODE_POWER_AND_ACTIVE:
        {
            if (!sensor_status.steeringOk)
            {
                app_canAlerts_VC_Info_DriveModeOverride_set(true);
                break;
            }
            app_canAlerts_VC_Info_DriveModeOverride_set(false);
            out = app_activeDifferential_computeTorque(0.0, wheel_angle, 0.0);

            PowerLimitingInputs powerLimitingInputs = { .total_requestedPower = (float)app_totalPower(&out),
                                                        .power_limit          = (float)power_limit,
                                                        .torqueToMotors       = &out,
                                                        .is_regen_mode        = false,
                                                        .derating_value       = 1.0f };
            app_powerLimiting_torqueReduction(&powerLimitingInputs);
            /// dont use torque allocation here
            app_canTx_VC_VcDriveMode_set(DRIVE_MODE_POWER_AND_ACTIVE);
            LOG_INFO("DriveHandling: Active Diff Power Limit Mode Active");
            break;
        }
        case DRIVE_MODE_TV:
        {
            if (!sensor_status.useTV)
            {
                break;
            }
            app_canAlerts_VC_Info_DriveModeOverride_set(false);
            app_torqueVectoring_run(apps_pedal_decimal);
            app_canTx_VC_VcDriveMode_set(DRIVE_MODE_TV);
            LOG_INFO("DriveHandling: Torque Vectoring Mode Active");
            break;
        }
        default:
            break;
    }

    // TODO: implement app_powerLimiting_computeMaxTorque
    const double torque_request = apps_pedal_decimal * MAX_TORQUE_REQUEST_NM;
    return (TorqueAllocationOutputs){ torque_request, torque_request, torque_request, torque_request };
}