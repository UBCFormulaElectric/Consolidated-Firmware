#include "App_Regen.h"
#include <stdlib.h>
#include "math.h"

/**
 * Check if left or right wheel is greater than 5.0km/hr
 * @return true if wheel speed meets this condition, false
 * otherwise
 */
static bool wheel_speed_in_range();

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
static void compute_regen_torque_request(ActiveDifferential_Inputs *inputs, RegenBraking *regenAttr);

static RegenBraking              regenAttributes = { .enable_active_differential = true };
static ActiveDifferential_Inputs activeDifferentialInputs;

void App_Run_Regen(float *prev_torque_request, float accelerator_pedal_percentage)
{
    activeDifferentialInputs.accelerator_pedal_percentage = accelerator_pedal_percentage;

    if (App_Regen_SafetyCheck(&regenAttributes))
    {
        regenAttributes.prev_torque_request_Nm = *prev_torque_request;
        activeDifferentialInputs.steering_angle_deg =
            App_CanRx_FSM_SteeringAngle_Get() * APPROX_STEERING_TO_WHEEL_ANGLE;

        compute_regen_torque_request(&activeDifferentialInputs, &regenAttributes);
        App_CanTx_DCM_Warning_RegenNotAvailable_Set(false);
    }
    else
    {
        regenAttributes.left_inverter_torque_Nm  = 0.0;
        regenAttributes.right_inverter_torque_Nm = 0.0;
        regenAttributes.prev_torque_request_Nm   = 0.0;

        App_CanTx_DCM_Warning_RegenNotAvailable_Set(true);
    }

    *prev_torque_request = regenAttributes.prev_torque_request_Nm;

    App_Regen_Send_Torque_Request(regenAttributes.left_inverter_torque_Nm, regenAttributes.right_inverter_torque_Nm);
}

bool App_Regen_SafetyCheck(RegenBraking *regenAttr)
{
    bool battery_temp_in_range = App_CanRx_BMS_MaxCellTemperature_Get() < MAX_BATTERY_TEMP;
    return battery_temp_in_range && wheel_speed_in_range() && power_limit_check(regenAttr);
}

void App_Regen_Send_Torque_Request(float left, float right)
{
    App_CanTx_DCM_LeftInverterTorqueCommand_Set(left);
    App_CanTx_DCM_RightInverterTorqueCommand_Set(right);
}

void App_ActiveDifferential_ComputeNegativeTorque(
    ActiveDifferential_Inputs *inputs,
    RegenBraking *             regenAttr,
    float                      torqueRequest)
{
    float Delta = App_ActiveDifferential_WheelAngleToSpeedDelta(inputs->steering_angle_deg);

    float torque_left_Nm         = torqueRequest * (1 + Delta);
    float torque_right_Nm        = torqueRequest * (1 - Delta);
    float torque_negative_max_Nm = fminf(torque_left_Nm, torque_right_Nm);

    float scale = 1.0f;
    if (-torque_negative_max_Nm > -MAX_REGEN_nm)
    {
        scale = MAX_REGEN_nm / torque_negative_max_Nm;
    }

    regenAttr->left_inverter_torque_Nm  = torque_left_Nm * scale;
    regenAttr->right_inverter_torque_Nm = torque_right_Nm * scale;
}

float App_ActiveDifferential_WheelAngleToSpeedDelta(float wheel_angle_deg)
{
    // angle > 0 = right
    // angle < = left

    return TRACK_WIDTH_mm * tanf(DEG_TO_RAD(wheel_angle_deg)) / (2 * WHEELBASE_mm);
}

static bool wheel_speed_in_range()
{
    float motor_speed_right_kph = MOTOR_RPM_TO_KMH(-(float)App_CanRx_INVR_MotorSpeed_Get());
    float motor_speed_left_kph  = MOTOR_RPM_TO_KMH((float)App_CanRx_INVL_MotorSpeed_Get());

    return motor_speed_right_kph > SPEED_MIN_kph && motor_speed_left_kph > SPEED_MIN_kph;
}

static bool power_limit_check(RegenBraking *regenAttr)
{
    // TODO: Update check once power limiting is available
    regenAttr->current_battery_level = App_CanRx_BMS_MaxCellVoltage_Get();
    return regenAttr->current_battery_level < 4.1f;
}

static void compute_regen_torque_request(ActiveDifferential_Inputs *inputs, RegenBraking *regenAttr)
{
    float pedal_percentage = inputs->accelerator_pedal_percentage / MAX_PEDAL_POSITION;
    float torqueRequest    = MAX_REGEN_nm * pedal_percentage;
    float torqueChange;

    if (regenAttr->current_battery_level > 3.9f)
    {
        torqueRequest = torqueRequest * 0.75f;
    }

    torqueChange = torqueRequest - regenAttr->prev_torque_request_Nm;

    if ((float)fabs(torqueChange) > MAX_TORQUE_CHANGE)
    {
        torqueRequest =
            regenAttr->prev_torque_request_Nm + (torqueChange) / (float)(fabs(torqueChange)) * MAX_TORQUE_CHANGE;
    }

    regenAttr->prev_torque_request_Nm = torqueRequest;

    if (regenAttr->enable_active_differential)
    {
        App_ActiveDifferential_ComputeNegativeTorque(inputs, regenAttr, torqueRequest);
    }
    else
    {
        regenAttr->left_inverter_torque_Nm  = torqueRequest;
        regenAttr->right_inverter_torque_Nm = torqueRequest;
    }
}