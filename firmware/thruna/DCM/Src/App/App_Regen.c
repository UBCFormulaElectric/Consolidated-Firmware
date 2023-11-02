#include "App_Regen.h"
#include <stdlib.h>
#include "math.h"

/**
 * Check if left or right wheel is greater than 5.0km/hr
 * @return true if wheel speed meets this condition, false
 * otherwise
 */
static bool wheel_speed_in_range(void);

/**
 * Check if battery cells are less than 4.0V
 * @return true battery cells meet this condition,
 * false otherwise
 */
static bool power_limit_check(RegenBraking *regenAttr);

/**
 * Algorithm to send negative torque request dependent
 * on accelerator pedal percentage in range [-50,50]
 */
static void compute_regen_torque_request(ActiveDifferential_Inputs *inputs, RegenBraking *regenAttr);
float App_ActiveDifferential_WheelAngleToSpeedDelta(float wheel_angle_deg);

#define DEG_TO_RAD(degrees) ((degrees) * (float)M_PI / 180.0f)
#define WHEELBASE_mm 1550
#define TRACK_WIDTH_mm 1100
#define APPROX_STEERING_TO_WHEEL_ANGLE 0.3f

static RegenBraking regenAttributes;
static ActiveDifferential_Inputs activeDifferentialInputs;

static float steering_angle_deg;

const float         MAX_REGEN_Nm            = -50.0f; // TODO: find max regen torque value
const float         wheelSpeedThreshold = 5.0f;

void App_Run_Regen(bool *regen, float accelerator_pedal_percentage)
{
    activeDifferentialInputs.accelerator_pedal_percentage = accelerator_pedal_percentage-50.0f;

    if (accelerator_pedal_percentage >= 0) {
        *regen = false;
        return;
    } 
    else if (App_Regen_Safety(&regenAttributes)) {
        steering_angle_deg         = App_CanRx_FSM_SteeringAngle_Get()*APPROX_STEERING_TO_WHEEL_ANGLE;
        activeDifferentialInputs.steering_angle_deg = steering_angle_deg;

        compute_regen_torque_request(&activeDifferentialInputs, &regenAttributes);
        *regen = true;
    } 
    else {
        regenAttributes.left_inverter_torque_Nm  = 0.0;
        regenAttributes.right_inverter_torque_Nm = 0.0;
        App_CanTx_DCM_RegenNotAvailable_Set(true);
    }

    App_Regen_Activate(regenAttributes.left_inverter_torque_Nm, regenAttributes.right_inverter_torque_Nm);
}

bool App_Regen_Safety(RegenBraking *regenAttr)
{
    const bool batteryTempInRange = App_CanRx_BMS_MaxCellTemperature_Get() < 45;
    return batteryTempInRange && wheel_speed_in_range() && power_limit_check(regenAttr);
}

void App_Regen_Activate(float left, float right)
{
    App_CanTx_DCM_LeftInverterTorqueCommand_Set(left);
    App_CanTx_DCM_RightInverterTorqueCommand_Set(right);
}

// TODO: linear from 90% 3.9 taper

void App_ActiveDifferential_ComputeNegativeTorque(ActiveDifferential_Inputs *inputs, RegenBraking *regenAttr)
{
    float Delta = App_ActiveDifferential_WheelAngleToSpeedDelta(inputs->steering_angle_deg);
    float pedal_percentage = inputs->accelerator_pedal_percentage;

    //more regen torque on the outer wheel

    float torque_left_Nm  = pedal_percentage * (1 + Delta);
    float torque_right_Nm = pedal_percentage * (1 - Delta);
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


static bool wheel_speed_in_range(void)
{
    float right = App_CanRx_FSM_RightWheelSpeed_Get();
    float left  = App_CanRx_FSM_LeftWheelSpeed_Get();

    return right > wheelSpeedThreshold && left > wheelSpeedThreshold;
}

static bool power_limit_check(RegenBraking *regenAttr)
{
    // TODO: Update check once power limiting is available @Will Chaba
    regenAttr->current_battery_level = App_CanRx_BMS_MaxCellVoltage_Get();
    return regenAttr->current_battery_level < 4.0f;
}

static void compute_regen_torque_request(ActiveDifferential_Inputs *inputs, RegenBraking *regenAttr)
{

    App_ActiveDifferential_ComputeNegativeTorque(inputs, regenAttr);

}
