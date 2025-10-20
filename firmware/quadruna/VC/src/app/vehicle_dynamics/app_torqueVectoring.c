#include "app_timer.h"
#include "app_torqueVectoring.h"
#include "app_vehicleDynamicsConstants.h"
#include "app_powerLimiting.h"
#include "app_activeDifferential.h"
#include "app_tractionControl.h"
#include "app_yawRateController.h"
#include "app_sbgEllipse.h"
#include "app_canRx.h"
#include "app_canTx.h"
#include "app_utils.h"
#include "app_imu.h"
#include "app_torqueDistribution.h"
#include "app_units.h"

#define MOTOR_NOT_SPINNING_SPEED_RPM 1000
static TimerChannel pid_timeout;

static PowerLimiting_Inputs       power_limiting_inputs = { .power_limit_kW = POWER_LIMIT_CAR_kW };
static ActiveDifferential_Inputs  active_differential_inputs;
static ActiveDifferential_Outputs active_differential_outputs;
static TractionControl_Inputs     traction_control_inputs;
// static TractionControl_Outputs    traction_control_outputs;
static YawRateController yaw_rate_controller;

static bool run_traction_control = false;

// NOTE: Correction factor centered about 0.0f

/**
 * No PID for now.
 */

static PID                    pid_power_correction;
static float                  pid_power_correction_factor = 0.0f;
static PID                    pid_traction_control;
const ImuData                *imu_output;
static TorqueAllocationInputs torqueToLoadTransf;
static PID                    yrc_pid;

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
static float long_load_transfer_scalar;

void app_torqueVectoring_init(void)
{
    app_canTx_VC_TorqueVectoringEnabled_set(true);
    app_pid_init(&pid_power_correction, &PID_POWER_CORRECTION_CONFIG);
    app_pid_init(&pid_traction_control, &PID_TRACTION_CONTROL_CONFIG);
    app_yawRateController_init(&yaw_rate_controller, &yrc_pid, &YAW_RATE_CONTROLLER_CONFIG);
    traction_control_inputs.pid = &pid_traction_control;

    app_timer_init(&pid_timeout, PID_TIMEOUT_ms);
}

