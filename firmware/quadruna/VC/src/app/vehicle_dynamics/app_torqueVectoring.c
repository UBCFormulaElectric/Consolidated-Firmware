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
#define TRACTION_CONTROL_STEER_LOWER_BOUND -10.0f
#define TRACTION_CONTROL_STEER_UPPER_BOUND 10.0f
#define PEDAL_NOT_PRESSED 0.0f
#define STEERING_ANG_HYSTERSIS 5.0f
#define STATE_ON_WEIGHT 1.0f
#define STATE_OFF_WEIGHT 0.0f

static PowerLimiting_Inputs       power_limiting_inputs = { .power_limit_kW = POWER_LIMIT_CAR_kW };
static ActiveDifferential_Inputs  active_differential_inputs;
static ActiveDifferential_Outputs active_differential_outputs;
static TractionControl_Inputs     traction_control_inputs;
static TractionControl_Outputs    traction_control_outputs;
static Torque_to_Inverters        torque_to_inverters;

/* Traction Control Variables*/
static bool                  run_traction_control = true;
static TorqueVectoringStates current_torqueVectoring_state;
static float                 tractionControlWeight;
static float                 activeDiffWeight;
static PID                   pid_traction_control;

/* TV variables */
static float accelerator_pedal_percent;
static float motor_speed_left_rpm;
static float motor_speed_right_rpm;
static float battery_voltage;
static float current_consumption;
static float steering_angle_deg;
static float power_limit;

/* static function forward references*/
static void  noPedalInput();
static void  runTractionControl();
static void  runActiveDiff();
static void  app_torqueVectoring_stateMachine();
static void  app_torqueVectoringTransitionHandler();
static void  app_torqueVectoringStateTickHandler();
static void  activeDiff_run_on_entry();
static void  activeDiff_run_on_exit();
static void  tractionControl_run_on_entry();
static void  tractionControl_run_on_exit();
static float app_torqueVectoring_powerToTorque(
    float power_kW,
    float left_motor_speed_rpm,
    float right_motor_speed_rpm,
    float cl,
    float cr);

/* public functions */
void app_torqueVectoring_init(void)
{
    app_canTx_VC_TorqueVectoringEnabled_set(true);
    current_torqueVectoring_state = TV_OFF;
    activeDiffWeight              = STATE_OFF_WEIGHT;

    // initializing active diff outputs
    active_differential_outputs.torque_left_Nm  = 0.0;
    active_differential_outputs.torque_right_Nm = 0.0;

    // initializing active diff outputs
    traction_control_outputs.torque_left_Nm  = 0.0;
    traction_control_outputs.torque_right_Nm = 0.0;
    tractionControlWeight                    = STATE_OFF_WEIGHT;
}

