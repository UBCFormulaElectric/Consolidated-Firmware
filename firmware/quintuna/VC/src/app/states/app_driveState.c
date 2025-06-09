#include "io_log.h"
#ifdef TARGET_EMBEDDED
#include "io_canTx.h"
#endif

#include "app_canTx.h"
#include "app_canRx.h"
#include "app_units.h"
#include "app_utils.h"
#include "app_signal.h"
#include "app_vehicleDynamicsConstants.h"
#include "app_torqueVectoring.h"
#include "app_regen.h"
#include "app_states.h"
#include "app_powerManager.h"
#include "app_warningHanding.h"
#include "app_canAlerts.h"
#include "app_vehicleDynamics.h"

#define EFFICIENCY_ESTIMATE (0.80f)
#define BUZZER_ON_DURATION_MS 2000
#define INV_OFF 0

static bool         launch_control_switch_is_on;
static bool         launch_control_switch_is_on;
static bool         regen_switch_is_on;
static TimerChannel buzzer_timer;
static SensorChecks sensor_checks;

#define EFFICIENCY_ESTIMATE (0.80f)
#define BUZZER_ON_DURATION_MS 2000

static bool         torque_vectoring_switch_is_on;
static bool         regen_switch_is_on;
static TimerChannel buzzer_timer;

static PowerManagerConfig power_manager_state = {
    .efuse_configs = { [EFUSE_CHANNEL_F_INV]   = { .efuse_enable = true, .timeout = 0, .max_retry = 5 },
                       [EFUSE_CHANNEL_RSM]     = { .efuse_enable = true, .timeout = 0, .max_retry = 5 },
                       [EFUSE_CHANNEL_BMS]     = { .efuse_enable = true, .timeout = 0, .max_retry = 5 },
                       [EFUSE_CHANNEL_R_INV]   = { .efuse_enable = true, .timeout = 0, .max_retry = 5 },
                       [EFUSE_CHANNEL_DAM]     = { .efuse_enable = true, .timeout = 0, .max_retry = 5 },
                       [EFUSE_CHANNEL_FRONT]   = { .efuse_enable = true, .timeout = 0, .max_retry = 5 },
                       [EFUSE_CHANNEL_RL_PUMP] = { .efuse_enable = true, .timeout = 200, .max_retry = 5 },
                       [EFUSE_CHANNEL_RR_PUMP] = { .efuse_enable = true, .timeout = 200, .max_retry = 5 },
                       [EFUSE_CHANNEL_F_PUMP]  = { .efuse_enable = true, .timeout = 200, .max_retry = 5 },
                       [EFUSE_CHANNEL_L_RAD]   = { .efuse_enable = true, .timeout = 200, .max_retry = 5 },
                       [EFUSE_CHANNEL_R_RAD]   = { .efuse_enable = true, .timeout = 200, .max_retry = 5 } }
};

static void runDrivingAlgorithm(float apps_pedal_percentage, float sapps_pedal_percentage);
static bool driveStatePassPreCheck();
static void app_regularDrive_run(float apps_pedal_percentage);