void app_torqueVectoring_run(float accelerator_pedal_percentage)
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
    imu_output                  = app_get_imu_struct();

    if (accelerator_pedal_percent > 0.0f)
    {
        app_torqueVectoring_handleAcceleration();
    }
    else
    {
        app_canTx_VC_LeftInverterTorqueCommand_set(0.0f);
        app_canTx_VC_RightInverterTorqueCommand_set(0.0f);
    }
}
// Read data from CAN
void app_torqueVectoring_handleAcceleration(void)
{
    // Reset control loops if timeout elapsed
    TimerState timeout = app_timer_updateAndGetState(&pid_timeout);
    if (timeout == TIMER_STATE_EXPIRED)
    {
        app_pid_requestReset(&pid_power_correction);
        app_pid_requestReset(&pid_traction_control);
    }
    app_timer_restart(&pid_timeout);

    // imu load transfer calc
    long_load_transfer_scalar = app_loadTransferConstant(imu_output->long_accel);

    // Power Limiting
    power_limiting_inputs.left_motor_temp_C         = left_motor_temp_C;
    power_limiting_inputs.right_motor_temp_C        = right_motor_temp_C;
    power_limiting_inputs.accelerator_pedal_percent = accelerator_pedal_percent;
    float estimated_power_limit;
    estimated_power_limit = app_powerLimiting_computeMaxPower(&power_limiting_inputs);
    // Power limit correction
    float power_limit = estimated_power_limit * (1.0f + pid_power_correction_factor);

    // Active Differential
    // active_differential_inputs.power_max_kW          = power_limit;
    // active_differential_inputs.motor_speed_left_rpm  = motor_speed_left_rpm;
    // active_differential_inputs.motor_speed_right_rpm = motor_speed_right_rpm;
    // active_differential_inputs.wheel_angle_deg       = steering_angle_deg * APPROX_STEERING_TO_WHEEL_ANGLE;
    // active_differential_inputs.requested_torque      = accelerator_pedal_percent * MAX_TORQUE_REQUEST_NM;
    // app_activeDifferential_computeTorque(&active_differential_inputs, &active_differential_outputs);
    // app_canTx_VC_ActiveDiffTorqueLeft_set(active_differential_outputs.torque_left_Nm);
    // app_canTx_VC_ActiveDiffTorqueRight_set(active_differential_outputs.torque_right_Nm);

    // Yaw Rate Controller
    yaw_rate_controller.wheel_angle_rad      = DEG_TO_RAD(steering_angle_deg * APPROX_STEERING_TO_WHEEL_ANGLE);
    yaw_rate_controller.vehicle_velocity_mps = KMH_TO_MPS(app_sbgEllipse_getVehicleVelocity());
    yaw_rate_controller.real_yaw_rate_rad    = DEG_TO_RAD(imu_output->yaw_rate);
    app_yawRateController_run(&yaw_rate_controller);

    app_canTx_VC_ReferenceYawRate_set(RAD_TO_DEG(app_yawRateController_getRefYawRateRad()));
    app_canTx_VC_YawMoment_set(app_yawRateController_getYawMoment());

    /**
     *  TRACTION CONTROL NOT TESTED ON CAR YET
     */
    // Traction Control
    // if (run_traction_control)
    // {
    //     traction_control_inputs.motor_speed_left_rpm        = motor_speed_left_rpm;
    //     traction_control_inputs.motor_speed_right_rpm       = motor_speed_right_rpm;
    //     traction_control_inputs.torque_left_Nm              = active_differential_outputs.torque_left_Nm;
    //     traction_control_inputs.torque_right_Nm             = active_differential_outputs.torque_right_Nm;
    //     traction_control_inputs.wheel_speed_front_left_kph  = wheel_speed_front_left_kph;
    //     traction_control_inputs.wheel_speed_front_right_kph = wheel_speed_front_right_kph;
    //     app_tractionControl_computeTorque(&traction_control_inputs, &traction_control_outputs);
    // }

    // Inverter Torque Request
    float desired_tot_yaw_moment = app_yawRateController_getYawMoment();
    torqueToLoadTransf.total_torque_request =
        accelerator_pedal_percent * (MAX_TORQUE_REQUEST_NM * 4) // 20.5 for each motor
                                    torqueToLoadTransf.rear_yaw_moment =
            desired_tot_yaw_moment / (1 + long_load_transfer_scalar);
    torqueToLoadTransf.front_yaw_moment = desired_tot_yaw_moment - torqueToLoadTransf.rear_yaw_moment;
    // if (run_traction_control)
    // {
    //     torque_left_final_Nm  = traction_control_outputs.torque_left_final_Nm;
    //     torque_right_final_Nm = traction_control_outputs.torque_right_final_Nm;
    // }
    // else
    // {
    //     torque_left_final_Nm  = active_differential_outputs.torque_left_Nm;
    //     torque_right_final_Nm = active_differential_outputs.torque_right_Nm;
    // }

    // Limit asymptotic torques at zero speed
    if (motor_speed_left_rpm < MOTOR_NOT_SPINNING_SPEED_RPM || motor_speed_right_rpm < MOTOR_NOT_SPINNING_SPEED_RPM)
    {
        torqueToLoadTransf.rear_left_motor_torque  = accelerator_pedal_percent * MAX_TORQUE_REQUEST_NM;
        torqueToLoadTransf.rear_right_motor_torque = accelerator_pedal_percent * MAX_TORQUE_REQUEST_NM;
    }

    // CLAMPS for safety only - should never exceed torque limit

    app_torqueAllocation(&torqueToLoadTransf, long_load_transfer_scalar);

    // Calculate power correction PID
    float power_consumed_measured = battery_voltage * current_consumption;

    // Commented out: Method for calculating power consumed from traction control outputs
    // float power_consumed_ideal    = (motor_speed_left_rpm * traction_control_outputs.torque_left_final_Nm +
    //                             motor_speed_right_rpm * traction_control_outputs.torque_right_final_Nm) /
    //                             POWER_TO_TORQUE_CONVERSION_FACTOR;

    float power_consumed_ideal = (motor_speed_left_rpm * torqueToLoadTransf.rear_left_motor_torque +
                                  motor_speed_right_rpm * torqueToLoadTransf.rear_right_motor_torque) /
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