// Read data from CAN
void app_torqueVectoring_run(float pedal_percentage)
{
    // // Read data from CAN
    // NOTE: Pedal percent CAN is in range 0.0-100.0%
    accelerator_pedal_percent = pedal_percentage;
    motor_speed_left_rpm      = (float)app_canRx_INVL_MotorSpeed_get();
    motor_speed_right_rpm     = -1 * (float)app_canRx_INVR_MotorSpeed_get();
    battery_voltage           = app_canRx_BMS_TractiveSystemVoltage_get();
    current_consumption       = app_canRx_BMS_TractiveSystemCurrent_get();
    steering_angle_deg        = app_canRx_FSM_SteeringAngle_get();

    // Power Limiting
    power_limiting_inputs.left_motor_temp_C         = app_canRx_INVL_MotorTemperature_get();
    power_limiting_inputs.right_motor_temp_C        = app_canRx_INVR_MotorTemperature_get();
    power_limiting_inputs.accelerator_pedal_percent = pedal_percentage;
    power_limit                                     = app_powerLimiting_computeMaxPower(&power_limiting_inputs);

    app_torqueVectoring_stateMachine();
    app_canTx_VC_TVState_set(current_torqueVectoring_state);

    /* Torque computation using state weights */
    traction_control_outputs.torque_left_Nm *= tractionControlWeight;
    traction_control_outputs.torque_right_Nm *= tractionControlWeight;
    active_differential_outputs.torque_left_Nm *= activeDiffWeight;
    active_differential_outputs.torque_right_Nm *= activeDiffWeight;

    app_canTx_VC_ActiveDiffTorqueLeft_set(active_differential_outputs.torque_left_Nm);
    app_canTx_VC_ActiveDiffTorqueRight_set(active_differential_outputs.torque_right_Nm);
    app_canTx_VC_TractionControlTorqueLeft_set(traction_control_outputs.torque_left_Nm);
    app_canTx_VC_TractionControlTorqueRight_set(traction_control_outputs.torque_right_Nm);

    /* Torque to send computation */
    torque_to_inverters.torque_left_final_Nm =
        traction_control_outputs.torque_left_Nm + active_differential_outputs.torque_left_Nm;
    torque_to_inverters.torque_right_final_Nm =
        traction_control_outputs.torque_right_Nm + active_differential_outputs.torque_right_Nm;

    // CLAMPS for safety only - should never exceed torque limit
    app_canTx_VC_LeftInverterTorqueCommand_set(
        CLAMP(torque_to_inverters.torque_left_final_Nm, 0, MAX_TORQUE_REQUEST_NM));
    app_canTx_VC_RightInverterTorqueCommand_set(
        CLAMP(torque_to_inverters.torque_right_final_Nm, 0, MAX_TORQUE_REQUEST_NM));

    // Calculate power correction PID
    float power_consumed_measured = battery_voltage * current_consumption;
    // Commented out: Method for calculating power consumed from traction control outputs
    float power_consumed_ideal = (motor_speed_left_rpm * torque_to_inverters.torque_left_final_Nm +
                                  motor_speed_right_rpm * torque_to_inverters.torque_right_final_Nm) /
                                 POWER_TO_TORQUE_CONVERSION_FACTOR;

    app_canTx_VC_PowerConsumedMeasuredValue_set(power_consumed_measured);
    app_canTx_VC_PowerConsumedIdeal_set(power_consumed_ideal);
}

/* Static functions*/

static void app_torqueVectoring_stateMachine()
{
    app_torqueVectoringTransitionHandler();
    app_torqueVectoringStateTickHandler();
}

/* transition handler */

static void app_torqueVectoringTransitionHandler()
{
    switch (current_torqueVectoring_state)
    {
        case TV_OFF:
            if (accelerator_pedal_percent > PEDAL_NOT_PRESSED)
            {
                if (fabsf(steering_angle_deg) <= TRACTION_CONTROL_STEER_UPPER_BOUND &&
                    run_traction_control) // just to make it shorter
                {
                    tractionControl_run_on_entry();
                    current_torqueVectoring_state = TRACTION_CONTROL;
                }

                else
                {
                    activeDiff_run_on_entry();
                    current_torqueVectoring_state = ACTIVE_DIFF;
                }
            }

            break;
        case ACTIVE_DIFF:
            if (accelerator_pedal_percent > PEDAL_NOT_PRESSED)
            {
                if ((TRACTION_CONTROL_STEER_LOWER_BOUND + STEERING_ANG_HYSTERSIS) <= steering_angle_deg &&
                    steering_angle_deg <= (TRACTION_CONTROL_STEER_UPPER_BOUND - STEERING_ANG_HYSTERSIS))
                {
                    if (run_traction_control)
                    {
                        activeDiff_run_on_exit();
                        tractionControl_run_on_entry();
                        current_torqueVectoring_state = TRACTION_CONTROL;
                    }
                }
            }
            else
            {
                activeDiff_run_on_exit();
                current_torqueVectoring_state = TV_OFF;
            }

            break;

        case TRACTION_CONTROL:
            if (accelerator_pedal_percent > PEDAL_NOT_PRESSED)
            {
                if (fabsf(steering_angle_deg) > TRACTION_CONTROL_STEER_UPPER_BOUND)
                {
                    tractionControl_run_on_exit();
                    activeDiff_run_on_entry();
                    current_torqueVectoring_state = ACTIVE_DIFF;
                }
            }
            else
            {
                tractionControl_run_on_exit();
                current_torqueVectoring_state = TV_OFF;
            }
            break;
        default:
            current_torqueVectoring_state = TV_OFF; // final safety measure
            break;
    }
}

