#include "app_timer.h"
#include "app_torqueVectoring.h"
#include "app_vehicleDynamicsConstants.h"
#include "app_powerLimiting.h"
#include "app_activeDifferential.h"
#include "app_tractionControl.h"
#include "app_canRx.h"
#include "app_canTx.h"
#include "app_utils.h"

#define MOTOR_NOT_SPINNING_SPEED_RPM 1000
#define TRACTION_CONTROL_LOWER_BOUND -10.0f
#define TRACTION_CONTROL_UPPER_BOUND 10.0f
#define PEDAL_NOT_PRESSED            0.0f
#define STEERING_ANG_HYSTERSIS       5.0f
static TimerChannel pid_timeout; 

static PowerLimiting_Inputs       power_limiting_inputs = { .power_limit_kW = POWER_LIMIT_CAR_kW };
static ActiveDifferential_Inputs  active_differential_inputs;
static ActiveDifferential_Outputs active_differential_outputs;
static TractionControl_Inputs     traction_control_inputs;
static TractionControl_Outputs    traction_control_outputs;
static Torque_to_Inverters        torque_to_inverters; 

static bool run_traction_control = true;
static TVInteralStates torqueVectoringInternalState= OFF;

// NOTE: Correction factor centered about 0.0f

/**
 * No PID for now.
 */

static PID   pid_power_correction;
static float pid_power_correction_factor = 0.0f;
static PID   pid_traction_control;

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


/* static function forward references*/
static TVInteralStates noPedalInput();
static TVInteralStates runTractionControl(float power_limit);
static TVInteralStates runActiveDiff(float power_limit);
static void app_torqueVectoring_stateMachine(float pedal_percentage);

/* public functions */
void app_torqueVectoring_init(void)
{
    app_canTx_VC_TorqueVectoringEnabled_set(true);
    app_pid_init(&pid_power_correction, &PID_POWER_CORRECTION_CONFIG);
    app_pid_init(&pid_traction_control, &PID_TRACTION_CONTROL_CONFIG);
    traction_control_inputs.pid = &pid_traction_control;

    app_timer_init(&pid_timeout, PID_TIMEOUT_ms);
    torqueVectoringInternalState = OFF;
}

// Read data from CAN
void app_torqueVectoring_run(float pedal_percentage)
{
    // Reset control loops if timeout elapsed
    TimerState timeout = app_timer_updateAndGetState(&pid_timeout);
    if (timeout == TIMER_STATE_EXPIRED)
    {
        app_pid_requestReset(&pid_power_correction);
        app_pid_requestReset(&pid_traction_control);
    }
    app_timer_restart(&pid_timeout);

    app_torqueVectoring_stateMachine(pedal_percentage); 
    app_canTx_VC_TVInternalState_set(torqueVectoringInternalState);
    
    torque_to_inverters.torque_left_final_Nm  = traction_control_outputs.torque_left_Nm + active_differential_outputs.torque_left_Nm;
    torque_to_inverters.torque_right_final_Nm = traction_control_outputs.torque_right_Nm + active_differential_outputs.torque_right_Nm;

    // Limit asymptotic torques at zero speed
    if (motor_speed_left_rpm < MOTOR_NOT_SPINNING_SPEED_RPM || motor_speed_right_rpm < MOTOR_NOT_SPINNING_SPEED_RPM)
    {
        torque_to_inverters.torque_left_final_Nm  = accelerator_pedal_percent * MAX_TORQUE_REQUEST_NM;
        torque_to_inverters.torque_right_final_Nm = accelerator_pedal_percent * MAX_TORQUE_REQUEST_NM;
    }

    // CLAMPS for safety only - should never exceed torque limit
    torque_to_inverters.torque_left_final_Nm  = CLAMP(torque_to_inverters.torque_left_final_Nm, 0, MAX_TORQUE_REQUEST_NM);
    torque_to_inverters.torque_right_final_Nm = CLAMP(torque_to_inverters.torque_right_final_Nm, 0, MAX_TORQUE_REQUEST_NM);
    app_canTx_VC_LeftInverterTorqueCommand_set(torque_to_inverters.torque_left_final_Nm);
    app_canTx_VC_RightInverterTorqueCommand_set(torque_to_inverters.torque_right_final_Nm);

    // Calculate power correction PID
    float power_consumed_measured = battery_voltage * current_consumption;

    // Commented out: Method for calculating power consumed from traction control outputs
    // float power_consumed_ideal    = (motor_speed_left_rpm * traction_control_outputs.torque_left_final_Nm +
    //                             motor_speed_right_rpm * traction_control_outputs.torque_right_final_Nm) /
    //                             POWER_TO_TORQUE_CONVERSION_FACTOR;

    float power_consumed_ideal =
        (motor_speed_left_rpm * torque_to_inverters.torque_left_final_Nm + 
        motor_speed_right_rpm * torque_to_inverters.torque_right_final_Nm) /
        POWER_TO_TORQUE_CONVERSION_FACTOR;
    float power_consumed_estimate = power_consumed_ideal / (1.0f + pid_power_correction_factor);
    pid_power_correction_factor -=
        app_pid_compute(&pid_power_correction, power_consumed_measured, power_consumed_estimate);
    pid_power_correction_factor = CLAMP(pid_power_correction_factor, PID_POWER_FACTOR_MIN, PID_POWER_FACTOR_MAX);
    app_canTx_VC_PowerEstimateMeasuredValue_set(power_consumed_measured);
    app_canTx_VC_PowerEstimateValue_set(power_consumed_estimate);
    app_canTx_VC_PIDPowerEstimateOutput_set(pid_power_correction_factor);
    app_canTx_VC_PIDPowerEstimateError_set(pid_power_correction.error);
    app_canTx_VC_PIDPowerEstimateDerivative_set(pid_power_correction.derivative);
    app_canTx_VC_PIDPowerEstimateIntegral_set(pid_power_correction.integral);
}

