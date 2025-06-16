#include "app_vehicleDynamicsConstants.h"
#include "app_torqueVectoring.h"
#include "app_regen.h"
#include "app_warningHanding.h"
#include "app_canAlerts.h"
#include "app_vehicleDynamics.h"
#include "app_powerLimiting.h"
#include "app_torqueDistribution.h"
#include "app_driveHandling.h"
#include "app_activeDifferential.h"
#include "app_canRx.h"
#include "app_sbgEllipse.h"

#define EFFICIENCY_ESTIMATE (0.80f)

static TorqueAllocationInputs torqueToMotorsInputs;
static SensorStatus            sensor_status;

static void app_performSensorChecks(void);

void app_vanillaDrive_run(const float apps_pedal_percentage, TorqueAllocationOutputs *torqueOutputToMotors)
{
    const float bms_available_power = (float)app_canRx_BMS_AvailablePower_get();
    const float motor_speed_fr_rpm  = (float)app_canRx_INVFR_ActualVelocity_get();
    const float motor_speed_fl_rpm  = (float)app_canRx_INVFL_ActualVelocity_get();
    const float motor_speed_rr_rpm  = (float)app_canRx_INVRR_ActualVelocity_get();
    const float motor_speed_rl_rpm  = (float)app_canRx_INVRL_ActualVelocity_get();
    float       bms_torque_limit    = MAX_TORQUE_REQUEST_NM;

    if ((motor_speed_fr_rpm + motor_speed_fl_rpm + motor_speed_rr_rpm + motor_speed_rl_rpm) > 0.0f)
    {
        // Estimate the maximum torque request to draw the maximum power available from the BMS
        const float available_output_power_w  = bms_available_power * EFFICIENCY_ESTIMATE;
        const float combined_motor_speed_rads = RPM_TO_RADS(motor_speed_fr_rpm) + RPM_TO_RADS(motor_speed_fl_rpm) +
                                                RPM_TO_RADS(motor_speed_rr_rpm) + RPM_TO_RADS(motor_speed_rl_rpm);
        bms_torque_limit = MIN(available_output_power_w / combined_motor_speed_rads, MAX_TORQUE_REQUEST_NM);
    }

    // Calculate the maximum torque request, according to the BMS available power
    const float max_bms_torque_request = apps_pedal_percentage * bms_torque_limit;

    const float pedal_based_torque = apps_pedal_percentage * MAX_TORQUE_REQUEST_NM;

    // data sheet says that the inverter expects a 16 bit signed int and that our sent request is scaled by 0.1
    const float torque_request = fminf(pedal_based_torque, max_bms_torque_request);

    torqueOutputToMotors->front_left_torque = torque_request;
    torqueOutputToMotors->front_right_torque = torque_request;
    torqueOutputToMotors->rear_left_torque = torque_request;
    torqueOutputToMotors->rear_right_torque = torque_request;
    
    app_canAlerts_VC_Info_DriveModeOverride_set(false);
}

void app_driveMode_run(const float apps_pedal_percentage, TorqueAllocationOutputs *torqueOutputToMotors)
{
    app_performSensorChecks();

    const DriveMode driveMode          = app_canRx_CRIT_DriveMode_get();
    const float     motor_speed_fr_rpm = (float)app_canRx_INVFR_ActualVelocity_get();
    const float     motor_speed_fl_rpm = (float)app_canRx_INVFL_ActualVelocity_get();
    const float     motor_speed_rr_rpm = (float)app_canRx_INVRR_ActualVelocity_get();
    const float     motor_speed_rl_rpm = (float)app_canRx_INVRL_ActualVelocity_get();
    const float     wheel_angle        = app_canRx_FSM_SteeringAngle_get() * APPROX_STEERING_TO_WHEEL_ANGLE;
    const float     max_power_limit    = app_powerLimiting_computeMaxPower(false);

    switch (driveMode)
    {
        case DRIVE_MODE_POWER:
        {    
            app_canAlerts_VC_Info_DriveModeOverride_set(false);
            
            torqueToMotorsInputs.front_yaw_moment    = 0.0f;
            torqueToMotorsInputs.rear_yaw_moment     = 0.0f;
            torqueToMotorsInputs.load_transfer_const = 0.0f;
            torqueToMotorsInputs.power_limit_kw      = app_powerLimiting_computeMaxPower(false);
            app_torqueAllocation(&torqueToMotorsInputs, torqueOutputToMotors);
            break;
        }
        case DRIVE_MODE_POWER_AND_ACTIVE:
        {
            if (sensor_status.steeringOk)
            {
                app_canAlerts_VC_Info_DriveModeOverride_set(false);
                
                ActiveDifferential_Inputs ad_in = { .accelerator_pedal_percentage = apps_pedal_percentage,
                    .motor_speed_fl_rpm           = motor_speed_fl_rpm,
                    .motor_speed_fr_rpm           = motor_speed_fr_rpm,
                    .motor_speed_rl_rpm           = motor_speed_rl_rpm,
                    .motor_speed_rr_rpm           = motor_speed_rr_rpm,
                    .power_max_kW    = app_powerLimiting_computeMaxPower(false),
                    .wheel_angle_deg = wheel_angle };

                ActiveDifferential_Outputs ad_out;

                app_activeDifferential_computeTorque(&ad_in, &ad_out);

                torqueOutputToMotors->front_left_torque  = ad_out.torque_fl_Nm;
                torqueOutputToMotors->front_right_torque = ad_out.torque_fr_Nm;
                torqueOutputToMotors->rear_left_torque   = ad_out.torque_rl_Nm;
                torqueOutputToMotors->rear_right_torque  = ad_out.torque_rr_Nm;

                const float requested_power = app_totalPower(torqueOutputToMotors);
                app_torqueReduction(requested_power, ad_in.power_max_kW, torqueOutputToMotors);
                /// dont use torque allocation here
            } 
            else 
            {
                app_canAlerts_VC_Info_DriveModeOverride_set(true);
                app_vanillaDrive_run(apps_pedal_percentage, torqueOutputToMotors);
            }
            break;
        }
        case DRIVE_MODE_TV: 
        {
            if (sensor_status.useTV) 
            {
                app_canAlerts_VC_Info_DriveModeOverride_set(false);
                app_torqueVectoring_run(apps_pedal_percentage);
            }
            else {
                app_canAlerts_VC_Info_DriveModeOverride_set(true);
                app_vanillaDrive_run(apps_pedal_percentage, torqueOutputToMotors);
            }
            break;
        }
        default:
            break;
    }
}

static void app_performSensorChecks(void)
{
    sensor_status.gpsOk = !app_canTx_VC_Info_SbgInitFailed_get() || !(app_sbgEllipse_getEkfSolutionMode() == POSITION);
    sensor_status.imuOk = !app_canTx_VC_Info_ImuInitFailed_get();
    sensor_status.steeringOk =
        !(app_canRx_FSM_Info_SteeringAngleOCSC_get() || app_canRx_FSM_Info_SteeringAngleOutOfRange_get());
    sensor_status.useTV = sensor_status.gpsOk && sensor_status.imuOk && sensor_status.steeringOk;

    if (!sensor_status.gpsOk)
        LOG_WARN("Sbg Ellipse not ok.");
    if (!sensor_status.imuOk)
        LOG_WARN("Imu not ok.");
    if (!sensor_status.steeringOk)
        LOG_WARN("Steering not ok");
}