/* tick handler*/
static void app_torqueVectoringStateTickHandler()
{
    switch (current_torqueVectoring_state)
    {
        case TV_OFF:
            noPedalInput();
            break;
        case ACTIVE_DIFF:
            runActiveDiff();
            break;
        case TRACTION_CONTROL:
            runTractionControl();
            break;
        default:
            noPedalInput(); // final safety measure
            break;
    }
}

static void noPedalInput()
{
    activeDiffWeight      = STATE_OFF_WEIGHT;
    tractionControlWeight = STATE_OFF_WEIGHT;
}

static void runTractionControl()
{
    traction_control_inputs.motor_speed_left_rpm  = motor_speed_left_rpm;
    traction_control_inputs.motor_speed_right_rpm = motor_speed_right_rpm;
    traction_control_inputs.torque_limit =
        app_torqueVectoring_powerToTorque(power_limit, motor_speed_left_rpm, motor_speed_right_rpm, 1, 1);
    traction_control_inputs.torque_left_Nm              = accelerator_pedal_percent * MAX_TORQUE_REQUEST_NM;
    traction_control_inputs.torque_right_Nm             = accelerator_pedal_percent * MAX_TORQUE_REQUEST_NM;
    traction_control_inputs.wheel_speed_front_left_kph  = app_canRx_FSM_LeftWheelSpeed_get();
    traction_control_inputs.wheel_speed_front_right_kph = app_canRx_FSM_RightWheelSpeed_get();
    app_tractionControl_computeTorque(&traction_control_inputs, &traction_control_outputs);
}

static void activeDiff_run_on_entry()
{
    activeDiffWeight = STATE_ON_WEIGHT;
}

static void activeDiff_run_on_exit()
{
    activeDiffWeight = STATE_OFF_WEIGHT;
}

static void tractionControl_run_on_entry()
{
    app_pid_init(&pid_traction_control, &PID_TRACTION_CONTROL_CONFIG);
    traction_control_inputs.pid = &pid_traction_control;
    tractionControlWeight       = STATE_ON_WEIGHT;
}

static void tractionControl_run_on_exit()
{
    // ensure no error is calculated and integral error is removed
    pid_traction_control.Kp       = 0.0f;
    pid_traction_control.Ki       = 0.0f;
    pid_traction_control.integral = 0.0f;
    pid_traction_control.Kd       = 0.0f;
    tractionControlWeight         = STATE_OFF_WEIGHT;
}

static void runActiveDiff()
{
    active_differential_inputs.power_max_kW          = power_limit;
    active_differential_inputs.motor_speed_left_rpm  = motor_speed_left_rpm;
    active_differential_inputs.motor_speed_right_rpm = motor_speed_right_rpm;
    active_differential_inputs.wheel_angle_deg       = steering_angle_deg * APPROX_STEERING_TO_WHEEL_ANGLE;
    active_differential_inputs.requested_torque      = accelerator_pedal_percent * MAX_TORQUE_REQUEST_NM;
    app_activeDifferential_computeTorque(&active_differential_inputs, &active_differential_outputs);
}

static float app_torqueVectoring_powerToTorque(
    float power_kW,
    float left_motor_speed_rpm,
    float right_motor_speed_rpm,
    float cl,
    float cr)
{
    return (POWER_TO_TORQUE_CONVERSION_FACTOR * power_kW) /
           (left_motor_speed_rpm * cl + right_motor_speed_rpm * cr + SMALL_EPSILON);
}