/* Static functions*/

static void app_torqueVectoring_stateMachine(float accelerator_pedal_percentage)
{
    // Read data from CAN
    // NOTE: Pedal percent CAN is in range 0.0-100.0%
    accelerator_pedal_percent   = accelerator_pedal_percentage;
    wheel_speed_front_left_kph  = app_canRx_FSM_LeftWheelSpeed_get();
    wheel_speed_front_right_kph = app_canRx_FSM_RightWheelSpeed_get();
    motor_speed_left_rpm        = (float)app_canRx_INVL_MotorSpeed_get();
    motor_speed_right_rpm       = -1 * (float)app_canRx_INVR_MotorSpeed_get();
    battery_voltage             = app_canRx_BMS_TractiveSystemVoltage_get();
    current_consumption         = app_canRx_BMS_TractiveSystemCurrent_get();
    left_motor_temp_C           = app_canRx_INVL_MotorTemperature_get();
    right_motor_temp_C          = app_canRx_INVR_MotorTemperature_get();
    steering_angle_deg          = app_canRx_FSM_SteeringAngle_get();

    // Power Limiting
    power_limiting_inputs.left_motor_temp_C         = left_motor_temp_C;
    power_limiting_inputs.right_motor_temp_C        = right_motor_temp_C;
    power_limiting_inputs.accelerator_pedal_percent = accelerator_pedal_percent;
    float estimated_power_limit;
    estimated_power_limit = app_powerLimiting_computeMaxPower(&power_limiting_inputs);
    // Power limit correction
    float power_limit = estimated_power_limit * (1.0f + pid_power_correction_factor);

    // initializing active diff outputs
    active_differential_outputs.torque_left_Nm = 0.0; 
    active_differential_outputs.torque_right_Nm = 0.0;

    // initializing active diff outputs
    traction_control_outputs.torque_left_Nm = 0.0; 
    traction_control_outputs.torque_right_Nm = 0.0;

    if(accelerator_pedal_percent <= PEDAL_NOT_PRESSED && torqueVectoringInternalState != OFF)
        {
            torqueVectoringInternalState = noPedalInput();
            return; 
        }

    switch (torqueVectoringInternalState)
    {

        case OFF:

            if(accelerator_pedal_percent > 0.0f)
            {
                if(TRACTION_CONTROL_LOWER_BOUND <= steering_angle_deg && steering_angle_deg <= TRACTION_CONTROL_UPPER_BOUND)
                {
                    torqueVectoringInternalState = runTractionControl(power_limit);
                }
                else
                {
                    torqueVectoringInternalState = runActiveDiff(power_limit);
                }
            }

            break;
        
        case TRACTION_CONTROL:

            if(!run_traction_control)
            {
                torqueVectoringInternalState = runActiveDiff(power_limit);
                break;
            }

            // we do not nessecarily want hystersis to enter active diff from traction control as if we increase the window for to be activate for a larger angle window
            // we would be further assuming that there are little lateral forces in play at a steering angle where they very much may be in play
            if (TRACTION_CONTROL_LOWER_BOUND <= steering_angle_deg && steering_angle_deg <= TRACTION_CONTROL_UPPER_BOUND)
            {
                torqueVectoringInternalState = runTractionControl(power_limit);
            }

            else
            {
                torqueVectoringInternalState = runActiveDiff(power_limit);
            }
            break; 
        
        case ACTIVE_DIFF:

            if(run_traction_control)
            {
                // we implement hystersis here as we do not want to oscillate back into slip control by accident
                if ((TRACTION_CONTROL_LOWER_BOUND + STEERING_ANG_HYSTERSIS) <= steering_angle_deg && steering_angle_deg <= (TRACTION_CONTROL_UPPER_BOUND - STEERING_ANG_HYSTERSIS))
                {
                    torqueVectoringInternalState = runTractionControl(power_limit);
                }

            }

            else
            {
                torqueVectoringInternalState = runActiveDiff(power_limit);
            }
            break; 
        
        default:
            torqueVectoringInternalState = noPedalInput();
            break;
        }

    return; 
}

