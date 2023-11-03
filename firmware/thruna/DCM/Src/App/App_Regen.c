#include "App_Regen.h"
#include <stdlib.h>
#include "math.h"

/**
 * Check if left or right wheel is greater than 5.0km/hr
 * @param RegenBraking struct to populate data
 * @return true if wheel speed meets this condition, false
 * otherwise
 */
static bool wheel_speed_in_range(RegenBraking *regenAttr);

/**
 * Check if battery cells are less than 4.0V
 * @param RegenBraking struct to populate data
 * @return true battery cells meet this condition,
 * false otherwise
 */
static bool power_limit_check(RegenBraking *regenAttr);

/**
 * Algorithm to send negative torque request dependent
 * on accelerator pedal percentage in range [-50,50]
 * and to do active differential or not
 * @param inputs are inputs for active differential
 * @param regenAttr struct provides for torque request
 */
static void compute_regen_torque_request(ActiveDifferential_Inputs *inputs, RegenBraking *regenAttr);

static RegenBraking              regenAttributes;
static ActiveDifferential_Inputs activeDifferentialInputs;

const float MAX_REGEN_Nm        = -50.0f; // TODO: find max regen torque value
const float wheelSpeedThreshold = 5.0f;
const float MAX_PEDAL_POS       = 50.0f;
const float MAX_CHANGE_TORQUE   = 10.0f;
const float min_scaling_speed   = 35.0f;

    void
    App_Run_Regen(bool *regen, float *prev_torque_request, float accelerator_pedal_percentage)
{
    activeDifferentialInputs.accelerator_pedal_percentage = accelerator_pedal_percentage;

    if (accelerator_pedal_percentage >= 0)
    {
        return;
    }
    else if (App_Regen_Safety(&regenAttributes))
    {
        activeDifferentialInputs.steering_angle_deg =
            App_CanRx_FSM_SteeringAngle_Get() * APPROX_STEERING_TO_WHEEL_ANGLE;
        regenAttributes.enable_active_differential = true;
        regenAttributes.prev_torque_request_Nm     = *prev_torque_request;

        compute_regen_torque_request(&activeDifferentialInputs, &regenAttributes);
        *regen = true;
    }
    else
    {
        regenAttributes.left_inverter_torque_Nm  = 0.0;
        regenAttributes.right_inverter_torque_Nm = 0.0;
        regenAttributes.prev_torque_request_Nm   = 0.0;
        App_CanTx_DCM_RegenNotAvailable_Set(true);
    }

    *prev_torque_request = regenAttributes.prev_torque_request_Nm;

    App_Regen_Activate(regenAttributes.left_inverter_torque_Nm, regenAttributes.right_inverter_torque_Nm);
}

bool App_Regen_Safety(RegenBraking *regenAttr)
{
    bool batteryTempInRange = App_CanRx_BMS_MaxCellTemperature_Get() < 45;
    return batteryTempInRange && wheel_speed_in_range(regenAttr) && power_limit_check(regenAttr);
}

void App_Regen_Activate(float left, float right)
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

    float torque_left_Nm  = torqueRequest * (1 + Delta);
    float torque_right_Nm = torqueRequest * (1 - Delta);
    float torque_max_Nm   = fmaxf(torque_left_Nm, torque_right_Nm);

    float scale = 1.0f;
    if (torque_max_Nm > -MAX_REGEN_Nm)
    {
        scale = MAX_REGEN_Nm / torque_max_Nm;
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

static bool wheel_speed_in_range(RegenBraking *regenAttr)
{
    regenAttr->wheel_speed_right_rpm = App_CanRx_FSM_RightWheelSpeed_Get();
    regenAttr->wheel_speed_left_rpm  = App_CanRx_FSM_LeftWheelSpeed_Get();

    return regenAttr->wheel_speed_right_rpm > wheelSpeedThreshold &&
           regenAttr->wheel_speed_left_rpm > wheelSpeedThreshold;
}

static bool power_limit_check(RegenBraking *regenAttr)
{
    // TODO: Update check once power limiting is available
    regenAttr->current_battery_level = App_CanRx_BMS_MaxCellVoltage_Get();
    return regenAttr->current_battery_level < 4.0f;
}

static void compute_regen_torque_request(ActiveDifferential_Inputs *inputs, RegenBraking *regenAttr)
{
    float pedal_percentage = inputs->accelerator_pedal_percentage;
    float max_wheel_speed  = MIN(regenAttr->wheel_speed_right_rpm, regenAttr->wheel_speed_left_rpm);
    float max_negative_torque_scale =
        (MAX_REGEN_Nm * CLAMP(max_wheel_speed, 0.0f, min_scaling_speed) / min_scaling_speed);
    float torqueRequest = (MAX_PEDAL_POS + pedal_percentage) * max_negative_torque_scale;
    float torqueChange;

    if (regenAttr->current_battery_level > 3.9f)
    {
        torqueRequest = torqueRequest * 0.75f;
    }

    torqueChange = torqueRequest - regenAttr->prev_torque_request_Nm;

    if ((float)fabs(torqueChange) > MAX_CHANGE_TORQUE)
    {
        torqueRequest = regenAttr->prev_torque_request_Nm + (torqueChange) / (float)(fabs(torqueChange)) * MAX_CHANGE_TORQUE;
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