static void driveStateRunOnEntry()
{
    // Enable buzzer on transition to drive, and start 2s timer.
    app_timer_init(&buzzer_timer, BUZZER_ON_DURATION_MS);
    app_timer_restart(&buzzer_timer);

    app_canTx_VC_State_set(VC_DRIVE_STATE);
    app_powerManager_updateConfig(power_manager_state);

    // Enable inverters
    app_canTx_VC_INVFRbEnable_set(true);
    app_canTx_VC_INVFLbEnable_set(true);
    app_canTx_VC_INVRRbEnable_set(true);
    app_canTx_VC_INVRLbEnable_set(true);

    app_canTx_VC_INVFLTorqueLimitPositive_set(MAX_TORQUE_REQUEST_NM);
    app_canTx_VC_INVFRTorqueLimitPositive_set(MAX_TORQUE_REQUEST_NM);
    app_canTx_VC_INVRLTorqueLimitPositive_set(MAX_TORQUE_REQUEST_NM);
    app_canTx_VC_INVRRTorqueLimitPositive_set(MAX_TORQUE_REQUEST_NM);

    app_canTx_VC_INVFLTorqueLimitNegative_set((int32_t)((-1) * (MAX_TORQUE_REQUEST_NM)));
    app_canTx_VC_INVFRTorqueLimitNegative_set((int32_t)((-1) * (MAX_TORQUE_REQUEST_NM)));
    app_canTx_VC_INVRLTorqueLimitNegative_set((int32_t)((-1) * (MAX_TORQUE_REQUEST_NM)));
    app_canTx_VC_INVRRTorqueLimitNegative_set((int32_t)((-1) * (MAX_TORQUE_REQUEST_NM)));

    if (app_canRx_CRIT_TorqueVecSwitch_get() == SWITCH_ON)
    {
        app_torqueVectoring_init();
        torque_vectoring_switch_is_on = true;
    }

    if (app_canRx_CRIT_RegenSwitch_get() == SWITCH_ON)
    {
        app_regen_init();
        regen_switch_is_on = true;
    }
}

static void driveStateRunOnTick100Hz(void)
{
    // pedal mapped changed from [0, 100] to [0.0, 1.0]
    float apps_pedal_percentage  = (float)app_canRx_FSM_PappsMappedPedalPercentage_get() * 0.01f;
    float sapps_pedal_percentage = (float)app_canRx_FSM_SappsMappedPedalPercentage_get() * 0.01f;

    if (!driveStatePassPreCheck())
    {
        app_canTx_VC_INVFRTorqueSetpoint_set(INV_OFF);
        app_canTx_VC_INVRRTorqueSetpoint_set(INV_OFF);
        app_canTx_VC_INVFLTorqueSetpoint_set(INV_OFF);
        app_canTx_VC_INVRLTorqueSetpoint_set(INV_OFF);
        return;
    }

    // regen switches pedal percentage from [0.0f, 1.0f] to [-0.2f, 0.8f] and then scaled to [-1.0f, 1.0f]
    if (regen_switch_is_on)
    {
        apps_pedal_percentage  = app_regen_pedalRemapping(apps_pedal_percentage);
        sapps_pedal_percentage = app_regen_pedalRemapping(sapps_pedal_percentage);
    }

    app_canTx_VC_MappedPedalPercentage_set(apps_pedal_percentage * 100.0f);
    runDrivingAlgorithm(apps_pedal_percentage, sapps_pedal_percentage);
}

static void driveStateRunOnExit(void)
{
    //     // Disable inverters and apply zero torque upon exiting drive state
    //     app_canTx_VC_LeftInverterEnable_set(false);
    //     app_canTx_VC_RightInverterEnable_set(false);

    //     app_canTx_VC_LeftInverterTorqueCommand_set(0.0f);
    //     app_canTx_VC_RightInverterTorqueCommand_set(0.0f);

    //     // Clear mapped pedal percentage
    //     app_canTx_VC_MappedPedalPercentage_set(0.0f);

    //     // Clear latched inverter faults
    //     app_canTx_VC_INVL_CommandParameterAddress_set((uint16_t)20);
    //     app_canTx_VC_INVL_CommandReadWrite_set(true);
    //     app_canTx_VC_INVL_CommandData_set((uint16_t)0);

    //     app_canTx_VC_INVR_CommandParameterAddress_set((uint16_t)20);
    //     app_canTx_VC_INVR_CommandReadWrite_set(true);
    //     app_canTx_VC_INVR_CommandData_set((uint16_t)0);

    // #ifdef TARGET_EMBEDDED
    //     io_canTx_VC_INVL_ReadWriteParamCommand_sendAperiodic();
    //     io_canTx_VC_INVL_ReadWriteParamCommand_sendAperiodic();
    //     io_canTx_VC_INVR_ReadWriteParamCommand_sendAperiodic();
    //     io_canTx_VC_INVR_ReadWriteParamCommand_sendAperiodic();
    // #endif
    //     // Disable buzzer on exit drive.
    //     io_efuse_setChannel(EFUSE_CHANNEL_BUZZER, false);
    //     app_canTx_VC_BuzzerOn_set(false);

    //     app_canTx_VC_RegenEnabled_set(false);
    //     app_canTx_VC_TorqueVectoringEnabled_set(false);
}

