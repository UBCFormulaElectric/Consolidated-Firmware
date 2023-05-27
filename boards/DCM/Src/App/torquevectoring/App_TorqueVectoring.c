#include "torquevectoring/App_TorqueVectoring.h"
#include "torquevectoring/App_TorqueVectoringConstants.h"
#include "torquevectoring/App_PowerLimiting.h"
#include "torquevectoring/App_ActiveDifferential.h"
#include "torquevectoring/App_TractionControl.h"
#include "torquevectoring/App_Regen.h"
#include "App_Timer.h"
#include "App_CanRx.h"
#include "App_CanTx.h"
#include "App_SharedMacros.h"

static TimerChannel pid_timeout;

static PowerLimiting_Inputs       power_limiting_inputs;
static ActiveDifferential_Inputs  active_differential_inputs;
static ActiveDifferential_Outputs active_differential_outputs;
static TractionControl_Inputs     traction_control_inputs;
// static TractionControl_Outputs    traction_control_outputs;
static Regen_Inputs  regen_inputs;
static Regen_Outputs regen_outputs;

// NOTE: Correction factor centered about 0.0f

/**
 * No PID for now.
 */

static PID pid_power_correction;
// static float pid_power_correction_factor = 0.0f;
static PID pid_traction_control;

static float accelerator_pedal_percent;
static float accelerator_pedal_percent;
static float wheel_speed_front_left_kph;
static float wheel_speed_front_right_kph;
static float motor_speed_left_rpm;
static float motor_speed_right_rpm;
static float battery_voltage;
static float current_consumption;
static float left_motor_temp_C;
static float right_motor_temp_C;
static float available_battery_power_kW;
static float steering_angle_deg;

/**
 * NEW: parameters for enabling/disabling power limiting and active differential
 */
static bool run_power_limiting      = true;
static bool run_active_differential = false;
static bool run_regen               = false;

void App_TorqueVectoring_Setup(void)
{
    App_PID_Init(&pid_power_correction, &PID_POWER_CORRECTION_CONFIG);
    App_PID_Init(&pid_traction_control, &PID_TRACTION_CONTROL_CONFIG);
    traction_control_inputs.pid = &pid_traction_control;

    App_Timer_InitTimer(&pid_timeout, PID_TIMEOUT_ms);
}

void App_TorqueVectoring_Run(void)
{
    // Read data from CAN
    // NOTE: Pedal percent CAN is in range 0.0-100.0%
    accelerator_pedal_percent   = App_CanRx_FSM_Apps_PappsMappedPedalPercentage_Get() * 0.01f;
    wheel_speed_front_left_kph  = App_CanRx_FSM_Wheels_LeftWheelSpeed_Get();
    wheel_speed_front_right_kph = App_CanRx_FSM_Wheels_RightWheelSpeed_Get();
    motor_speed_left_rpm        = (float)App_CanRx_INVL_MotorPositionInfo_MotorSpeed_Get();
    motor_speed_right_rpm       = (float)App_CanRx_INVR_MotorPositionInfo_MotorSpeed_Get();
    battery_voltage             = App_CanRx_BMS_TractiveSystem_TsVoltage_Get();
    current_consumption         = App_CanRx_BMS_TractiveSystem_TsCurrent_Get();
    left_motor_temp_C           = App_CanRx_INVL_Temperatures3_MotorTemperature_Get();
    right_motor_temp_C          = App_CanRx_INVR_Temperatures3_MotorTemperature_Get();
    // TODO (akoen): Available power will soon be replaced by current + voltage messages
    // available_battery_power_kW = App_CanRx_BMS_AvailablePower_AvailablePower_Get();
    available_battery_power_kW = 44.0f; // Tentative
    steering_angle_deg         = App_CanRx_FSM_Steering_SteeringAngle_Get();

    if (accelerator_pedal_percent > 0.0f)
    {
        App_TorqueVectoring_HandleAcceleration();
    }
    else
    {
        App_TorqueVectoring_HandleRegen();
    }
}

