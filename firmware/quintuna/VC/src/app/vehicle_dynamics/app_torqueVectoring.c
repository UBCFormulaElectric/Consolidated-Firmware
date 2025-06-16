#include "app_timer.h"
#include "app_torqueVectoring.h"
#include "app_vehicleDynamics.h"
#include "app_powerLimiting.h"
#include "app_activeDifferential.h"
#include "app_tractionControl.h"
#include "app_yawRateController.h"
#include "app_sbgEllipse.h"
#include "app_canRx.h"
#include "app_canTx.h"
#include "app_utils.h"
#include "app_imu.h"
#include "app_units.h"
#include "app_vehicleDynamicsConstants.h"
#include "app_sbgEllipse.h"
#include "app_torqueDistribution.h"

#define MOTOR_NOT_SPINNING_SPEED_RPM 1000
static TimerChannel           pid_timeout;
static TractionControl_Inputs traction_control_inputs;
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
static TorqueAllocationInputs torqueAllocation = { .front_yaw_moment     = 0.0f,
                                                   .load_transfer_const  = 0.0f,
                                                   .power_limit_kw       = 80.0f,
                                                   .rear_yaw_moment      = 0.0f,
                                                   .total_torque_request = 0.0f };
static PID                    yrc_pid;

static float accelerator_pedal_percent;
static float battery_voltage;
static float steering_angle_deg;
static float current_limit_based_max_power;

void app_torqueVectoring_init(void)
{
    app_canTx_VC_TorqueVectoringEnabled_set(true);
    app_pid_init(&pid_power_correction, &PID_POWER_CORRECTION_CONFIG);
    app_pid_init(&pid_traction_control, &PID_TRACTION_CONTROL_CONFIG);
    app_yawRateController_init(&yaw_rate_controller, &yrc_pid, &YAW_RATE_CONTROLLER_CONFIG);
    traction_control_inputs.pid = &pid_traction_control;

    app_timer_init(&pid_timeout, PID_TIMEOUT_ms);
}
// TODO: pass in the torque output pointer
void app_torqueVectoring_run(const float accelerator_pedal_percentage)
{
    // Read data from CAN
    // NOTE: Pedal percent CAN is in range 0.0-100.0%
    accelerator_pedal_percent = accelerator_pedal_percentage;
    battery_voltage           = app_canRx_BMS_TractiveSystemVoltage_get();
    steering_angle_deg        = app_canRx_FSM_SteeringAngle_get();
    imu_output                = app_get_imu_struct();
    // current_limit_based_max_power = app_canRx_BMS_current_limit();

    if (accelerator_pedal_percent > 0.0f)
    {
        app_torqueVectoring_handleAcceleration();
    }
}
// Read data from CAN
void app_torqueVectoring_handleAcceleration(void)
{
    // Power Limiting
    // Power limit correction
    float power_limit      = app_powerLimiting_computeMaxPower(RULES_BASED_POWER_LIMIT_KW); // -- HARD CODED FIX LATER
    float total_yaw_moment = 0.0f;
    float load_transfer_const = 1.0f;

    // Yaw Rate Controller
    yaw_rate_controller.wheel_angle_rad      = DEG_TO_RAD(steering_angle_deg * APPROX_STEERING_TO_WHEEL_ANGLE);
    yaw_rate_controller.vehicle_velocity_mps = KMH_TO_MPS(app_sbgEllipse_getVehicleVelocity());
    yaw_rate_controller.real_yaw_rate_rad    = DEG_TO_RAD(imu_output->yaw_rate);
    app_yawRateController_run(&yaw_rate_controller);

    app_canTx_VC_ReferenceYawRate_set(RAD_TO_DEG(app_yawRateController_getRefYawRateRad()));
    app_canTx_VC_CorrectionYawRate_set(app_yawRateController_getYawMoment());

    // imu load transfer calc
    torqueAllocation.load_transfer_const = app_loadTransferConstant(imu_output->long_accel);

    float desired_tot_yaw_moment          = app_yawRateController_getYawMoment();
    torqueAllocation.total_torque_request = accelerator_pedal_percent * (MAX_TORQUE_REQUEST_NM * 4);
    torqueAllocation.rear_yaw_moment      = total_yaw_moment / (1 + torqueAllocation.load_transfer_const);
    torqueAllocation.front_yaw_moment     = total_yaw_moment - torqueAllocation.rear_yaw_moment;
    // TODO: Adapt TV to torque allocation with pointers
    // app_torqueAllocation(&torqueAllocation);
    // app_torqueBroadCast();
}
