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
static PowerLimiting_Inputs      powerLimitingInputs;

void App_Run_Regen(float accelerator_pedal_percentage)
{
    activeDifferentialInputs.accelerator_pedal_percentage = accelerator_pedal_percentage;

    if (App_Regen_SafetyCheck(&regenAttributes))
    {
        activeDifferentialInputs.steering_angle_deg =
            App_CanRx_FSM_SteeringAngle_Get() * APPROX_STEERING_TO_WHEEL_ANGLE;

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

bool App_Regen_SafetyCheck(RegenBraking *regenAttr)
{
    bool battery_temp_in_range = App_CanRx_BMS_MaxCellTemperature_Get() < MAX_BATTERY_TEMP;
    return battery_temp_in_range && wheel_speed_in_range(regenAttr) && power_limit_check(regenAttr);
}

void App_Regen_Send_Torque_Request(float left, float right)
{
    App_CanTx_DCM_LeftInverterTorqueCommand_Set(left);
    App_CanTx_DCM_RightInverterTorqueCommand_Set(right);
}

void App_ActiveDifferential_ComputeNegativeTorque(ActiveDifferential_Inputs *inputs, RegenBraking *regenAttr)
{
    float Delta = App_ActiveDifferential_WheelAngleToSpeedDelta(inputs->steering_angle_deg);

    float cl = (1 + Delta);
    float cr = (1 - Delta);

    float torque_limit_Nm = App_ActiveDifferential_PowerToTorque(
        inputs->power_max_kW, regenAttr->motor_left_speed_rpm, regenAttr->motor_right_speed_rpm, cl, cr);

    float torque_left_Nm         = torque_limit_Nm * cl;
    float torque_right_Nm        = torque_limit_Nm * cr;
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

float App_ActiveDifferential_PowerToTorque(
    float power_kW,
    float left_motor_speed_rpm,
    float right_motor_speed_rpm,
    float cl,
    float cr)
{
    return (POWER_TO_TORQUE_CONVERSION_FACTOR * power_kW) /
           (left_motor_speed_rpm * cl + right_motor_speed_rpm * cr + SMALL_EPSILON);
}

/**
 * @note Computes the max allowable power draw from the battery based on motor temps,
 * the battery power limit (calculated by BMS), and accelerator pedal position.
 * @return A float for the maximum power allowed from the motor,
 */
float App_PowerLimiting_ComputeMaxPower(struct PowerLimiting_Inputs *inputs)
{
    float max_motor_temp = fmaxf(inputs->left_motor_temp_C, inputs->right_motor_temp_C);

    // ============== Calculate max powers =================
    // 1. Motor Temps
    float P_max_motor_temps = POWER_LIMIT_REGEN_kW;
    if ((float)max_motor_temp - MOTOR_TEMP_CUTOFF_c >= 30.0f)
    {
        P_max_motor_temps = 0.0;
    }
    else if (max_motor_temp > MOTOR_TEMP_CUTOFF_c)
    {
        P_max_motor_temps =
            POWER_LIMIT_REGEN_kW - (max_motor_temp - MOTOR_TEMP_CUTOFF_c) * MOTOR_TEMP_POWER_DECREMENTING_RATIO;
    }

    // 2. Battery state of charge
    float P_max_battery = inputs->available_battery_power_kW;

    // 3. Pedal percentage
    float P_max_accelerator = inputs->max_accelerator;

    // Calculate max power when fully throttled - for debugging purposes, to measure dips in available power
    float P_max_full_throttle = fminf(P_max_motor_temps, P_max_battery);
    // App_CanTx_DCM_PowerLimitValueAtFullThrottle_Set((float)P_max_full_throttle);

    float P_max = fminf(P_max_full_throttle, P_max_accelerator);

    // App_CanTx_DCM_PowerLimitValue_Set((float)P_max);
    return P_max;
}

static bool wheel_speed_in_range(RegenBraking *regenAttr)
{
    regenAttr->motor_right_speed_rpm = -(float)App_CanRx_INVR_MotorSpeed_Get();
    regenAttr->motor_left_speed_rpm  = (float)App_CanRx_INVL_MotorSpeed_Get();

    return MOTOR_RPM_TO_KMH(regenAttr->motor_right_speed_rpm) > SPEED_MIN_kph &&
           MOTOR_RPM_TO_KMH(regenAttr->motor_left_speed_rpm) > SPEED_MIN_kph;
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
    float min_motor_speed  = MOTOR_RPM_TO_KMH(MIN(regenAttr->motor_right_speed_rpm, regenAttr->motor_left_speed_rpm));

    powerInputs->left_motor_temp_C          = App_CanRx_INVL_MotorTemperature_Get();
    powerInputs->right_motor_temp_C         = App_CanRx_INVR_MotorTemperature_Get();
    powerInputs->available_battery_power_kW = POWER_LIMIT_REGEN_kW;

    if (regenAttr->current_battery_level > 3.9f)
    {
        pedal_percentage = pedal_percentage * 0.75f;
    }

    if (min_motor_speed <= 10.0f)
    {
        pedal_percentage = (min_motor_speed - SPEED_MIN_kph) / (SPEED_MIN_kph)*pedal_percentage;
    }

    powerInputs->max_accelerator = pedal_percentage * POWER_LIMIT_REGEN_kW;

    activeDiffInputs->power_max_kW = App_PowerLimiting_ComputeMaxPower(powerInputs);

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