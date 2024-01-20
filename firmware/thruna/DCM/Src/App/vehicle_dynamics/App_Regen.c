#include "App_Regen.h"
#include "App_SharedDcmConstants.h"
#include "App_PowerLimiting.h"
#include "App_ActiveDifferential.h"
#include "App_CanRx.h"
#include "App_CanTx.h"
#include "App_SharedMacros.h"
#include <stdlib.h>
#include "math.h"

/**
 * Check if left or right wheel is greater than 5.0km/hr
 * @param ActiveDifferential_Inputs struct to populate data
 * @return true if wheel speed meets this condition, false
 * otherwise
 */
static bool wheel_speed_in_range(ActiveDifferential_Inputs *inputs);

/**
 * Check if battery cells are less than 4.0V
 * @param RegenBraking struct to populate data
 * @return true battery cells meet this condition,
 * false otherwise
 */
static bool power_limit_check(RegenBraking *regenAttr);

/**
 * Algorithm to send negative torque request dependent
 * on accelerator pedal percentage in range [-100, 0]
 * and to do active differential or not
 * @param inputs are inputs for active differential
 * @param regenAttr struct provides for torque request
 */
static void compute_regen_torque_request(
    ActiveDifferential_Inputs *inputs,
    RegenBraking *             regenAttr,
    PowerLimiting_Inputs *     powerInputs);

static RegenBraking              regenAttributes = { .enable_active_differential = true };
static ActiveDifferential_Inputs activeDifferentialInputs;
static PowerLimiting_Inputs      powerLimitingInputs = { .power_limit_kW = POWER_LIMIT_REGEN_kW };

void App_Run_Regen(float accelerator_pedal_percentage)
{
    activeDifferentialInputs.accelerator_pedal_percentage = accelerator_pedal_percentage;

    if (App_Regen_SafetyCheck(&regenAttributes, &activeDifferentialInputs))
    {
        activeDifferentialInputs.wheel_angle_deg = App_CanRx_FSM_SteeringAngle_Get() * APPROX_STEERING_TO_WHEEL_ANGLE;

        compute_regen_torque_request(&activeDifferentialInputs, &regenAttributes, &powerLimitingInputs);
        App_CanTx_DCM_Warning_RegenNotAvailable_Set(false);
    }
    else
    {
        regenAttributes.left_inverter_torque_Nm  = 0.0;
        regenAttributes.right_inverter_torque_Nm = 0.0;

        App_CanTx_DCM_Warning_RegenNotAvailable_Set(true);
    }

    App_Regen_Send_Torque_Request(regenAttributes.left_inverter_torque_Nm, regenAttributes.right_inverter_torque_Nm);
}

bool App_Regen_SafetyCheck(RegenBraking *regenAttr, ActiveDifferential_Inputs *inputs)
{
    bool battery_temp_in_range = App_CanRx_BMS_MaxCellTemperature_Get() < MAX_BATTERY_TEMP;
    return battery_temp_in_range && wheel_speed_in_range(inputs) && power_limit_check(regenAttr);
}

void App_Regen_Send_Torque_Request(float left, float right)
{
    App_CanTx_DCM_LeftInverterTorqueCommand_Set(left);
    App_CanTx_DCM_RightInverterTorqueCommand_Set(right);
}

void App_ActiveDifferential_ComputeNegativeTorque(ActiveDifferential_Inputs *inputs, RegenBraking *regenAttr)
{
    float Delta = App_ActiveDifferential_WheelAngleToSpeedDelta(inputs->wheel_angle_deg);

    float cl = (1 + Delta);
    float cr = (1 - Delta);

    float torque_limit_Nm = -App_ActiveDifferential_PowerToTorque(
        inputs->power_max_kW, inputs->motor_speed_left_rpm, inputs->motor_speed_right_rpm, cl, cr);

    float torque_left_Nm         = torque_limit_Nm * cl;
    float torque_right_Nm        = torque_limit_Nm * cr;
    float torque_negative_max_Nm = fminf(torque_left_Nm, torque_right_Nm);

    float scale = 1.0f;
    if (torque_negative_max_Nm < MAX_REGEN_nm)
    {
        scale = MAX_REGEN_nm / torque_negative_max_Nm;
    }

    regenAttr->left_inverter_torque_Nm  = torque_left_Nm * scale;
    regenAttr->right_inverter_torque_Nm = torque_right_Nm * scale;
}

static bool wheel_speed_in_range(ActiveDifferential_Inputs *inputs)
{
    inputs->motor_speed_right_rpm = -(float)App_CanRx_INVR_MotorSpeed_Get();
    inputs->motor_speed_left_rpm  = (float)App_CanRx_INVL_MotorSpeed_Get();

    return MOTOR_RPM_TO_KMH(inputs->motor_speed_right_rpm) > SPEED_MIN_kph &&
           MOTOR_RPM_TO_KMH(inputs->motor_speed_left_rpm) > SPEED_MIN_kph;
}

static bool power_limit_check(RegenBraking *regenAttr)
{
    regenAttr->current_battery_level = App_CanRx_BMS_MaxCellVoltage_Get();
    return regenAttr->current_battery_level < 4.1f;
}

static void compute_regen_torque_request(
    ActiveDifferential_Inputs *activeDiffInputs,
    RegenBraking *             regenAttr,
    PowerLimiting_Inputs *     powerInputs)
{
    float pedal_percentage = activeDiffInputs->accelerator_pedal_percentage / MAX_PEDAL_POSITION;
    float min_motor_speed =
        MOTOR_RPM_TO_KMH(MIN(activeDiffInputs->motor_speed_right_rpm, activeDiffInputs->motor_speed_left_rpm));

    powerInputs->left_motor_temp_C  = App_CanRx_INVL_MotorTemperature_Get();
    powerInputs->right_motor_temp_C = App_CanRx_INVR_MotorTemperature_Get();

    if (regenAttr->current_battery_level > 3.9f)
    {
        pedal_percentage = pedal_percentage * 0.75f;
    }

    if (min_motor_speed <= 10.0f)
    {
        pedal_percentage = (min_motor_speed - SPEED_MIN_kph) / (SPEED_MIN_kph)*pedal_percentage;
    }

    powerInputs->accelerator_pedal_percent = pedal_percentage;
    activeDiffInputs->power_max_kW         = App_PowerLimiting_ComputeMaxPower(powerInputs);

    if (regenAttr->enable_active_differential)
    {
        App_ActiveDifferential_ComputeNegativeTorque(activeDiffInputs, regenAttr);
    }
    else
    {
        // no power limit, no active differential
        regenAttr->left_inverter_torque_Nm  = MAX_REGEN_nm * pedal_percentage;
        regenAttr->right_inverter_torque_Nm = MAX_REGEN_nm * pedal_percentage;
    }
}