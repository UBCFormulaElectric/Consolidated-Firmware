#include "io_log.h"
#ifdef TARGET_EMBEDDED
#include "io_canTx.h"
#endif

#include "app_canTx.h"
#include "app_canRx.h"
#include <app_utils.h>
#include "app_signal.h"
#include "app_vehicleDynamicsConstants.h"
#include "app_torqueVectoring.h"
#include "app_regen.h"
#include "app_states.h"
#include "app_powerManager.h"
#include "app_warningHandling.h"
#include "app_canAlerts.h"

#define EFFICIENCY_ESTIMATE (0.80f)
#define BUZZER_ON_DURATION_MS 2000
#define INV_OFF 0

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
                       [EFUSE_CHANNEL_L_RAD]   = { .efuse_enable = true, .timeout = 200, .max_retry = 5 },
                       [EFUSE_CHANNEL_R_RAD]   = { .efuse_enable = true, .timeout = 200, .max_retry = 5 } }
};

static void runDrivingAlgorithm(float apps_pedal_percentage, float sapps_pedal_percentage);
static bool driveStatePassPreCheck(void);
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

    app_canTx_VC_INVFLTorqueLimitNegative_set(((-1) * (MAX_TORQUE_REQUEST_NM)));
    app_canTx_VC_INVFRTorqueLimitNegative_set(((-1) * (MAX_TORQUE_REQUEST_NM)));
    app_canTx_VC_INVRLTorqueLimitNegative_set(((-1) * (MAX_TORQUE_REQUEST_NM)));
    app_canTx_VC_INVRRTorqueLimitNegative_set(((-1) * (MAX_TORQUE_REQUEST_NM)));

    if (app_canRx_CRIT_VanillaOverrideSwitch_get() == SWITCH_ON)
    {
        // app_torqueVectoring_init(); -- COMMENTED OUT TO SPIN
        torque_vectoring_switch_is_on = true;
    }

    if (app_canRx_CRIT_RegenSwitch_get() == SWITCH_ON)
    {
        // app_regen_init(); -- COMMENTED OUT TO SPIN
        regen_switch_is_on = true;
    }
}

static void driveStateRunOnTick100Hz(void)
{
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

    // regen switched pedal percentage from [0, 100] to [0.0, 1.0] to [-0.3, 0.7] and then scaled to [-1,1]
    // if (regen_switch_is_on) -- COMMENTED OUT FOR WHEEL SPINNING
    // {
    //     apps_pedal_percentage  = (float) app_regen_pedalRemapping(apps_pedal_percentage);
    //     sapps_pedal_percentage = (float)app_regen_pedalRemapping(sapps_pedal_percentage);
    // }

    // app_canTx_VC_MappedPedalPercentage_set((float)apps_pedal_percentage);
    runDrivingAlgorithm((float)apps_pedal_percentage, (float)sapps_pedal_percentage);
}

static void driveStateRunOnExit(void) {}