static bool driveStatePassPreCheck()
{
    // All states module checks for faults, and returns whether or not a fault was detected.
    warningType warning_check = app_warningHandling_globalWarningCheck();

    // Make sure you can only turn on VD in init and not during drive, only able to turn off
    bool prev_regen_switch_val = regen_switch_is_on;
    regen_switch_is_on         = app_canRx_CRIT_RegenSwitch_get() == SWITCH_ON && prev_regen_switch_val;

    bool prev_launch_control_switch_is_on = launch_control_switch_is_on;
    launch_control_switch_is_on =
        app_canRx_CRIT_LaunchControlSwitch_get() == SWITCH_ON && prev_launch_control_switch_is_on;

    /* TODO: Vehicle dyanmics people need to make sure to do a check if sensor init failed
        or not before using closed loop features */

    if (INVERTER_FAULT == warning_check)
    {
        app_canAlerts_VC_Info_InverterRetry_set(true);
        app_stateMachine_setNextState(&hvInit_state);
        // MAKE FUNCTION IN TORQUE DISTRIBUTION WHEN 4WD merged
        return false;
    }

    else if (BOARD_WARNING_DETECTED == warning_check)
    {
        return false;
    }

    if (SWITCH_OFF == app_canRx_CRIT_StartSwitch_get())
    {
        app_stateMachine_setNextState(&hv_state);
        return false;
    }

    // Update Regen + TV LEDs
    if (!regen_switch_is_on)
    {
        app_canTx_VC_RegenEnabled_set(false);
        app_canTx_VC_Info_RegenNotAvailable_set(true);
    }

    if (!launch_control_switch_is_on)
    {
        app_canTx_VC_TorqueVectoringEnabled_set(true);
    }

    return true;
}

static void runDrivingAlgorithm(float apps_pedal_percentage, float sapps_pedal_percentage)
{
    app_performSensorChecks();

    // TODO: bring back when software BSPD is done
    //  if (app_faultCheck_checkSoftwareBspd(apps_pedal_percentage, sapps_pedal_percentage))
    //  {
    //      // If bspd warning is true, set torque to 0.0
    //      app_canTx_VC_INVFRTorqueSetpoint_set(INV_OFF);
    //      app_canTx_VC_INVRRTorqueSetpoint_set(INV_OFF);
    //      app_canTx_VC_INVFLTorqueSetpoint_set(INV_OFF);
    //      app_canTx_VC_INVRLTorqueSetpoint_set(INV_OFF);
    //  }

    if (SWITCH_ON == app_canRx_CRIT_VanillaOverrideSwitch_get())
    {
        app_regularDrive_run(apps_pedal_percentage);
    }
    else
    {
    }
    // TODO: we want to add two more driving modes... just Power limiting and Power limiting and active diff
}

