#include "torquevectoring/App_TorqueVectoring.h"
#include "app_torqueVectoringConstants.h"
#include "torquevectoring/app_powerLimiting.h"
#include "torquevectoring/app_activeDifferential.h"
#include "torquevectoring/app_tractionControl.h"
#include "App_Timer.h"
#include "App_CanRx.h"
#include "App_CanTx.h"
#include "App_SharedMacros.h"
#include "App_SharedConstants.h"

#define MOTOR_NOT_SPINNING_SPEED_RPM 1000
static TimerChannel pid_timeout;

static PowerLimiting_Inputs       power_limiting_inputs;
static ActiveDifferential_Inputs  active_differential_inputs;
static ActiveDifferential_Outputs active_differential_outputs;
static TractionControl_Inputs     traction_control_inputs;
// static TractionControl_Outputs    traction_control_outputs;

// NOTE: Correction factor centered about 0.0f

/**
 * No PID for now.
 */

static PID   pid_power_correction;
static float pid_power_correction_factor = 0.0f;
static PID   pid_traction_control;

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
static float steering_angle_deg;

void app_torqueVectoring_setup(void)
{
    app_pid_init(&pid_power_correction, &PID_POWER_CORRECTION_CONFIG);
    app_pid_init(&pid_traction_control, &PID_TRACTION_CONTROL_CONFIG);
    traction_control_inputs.pid = &pid_traction_control;

    App_Timer_InitTimer(&pid_timeout, PID_TIMEOUT_ms);
}