static bool driveStatePassPreCheck()
{
    // All states module checks for faults, and returns whether or not a fault was detected.
    WarningType warning_check = app_warningHandling_globalWarningCheck();

    // Make sure you can only turn on VD in init and not during drive, only able to turn off
    bool prev_regen_switch_val = regen_switch_is_on;
    regen_switch_is_on         = app_canRx_CRIT_RegenSwitch_get() == SWITCH_ON && prev_regen_switch_val;

    bool prev_torque_vectoring_switch_val = torque_vectoring_switch_is_on;
    // regen_switch_is_on = app_canRx_CRIT_VanillaOverrideSwitch_get() == SWITCH_ON && prev_torque_vectoring_switch_val;

    /* TODO: Vehicle dyanmics people need to make sure to do a check if sensor init failed
        or not before using closed loop features */
    // Update Regen + TV LEDs

    if (INVERTER_FAULT == warning_check)
    {
        app_canAlerts_VC_Info_InverterRetry_set(true);
        // Go to inverter on state to unset the fault on the inverters and restart the sequence
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

    if (!regen_switch_is_on)
    {
        app_canTx_VC_RegenEnabled_set(false);
        app_canTx_VC_Info_RegenNotAvailable_set(true);
    }

    if (!torque_vectoring_switch_is_on)
    {
        app_canTx_VC_TorqueVectoringEnabled_set(true);
    }

    return true;
}

static void runDrivingAlgorithm(float apps_pedal_percentage, float sapps_pedal_percentage)
{
    // TODO: bring back when software BSPD is done
    if (app_warningHandling_checkSoftwareBspd(apps_pedal_percentage, sapps_pedal_percentage))
    {
        // If bspd warning is true, set torque to 0.0
        app_canTx_VC_INVFRTorqueSetpoint_set(INV_OFF);
        app_canTx_VC_INVRRTorqueSetpoint_set(INV_OFF);
        app_canTx_VC_INVFLTorqueSetpoint_set(INV_OFF);
        app_canTx_VC_INVRLTorqueSetpoint_set(INV_OFF);
        return;
    }
    //  if (apps_pedal_percentage < 0.0f && regen_switch_is_on)
    //  {
    //      app_regen_run(apps_pedal_percentage);
    //  }
    //  else if (torque_vectoring_switch_is_on)
    //  {
    //      app_torqueVectoring_run(apps_pedal_percentage);
    //  }
    //  else
    //  {
    app_regularDrive_run(apps_pedal_percentage);
    // }

    // TODO: we want to add two more driving modes... just Power limiting and Power limiting and active diff
}

static void app_regularDrive_run(float apps_pedal_percentage)
{
    // TODO: Use power limiting in regular drive
    // TODO: Implement active diff  in regular drive at min
    // const float bms_available_power         = (float)app_canRx_BMS_AvailablePower_get();
    // const float right_front_motor_speed_rpm = (float)app_canRx_INVR_MotorSpeed_get();
    // const float right_back_motor_speed_rpm  = (float)app_canRx_INVR_MotorSpeed_get();
    // const float left_front_motor_speed_rpm  = (float)app_canRx_INVL_MotorSpeed_get();
    // const float left_back_motor_speed_rpm   = (float)app_canRx_INVL_MotorSpeed_get();
    // float       bms_torque_limit            = MAX_TORQUE_REQUEST_NM;

    // if ((right_front_motor_speed_rpm + right_back_motor_speed_rpm + left_front_motor_speed_rpm +
    //      left_back_motor_speed_rpm) > 0.0f)
    // {
    //     // Estimate the maximum torque request to draw the maximum power available from the BMS
    //     const float available_output_power_w = bms_available_power * EFFICIENCY_ESTIMATE;
    //     const float combined_motor_speed_rads =
    //         RPM_TO_RADS(right_front_motor_speed_rpm) + RPM_TO_RADS(right_back_motor_speed_rpm) +
    //         RPM_TO_RADS(left_front_motor_speed_rpm) + RPM_TO_RADS(left_back_motor_speed_rpm);
    //     bms_torque_limit = MIN(available_output_power_w / combined_motor_speed_rads, MAX_TORQUE_REQUEST_NM);
    // }

    // Calculate the maximum torque request, according to the BMS available power
    // const float max_bms_torque_request = apps_pedal_percentage * bms_torque_limit;

    const float pedal_based_torque = MIN((apps_pedal_percentage * MAX_TORQUE_REQUEST_NM), MAX_TORQUE_REQUEST_NM);

    // Calculate the actual torque request to transmit ---- VERY IMPORTANT NEED TO MAKE A TORQUE TRANSMISSION FUNCTION
    // data sheet says that the inverter expects a 16 bit signed int and that our sent request is scaled by 0.1
    int16_t torque_request = (int16_t)((pedal_based_torque));

    // Transmit torque command to both inverters
    app_canTx_VC_INVFRTorqueSetpoint_set(torque_request);
    app_canTx_VC_INVRRTorqueSetpoint_set(torque_request);
    app_canTx_VC_INVFLTorqueSetpoint_set(torque_request);
    app_canTx_VC_INVRLTorqueSetpoint_set(torque_request);
}

State drive_state = {
    .name              = "DRIVE",
    .run_on_entry      = driveStateRunOnEntry,
    .run_on_tick_100Hz = driveStateRunOnTick100Hz,
    .run_on_exit       = driveStateRunOnExit,
};
