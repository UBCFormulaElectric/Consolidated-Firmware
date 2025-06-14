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
static TorqueAllocationInputs torqueToMotorsInputs;

void app_VanillaDrive_run(const float apps_pedal_percentage, TorqueAllocationOutputs *torqueOutputToMotors)
{
    // TODO: Use power limiting in regular drive
    // TODO: Implement active diff  in regular drive at min
    // TODO: Use sensor checks here to disable things accordingly (active diff, load trans)
    // TODO: set Load Transfer Const = 1 and set Desired Yaw Moment = 0

    // TODO: Implement active diff in regular drive at min
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

    const float pedal_based_torque = MIN((apps_pedal_percentage * MAX_TORQUE_REQUEST_NM), 1);

    // Calculate the actual torque request to transmit ---- VERY IMPORTANT NEED TO MAKE A TORQUE TRANSMISSION FUNCTION
    // data sheet says that the inverter expects a 16 bit signed int and that our sent request is scaled by 0.1
    const int16_t torque_request = PEDAL_REMAPPING(pedal_based_torque);

    // Transmit torque command to both inverters
    // TODO: USE TORQUE BROADCAST -- fix mapping
}

static void app_driveMode_driving(const float apps_pedal_percentage, TorqueAllocationOutputs *torqueOutputToMotors)
{
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
            torqueToMotorsInputs.front_yaw_moment    = 0.0f;
            torqueToMotorsInputs.rear_yaw_moment     = 0.0f;
            torqueToMotorsInputs.load_transfer_const = 0.0f;
            torqueToMotorsInputs.power_limit_kw =
                app_powerLimiting_computeMaxPower(regen_switch_is_on) app_torqueAllocation(&torqueToMotorsInputs);
            break;
        case DRIVE_MODE_POWER_AND_ACTIVE:
            const ActiveDifferential_Inputs ad_in = { .accelerator_pedal_percentage = apps_pedal_percentage,
                                                      .motor_speed_fl_rpm           = motor_speed_fl_rpm,
                                                      .motor_speed_fr_rpm           = motor_speed_fr_rpm,
                                                      .motor_speed_rl_rpm           = motor_speed_rl_rpm,
                                                      .motor_speed_rr_rpm           = motor_speed_rr_rpm,
                                                      .power_max_kW    = app_powerLimiting_computeMaxPower(false),
                                                      .wheel_angle_deg = wheel_angle };
            ActiveDifferential_Outputs      ad_out;

            app_activeDifferential_computeTorque(&ad_in, &ad_out);

            torqueOutputToMotors->front_left_torque  = ad_out.torque_fl_Nm;
            torqueOutputToMotors->front_right_torque = ad_out.torque_fr_Nm;
            torqueOutputToMotors->rear_left_torque   = ad_out.torque_rl_Nm;
            torqueOutputToMotors->rear_right_torque  = ad_out.torque_rr_Nm;

            const float requested_power = app_totalPower(torqueOutputToMotors);
            app_torqueReduction(requested_power, ad_in.power_max_kW, torqueOutputToMotors);
            /// dont use torque allocation here
            break;
        case DRIVE_MODE_TV:
            app_torqueVectoring_handleAcceleration(apps_pedal_percentage);

        default:
            break;
    }
}