void App_TorqueVectoring_HandleAcceleration(void)
{
    // Reset control loops if timeout elapsed
    TimerState timeout = App_Timer_UpdateAndGetState(&pid_timeout);
    if (timeout == TIMER_STATE_EXPIRED)
    {
        App_PID_RequestReset(&pid_power_correction);
        App_PID_RequestReset(&pid_traction_control);
    }
    App_Timer_Restart(&pid_timeout);

    // Power Limiting
    power_limiting_inputs.left_motor_temp_C          = left_motor_temp_C;
    power_limiting_inputs.right_motor_temp_C         = right_motor_temp_C;
    power_limiting_inputs.available_battery_power_kW = available_battery_power_kW;
    power_limiting_inputs.accelerator_pedal_percent  = accelerator_pedal_percent;
    float estimated_power_limit;
    if (run_power_limiting)
    {
        estimated_power_limit = App_PowerLimiting_ComputeMaxPower(&power_limiting_inputs);
    }
    else
    {
        estimated_power_limit = available_battery_power_kW;
    }
    App_CanTx_DCM_DEBUG_Power_PowerMeasured_Set((float)estimated_power_limit);

    // Power limit correction

    /**
     * SKIP POWER LIMIT CORRECTION FOR NOW
     */
    // float power_limit = estimated_power_limit * (1.0f + pid_power_correction_factor);
    float power_limit = estimated_power_limit;

    // Active Differential
    /**
     * TORQUE REQUEST IN CASE OF NOT USING DIFFERENTIAL - OTHERWISE IS NOT SET AND NOT USED
     */
    float torque_request_no_differential = 0;

    if (run_active_differential)
    {
        active_differential_inputs.power_max_kW          = power_limit;
        active_differential_inputs.motor_speed_left_rpm  = motor_speed_left_rpm;
        active_differential_inputs.motor_speed_right_rpm = motor_speed_right_rpm;
        active_differential_inputs.wheel_angle_deg       = steering_angle_deg * APPROX_STEERING_TO_WHEEL_ANGLE;
        App_ActiveDifferential_ComputeTorque(&active_differential_inputs, &active_differential_outputs);
        App_CanTx_DCM_DEBUG_ActiveDiff_TorqueLeft_Set(active_differential_outputs.torque_left_Nm);
        App_CanTx_DCM_DEBUG_ActiveDiff_TorqueRight_Set(active_differential_outputs.torque_right_Nm);
    }
    else
    {
        torque_request_no_differential =
            MIN(accelerator_pedal_percent * available_battery_power_kW, MOTOR_TORQUE_LIMIT_Nm);
        App_CanTx_DCM_DEBUG_ActiveDiff_TorqueRight_Set(torque_request_no_differential);
        App_CanTx_DCM_DEBUG_ActiveDiff_TorqueLeft_Set(torque_request_no_differential);
    }

    /**
     * SKIP TRACTION CONTROL FOR NOW
     */

    // Traction Control
    // traction_control_inputs.motor_speed_left_rpm        = motor_speed_left_rpm;
    // traction_control_inputs.motor_speed_right_rpm       = motor_speed_right_rpm;
    // traction_control_inputs.torque_left_Nm              = active_differential_outputs.torque_left_Nm;
    // traction_control_inputs.torque_right_Nm             = active_differential_outputs.torque_right_Nm;
    // traction_control_inputs.wheel_speed_front_left_kph  = wheel_speed_front_left_kph;
    // traction_control_inputs.wheel_speed_front_right_kph = wheel_speed_front_right_kph;
    // App_TractionControl_ComputeTorque(&traction_control_inputs, &traction_control_outputs);

    // Inverter Torque Request
    /**
     * FEED ACTIVE_DIFFERENTIAL_INPUTS TO INVERTERS, BYPASS TRACTION CONTROL
     */

    float torque_left_final_Nm;
    float torque_right_final_Nm;
    if (run_active_differential)
    {
        torque_left_final_Nm  = active_differential_outputs.torque_left_Nm;
        torque_right_final_Nm = active_differential_outputs.torque_right_Nm;
    }
    else
    {
        torque_left_final_Nm  = torque_request_no_differential;
        torque_right_final_Nm = torque_request_no_differential;
    }
    // CLAMPS for safety only - should never exceed torque limit
    torque_left_final_Nm  = CLAMP(torque_left_final_Nm, 0, MOTOR_TORQUE_LIMIT_Nm);
    torque_right_final_Nm = CLAMP(torque_right_final_Nm, 0, MOTOR_TORQUE_LIMIT_Nm);
    App_CanTx_DCM_LeftInverterCommand_TorqueCommand_Set(torque_left_final_Nm);
    App_CanTx_DCM_RightInverterCommand_TorqueCommand_Set(torque_right_final_Nm);

    // Calculate power correction PID
    // float power_consumed_measured = battery_voltage * current_consumption;
    // float power_consumed_ideal    = (motor_speed_left_rpm * traction_control_outputs.torque_left_final_Nm +
    //                               motor_speed_right_rpm * traction_control_outputs.torque_right_final_Nm) /
    //                              POWER_TO_TORQUE_CONVERSION_FACTOR;
    // float power_consumed_estimate = power_consumed_ideal / (1.0f + pid_power_correction_factor);
    // pid_power_correction_factor -=
    //     App_PID_Compute(&pid_power_correction, power_consumed_measured, power_consumed_estimate);
    // pid_power_correction_factor = CLAMP(pid_power_correction_factor, PID_POWER_FACTOR_MIN, PID_POWER_FACTOR_MAX);
    // App_CanTx_DCM_DEBUG_Power_PowerMeasured_Set(power_consumed_measured);
    // App_CanTx_DCM_DEBUG_Power_PowerEstimate_Set(power_consumed_estimate);
    // App_CanTx_DCM_DEBUG_PIDPowerEstimate_Output_Set(pid_power_correction_factor);
    // App_CanTx_DCM_DEBUG_PIDPowerEstimate_Error_Set(pid_power_correction.error);
    // App_CanTx_DCM_DEBUG_PIDPowerEstimate_Derivative_Set(pid_power_correction.derivative);
    // App_CanTx_DCM_DEBUG_PIDPowerEstimate_Integral_Set(pid_power_correction.integral);
}

void App_TorqueVectoring_HandleRegen(void)
{
    if (run_regen)
    { // Run Regen
        regen_inputs.wheel_speed_front_left_kph  = wheel_speed_front_left_kph;
        regen_inputs.wheel_speed_front_right_kph = wheel_speed_front_right_kph;
        App_Regen_ComputeTorque(&regen_inputs, &regen_outputs);
        App_CanTx_DCM_LeftInverterCommand_TorqueCommand_Set(regen_outputs.regen_torque_left_Nm);
        App_CanTx_DCM_RightInverterCommand_TorqueCommand_Set(regen_outputs.regen_torque_right_Nm);
    }
    else
    { // No regen
        App_CanTx_DCM_LeftInverterCommand_TorqueCommand_Set(0);
        App_CanTx_DCM_RightInverterCommand_TorqueCommand_Set(0);
    }
}