/* Event Handlers */

static TVInteralStates noPedalInput()
{
    app_canTx_VC_LeftInverterTorqueCommand_set(0.0f);
    app_canTx_VC_RightInverterTorqueCommand_set(0.0f);
    return OFF;
}

static TVInteralStates runTractionControl(float power_limit)
{
    if(torqueVectoringInternalState != TRACTION_CONTROL)
    {
        pid_traction_control.integral = 0; // WE DO NOT WANT TO KEEP RESETING ACCUMULATED ERROR TO 0 WHILE WE ARE CONTINUOUSLY IN TRACTION CONTORL
    }
    traction_control_inputs.motor_speed_left_rpm        = motor_speed_left_rpm;
    traction_control_inputs.motor_speed_right_rpm       = motor_speed_right_rpm;
    traction_control_inputs.torque_limit                = app_torqueVectoring_powerToTorque(power_limit,
                                                             motor_speed_left_rpm, 
                                                             motor_speed_right_rpm,
                                                             1,
                                                             1);
    traction_control_inputs.torque_left_Nm              = accelerator_pedal_percent * MAX_TORQUE_REQUEST_NM;
    traction_control_inputs.torque_right_Nm             = accelerator_pedal_percent * MAX_TORQUE_REQUEST_NM;
    traction_control_inputs.wheel_speed_front_left_kph  = wheel_speed_front_left_kph;
    traction_control_inputs.wheel_speed_front_right_kph = wheel_speed_front_right_kph;
    app_tractionControl_computeTorque(&traction_control_inputs, &traction_control_outputs);
    return TRACTION_CONTROL;
}

static TVInteralStates runActiveDiff(float power_limit)
{
        active_differential_inputs.power_max_kW          = power_limit; 
        active_differential_inputs.motor_speed_left_rpm  = motor_speed_left_rpm;
        active_differential_inputs.motor_speed_right_rpm = motor_speed_right_rpm;
        active_differential_inputs.wheel_angle_deg       = steering_angle_deg * APPROX_STEERING_TO_WHEEL_ANGLE;
        active_differential_inputs.requested_torque      = accelerator_pedal_percent * MAX_TORQUE_REQUEST_NM;
        app_activeDifferential_computeTorque(&active_differential_inputs, &active_differential_outputs);
        app_canTx_VC_ActiveDiffTorqueLeft_set(active_differential_outputs.torque_left_Nm);
        app_canTx_VC_ActiveDiffTorqueRight_set(active_differential_outputs.torque_right_Nm);
        return ACTIVE_DIFF;

}



float app_torqueVectoring_powerToTorque(
    float power_kW,
    float left_motor_speed_rpm,
    float right_motor_speed_rpm,
    float cl,
    float cr)
{
    return (POWER_TO_TORQUE_CONVERSION_FACTOR * power_kW) /
           (left_motor_speed_rpm * cl + right_motor_speed_rpm * cr + SMALL_EPSILON);
}