void app_torqueVectoring_run(void)
{
    // Read data from CAN
    // NOTE: Pedal percent CAN is in range 0.0-100.0%
    accelerator_pedal_percent   = App_CanRx_FSM_PappsMappedPedalPercentage_Get() * 0.01f;
    wheel_speed_front_left_kph  = App_CanRx_FSM_LeftWheelSpeed_Get();
    wheel_speed_front_right_kph = App_CanRx_FSM_RightWheelSpeed_Get();
    motor_speed_left_rpm        = (float)App_CanRx_INVL_MotorSpeed_Get();
    motor_speed_right_rpm       = -1 * (float)App_CanRx_INVR_MotorSpeed_Get();
    battery_voltage             = App_CanRx_BMS_TractiveSystemVoltage_Get();
    current_consumption         = App_CanRx_BMS_TractiveSystemCurrent_Get();
    left_motor_temp_C           = App_CanRx_INVL_MotorTemperature_Get();
    right_motor_temp_C          = App_CanRx_INVR_MotorTemperature_Get();
    steering_angle_deg          = App_CanRx_FSM_SteeringAngle_Get();

    if (accelerator_pedal_percent > 0.0f)
    {
        app_torqueVectoring_handleAcceleration();
    }
}
// Read data from CAN
void app_torqueVectoring_handleAcceleration(void)
{
    // Reset control loops if timeout elapsed
    TimerState timeout = App_Timer_UpdateAndGetState(&pid_timeout);
    if (timeout == TIMER_STATE_EXPIRED)
    {
        app_pid_requestReset(&pid_power_correction);
        app_pid_requestReset(&pid_traction_control);
    }
    App_Timer_Restart(&pid_timeout);

    // Power Limiting
    power_limiting_inputs.left_motor_temp_C          = left_motor_temp_C;
    power_limiting_inputs.right_motor_temp_C         = right_motor_temp_C;
    power_limiting_inputs.available_battery_power_kW = POWER_LIMIT_CAR_kW;
    power_limiting_inputs.accelerator_pedal_percent  = accelerator_pedal_percent;
    float estimated_power_limit;
    estimated_power_limit = app_powerLimiting_computeMaxPower(&power_limiting_inputs);

    // Power limit correction
    float power_limit = estimated_power_limit * (1.0f + pid_power_correction_factor);

    // Active Differential
    active_differential_inputs.power_max_kW          = power_limit;
    active_differential_inputs.motor_speed_left_rpm  = motor_speed_left_rpm;
    active_differential_inputs.motor_speed_right_rpm = motor_speed_right_rpm;
    active_differential_inputs.wheel_angle_deg       = steering_angle_deg * APPROX_STEERING_TO_WHEEL_ANGLE;
    app_activeDifferential_ComputeTorque(&active_differential_inputs, &active_differential_outputs);
    App_CanTx_DCM_ActiveDiffTorqueLeft_Set(active_differential_outputs.torque_left_Nm);
    App_CanTx_DCM_ActiveDiffTorqueRight_Set(active_differential_outputs.torque_right_Nm);

    /**
     *  TRACTION CONTROL NOT TESTED ON CAR YET
     */
    // Traction Control
    // traction_control_inputs.motor_speed_left_rpm        = motor_speed_left_rpm;
    // traction_control_inputs.motor_speed_right_rpm       = motor_speed_right_rpm;
    // traction_control_inputs.torque_left_Nm              = active_differential_outputs.torque_left_Nm;
    // traction_control_inputs.torque_right_Nm             = active_differential_outputs.torque_right_Nm;
    // traction_control_inputs.wheel_speed_front_left_kph  = wheel_speed_front_left_kph;
    // traction_control_inputs.wheel_speed_front_right_kph = wheel_speed_front_right_kph;
    // app_tractionControl_computeTorque(&traction_control_inputs, &traction_control_outputs);

    // Inverter Torque Request
    float torque_left_final_Nm;
    float torque_right_final_Nm;
    torque_left_final_Nm  = active_differential_outputs.torque_left_Nm;
    torque_right_final_Nm = active_differential_outputs.torque_right_Nm;

    // Limit asymptotic torques at zero speed
    if (motor_speed_left_rpm < MOTOR_NOT_SPINNING_SPEED_RPM || motor_speed_right_rpm < MOTOR_NOT_SPINNING_SPEED_RPM)
    {
        torque_left_final_Nm  = accelerator_pedal_percent * MAX_TORQUE_REQUEST_NM;
        torque_right_final_Nm = accelerator_pedal_percent * MAX_TORQUE_REQUEST_NM;
    }

    // CLAMPS for safety only - should never exceed torque limit
    torque_left_final_Nm  = CLAMP(torque_left_final_Nm, 0, MAX_TORQUE_REQUEST_NM);
    torque_right_final_Nm = CLAMP(torque_right_final_Nm, 0, MAX_TORQUE_REQUEST_NM);
    App_CanTx_DCM_LeftInverterTorqueCommand_Set(torque_left_final_Nm);
    App_CanTx_DCM_RightInverterTorqueCommand_Set(torque_right_final_Nm);

    // Calculate power correction PID
    float power_consumed_measured = battery_voltage * current_consumption;

    // Commented out: Method for calculating power consumed from traction control outputs
    // float power_consumed_ideal    = (motor_speed_left_rpm * traction_control_outputs.torque_left_final_Nm +
    //                             motor_speed_right_rpm * traction_control_outputs.torque_right_final_Nm) /
    //                             POWER_TO_TORQUE_CONVERSION_FACTOR;

    float power_consumed_ideal =
        (motor_speed_left_rpm * torque_left_final_Nm + motor_speed_right_rpm * torque_right_final_Nm) /
        POWER_TO_TORQUE_CONVERSION_FACTOR;
    float power_consumed_estimate = power_consumed_ideal / (1.0f + pid_power_correction_factor);
    pid_power_correction_factor -=
        app_pid_compute(&pid_power_correction, power_consumed_measured, power_consumed_estimate);
    pid_power_correction_factor = CLAMP(pid_power_correction_factor, PID_POWER_FACTOR_MIN, PID_POWER_FACTOR_MAX);
    App_CanTx_DCM_PowerEstimateMeasuredValue_Set(power_consumed_measured);
    App_CanTx_DCM_PowerEstimateValue_Set(power_consumed_estimate);
    App_CanTx_DCM_PIDPowerEstimateOutput_Set(pid_power_correction_factor);
    App_CanTx_DCM_PIDPowerEstimateError_Set(pid_power_correction.error);
    App_CanTx_DCM_PIDPowerEstimateDerivative_Set(pid_power_correction.derivative);
    App_CanTx_DCM_PIDPowerEstimateIntegral_Set(pid_power_correction.integral);
}