static void app_regularDrive_run(float apps_pedal_percentage)
{
    // TODO: Use power limiting in regular drive
    // TODO: Implement active diff  in regular drive at min
    // TODO: Use sensor checks here to disable things accordingly (active diff, load trans)
    // TODO: set Load Transfer Const = 1 and set Desired Yaw Moment = 0

    // TODO: Implement active diff in regular drive at min
    const float bms_available_power         = (float)app_canRx_BMS_AvailablePower_get();
    const float right_front_motor_speed_rpm = (float)app_canRx_INVR_MotorSpeed_get();
    const float right_back_motor_speed_rpm  = (float)app_canRx_INVR_MotorSpeed_get();
    const float left_front_motor_speed_rpm  = (float)app_canRx_INVL_MotorSpeed_get();
    const float left_back_motor_speed_rpm   = (float)app_canRx_INVL_MotorSpeed_get();
    float       bms_torque_limit            = MAX_TORQUE_REQUEST_NM;

    if ((right_front_motor_speed_rpm + right_back_motor_speed_rpm + left_front_motor_speed_rpm +
         left_back_motor_speed_rpm) > 0.0f)
    {
        // Estimate the maximum torque request to draw the maximum power available from the BMS
        const float available_output_power_w = bms_available_power * EFFICIENCY_ESTIMATE;
        const float combined_motor_speed_rads =
            RPM_TO_RADS(right_front_motor_speed_rpm) + RPM_TO_RADS(right_back_motor_speed_rpm) +
            RPM_TO_RADS(left_front_motor_speed_rpm) + RPM_TO_RADS(left_back_motor_speed_rpm);
        bms_torque_limit = MIN(available_output_power_w / combined_motor_speed_rads, MAX_TORQUE_REQUEST_NM);
    }

    // Calculate the maximum torque request, according to the BMS available power
    const float max_bms_torque_request = apps_pedal_percentage * bms_torque_limit;

    const float pedal_based_torque = MIN((apps_pedal_percentage * MAX_TORQUE_REQUEST_NM), 1);

    // Calculate the actual torque request to transmit ---- VERY IMPORTANT NEED TO MAKE A TORQUE TRANSMISSION FUNCTION
    // data sheet says that the inverter expects a 16 bit signed int and that our sent request is scaled by 0.1
    int16_t torque_request = (int16_t)((pedal_based_torque));

    // Transmit torque command to both inverters
    app_canTx_VC_INVFRTorqueSetpoint_set(torque_request);
    app_canTx_VC_INVRRTorqueSetpoint_set(torque_request);
    app_canTx_VC_INVFLTorqueSetpoint_set(torque_request);
    app_canTx_VC_INVRLTorqueSetpoint_set(torque_request);
}

static void app_driveSwitchInit(void)
{
    if (SWITCH_ON == app_canRx_CRIT_LaunchControlSwitch_get())
    {
        // app_torqueVectoring_init(); -- COMMENTED OUT TO SPIN
        launch_control_switch_is_on = true;
    }

    if (SWITCH_ON == app_canRx_CRIT_RegenSwitch_get())
    {
        // app_regen_init(); -- COMMENTED OUT TO SPIN
        regen_switch_is_on = true;
    }
}

static app_driveMode_driving(void)
{
    DriveMode driveMode = app_canRx_CRIT_DriveMode_get();
}

static void app_non_vanilla_driving(float apps_pedal_percentage, float sapps_pedal_percentage)
{
    if (apps_pedal_percentage < 0.0f && regen_switch_is_on)
    {
        app_regen_run(apps_pedal_percentage);
    }
    else
    {
        // as of now no launch control is being implemented thus if the regen switch is not on then we stick to our
        // drive modes
    }
}

static void app_performSensorChecks(void)
{
    sensor_checks.gpsOk = !app_canTx_VC_Info_SbgInitFailed_get();
    sensor_checks.imuOk = !app_canTx_VC_Info_ImuInitFailed_get();
    sensor_checks.steeringOk =
        !(app_canRx_FSM_Info_SteeringAngleOCSC_get() || app_canRx_FSM_Info_SteeringAngleOutOfRange_get());
    sensor_checks.useTV = sensor_checks.gpsOk && sensor_checks.imuOk && sensor_checks.steeringOk;
}

State drive_state = {
    .name              = "DRIVE",
    .run_on_entry      = driveStateRunOnEntry,
    .run_on_tick_100Hz = driveStateRunOnTick100Hz,
    .run_on_exit       = driveStateRunOnExit,
};