#include "torquevectoring/App_TorqueVectoring.h"
#include "torquevectoring/App_TorqueVectoringConstants.h"
#include "torquevectoring/App_PowerLimiting.h"
#include "torquevectoring/App_ActiveDifferential.h"
#include "torquevectoring/App_TractionControl.h"
#include "torquevectoring/App_Regen.h"
#include "App_Timer.h"
#include "App_CanRx.h"
#include "App_CanTx.h"

TimerChannel pid_timeout;

PowerLimiting_Inputs       power_limiting_inputs;
ActiveDifferential_Inputs  active_differential_inputs;
ActiveDifferential_Outputs active_differential_outputs;
TractionControl_Inputs     traction_control_inputs;
TractionControl_Outputs    traction_control_outputs;
Regen_Inputs               regen_inputs;
Regen_Outputs              regen_outputs;

PID   pid_power_correction;
float pid_power_correction_factor = 1.0f;

void App_TorqueVectoring_Setup(void)
{
    App_PID_Init(&pid_power_correction, &PID_POWER_CORRECTION_CONFIG);

    PID pid_traction_control;
    App_PID_Init(&pid_traction_control, &PID_TRACTION_CONTROL_CONFIG);
    traction_control_inputs.pid = &pid_traction_control;

    App_Timer_InitTimer(&pid_timeout, PID_TIMEOUT_ms);
}

void App_TorqueVectoring_Run(void)
{
    // Shared CAN inputs
    // Pedal percent is in range 0.0-100.0%
    float accelerator_pedal_percent   = App_CanRx_FSM_Apps_PappsMappedPedalPercentage_Get();
    float wheel_speed_front_left_kph  = App_CanRx_FSM_Wheels_LeftWheelSpeed_Get();
    float wheel_speed_front_right_kph = App_CanRx_FSM_Wheels_RightWheelSpeed_Get();

    if (accelerator_pedal_percent > 1.0f)
    {
        // Reset control loops if timeout elapsed
        TimerState timeout = App_Timer_UpdateAndGetState(&pid_timeout);
        if (timeout == TIMER_STATE_EXPIRED)
        {
            App_PID_RequestReset(&pid_power_correction);
            pid_power_correction_factor = 1.0f;
            App_PID_RequestReset(traction_control_inputs.pid);
        }
        App_Timer_Restart(&pid_timeout);

        // Get CAN messages re-used by multiple modules
        float motor_speed_left_rpm  = (float)App_CanRx_INVL_MotorPositionInfo_MotorSpeed_Get();
        float motor_speed_right_rpm = (float)App_CanRx_INVR_MotorPositionInfo_MotorSpeed_Get();

        // Power Limiting
        power_limiting_inputs.left_motor_temp_C  = App_CanRx_INVL_Temperatures3_MotorTemperature_Get();
        power_limiting_inputs.right_motor_temp_C = App_CanRx_INVR_Temperatures3_MotorTemperature_Get();
        // TODO(akoen): Available power will soon be replaced by current + voltage messages
        power_limiting_inputs.available_battery_power_kW = App_CanRx_BMS_AvailablePower_AvailablePower_Get();
        power_limiting_inputs.accelerator_pedal_percent  = accelerator_pedal_percent;
        float estimated_power_limit                      = App_PowerLimiting_ComputeMaxPower(&power_limiting_inputs);

        // Power limit correction
        float power_limit = estimated_power_limit * pid_power_correction_factor;

        // Active Differential
        active_differential_inputs.power_max_kW          = power_limit;
        active_differential_inputs.motor_speed_left_rpm  = motor_speed_left_rpm;
        active_differential_inputs.motor_speed_right_rpm = motor_speed_right_rpm;
        // TODO(akoen): Steering angle to wheel angle
        // active_differential_inputs.wheel_angle_deg = App_Can
        App_ActiveDifferential_ComputeTorque(&active_differential_inputs, &active_differential_outputs);

        // Traction Control
        traction_control_inputs.motor_speed_left_rpm        = motor_speed_left_rpm;
        traction_control_inputs.motor_speed_right_rpm       = motor_speed_right_rpm;
        traction_control_inputs.torque_left_Nm              = active_differential_outputs.torque_left_Nm;
        traction_control_inputs.torque_right_Nm             = active_differential_outputs.torque_right_Nm;
        traction_control_inputs.wheel_speed_front_left_kph  = wheel_speed_front_left_kph;
        traction_control_inputs.wheel_speed_front_right_kph = wheel_speed_front_right_kph;
        App_TractionControl_ComputeTorque(&traction_control_inputs, &traction_control_outputs);

        // Inverter Torque Requests
        App_CanTx_DCM_LeftInverterCommand_TorqueCommand_Set(traction_control_outputs.torque_left_final_Nm);
        App_CanTx_DCM_RightInverterCommand_TorqueCommand_Set(traction_control_outputs.torque_right_final_Nm);

        // Calculate power correction PID
        float battery_voltage         = App_CanRx_BMS_TractiveSystem_TsVoltage_Get();
        float current_consumption     = App_CanRx_BMS_TractiveSystem_TsCurrent_Get();
        float power_consumed_measured = battery_voltage * current_consumption;
        float power_consumed_ideal    = (motor_speed_left_rpm * traction_control_outputs.torque_left_final_Nm +
                                      motor_speed_right_rpm * traction_control_outputs.torque_right_final_Nm) /
                                     POWER_TO_TORQUE_CONVERSION_FACTOR;
        float power_consumed_estimate = power_consumed_ideal / pid_power_correction_factor;
        pid_power_correction_factor =
            App_PID_Compute(&pid_power_correction, power_consumed_measured, power_consumed_estimate);
    }
    else
    {
        regen_inputs.wheel_speed_front_left_kph  = wheel_speed_front_left_kph;
        regen_inputs.wheel_speed_front_right_kph = wheel_speed_front_right_kph;
        App_Regen_ComputeTorque(&regen_inputs, &regen_outputs);

        App_CanTx_DCM_LeftInverterCommand_TorqueCommand_Set(regen_outputs.regen_torque_left_Nm);
        App_CanTx_DCM_RightInverterCommand_TorqueCommand_Set(regen_outputs.regen_torque_right_Nm);
    }
}
