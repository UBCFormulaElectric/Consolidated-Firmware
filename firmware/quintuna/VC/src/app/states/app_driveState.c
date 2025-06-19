#include "app_states.h"

#include "app_canTx.h"
#include "app_canRx.h"

#include <app_utils.h>
#include "app_canAlerts.h"
#include "app_powerManager.h"
#include "app_warningHandling.h"

#include "app_regen.h"
#include "app_vehicleDynamicsConstants.h"
#include "app_torqueVectoring.h"
#include "app_vehicleDynamics.h"
#include "app_torqueDistribution.h"
#include "app_driveHandling.h"

#define OFF 0

static bool                    launch_control_switch_is_on;
static bool                    regen_switch_is_on;
static TorqueAllocationOutputs torqueOutputToMotors;

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

static bool driveStatePassPreCheck()
{
    // All states module checks for faults, and returns whether or not a fault was detected.
    const WarningType warning_check = app_warningHandling_globalWarningCheck();

    // Make sure you can only turn on VD in init and not during drive, only able to turn off
    const bool prev_regen_switch_val = regen_switch_is_on;
    regen_switch_is_on               = app_canRx_CRIT_RegenSwitch_get() == SWITCH_ON && prev_regen_switch_val;

    const bool prev_launch_control_switch_is_on = launch_control_switch_is_on;
    launch_control_switch_is_on =
        app_canRx_CRIT_LaunchControlSwitch_get() == SWITCH_ON && prev_launch_control_switch_is_on;

    // should take priority over inverter fault as this will set torque to 0 and then
    // inverter retry can happen the next time the user enters drive state if inverters have faulted
    if (SWITCH_ON == app_canRx_CRIT_StartSwitch_get())
    {
        app_stateMachine_setNextState(&hv_state);
        return false;
    }

    if (INVERTER_FAULT == warning_check)
    {
        app_canAlerts_VC_Info_InverterRetry_set(true);
        // Go to inverter on state to unset the fault on the inverters and restart the sequence
        app_stateMachine_setNextState(&hvInit_state);
        return false;
    }

    if (BOARD_WARNING_DETECTED == warning_check)
    {
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
        app_canTx_VC_TorqueVectoringEnabled_set(false);
    }

    return true;
}

static void runDrivingAlgorithm(const float apps_pedal_percentage)
{
    // All states module checks for faults, and returns whether or not a fault was detected.
    WarningType warning_check = app_warningHandling_globalWarningCheck();

    // Make sure you can only turn on VD in init and not during drive, only able to turn off
    bool prev_regen_switch_val = regen_switch_is_on;
    regen_switch_is_on         = app_canRx_CRIT_RegenSwitch_get() == SWITCH_ON && prev_regen_switch_val;

    // bool prev_torque_vectoring_switch_val = torque_vectoring_switch_is_on;
    //  regen_switch_is_on = app_canRx_CRIT_VanillaOverrideSwitch_get() == SWITCH_ON &&
    //  prev_torque_vectoring_switch_val;

    /* TODO: Vehicle dyanmics people need to make sure to do a check if sensor init failed
        or not before using closed loop features */
    // Update Regen + TV LEDs

    if (apps_pedal_percentage < 0.0f && regen_switch_is_on)
    {
        app_regen_run(apps_pedal_percentage, &torqueOutputToMotors);
    }
    else if (SWITCH_ON == app_canRx_CRIT_VanillaOverrideSwitch_get())
    {
        app_vanillaDrive_run(apps_pedal_percentage, &torqueOutputToMotors);
    }
    else
    {
        app_driveMode_run(apps_pedal_percentage, &torqueOutputToMotors);
    }

    app_torqueBroadCast(&torqueOutputToMotors);
}

static void app_switchInit(void)
{
    if (SWITCH_ON == app_canRx_CRIT_LaunchControlSwitch_get())
    {
        launch_control_switch_is_on = true;
    }

    if (SWITCH_ON == app_canRx_CRIT_RegenSwitch_get())
    {
        regen_switch_is_on = true;
    }
}

static void app_enable_inv(void)
{
    app_canTx_VC_INVFRbEnable_set(true);
    app_canTx_VC_INVFLbEnable_set(true);
    app_canTx_VC_INVRRbEnable_set(true);
    app_canTx_VC_INVRLbEnable_set(true);

    app_canTx_VC_INVFLTorqueLimitPositive_set(PEDAL_REMAPPING(MAX_TORQUE_REQUEST_NM));
    app_canTx_VC_INVFRTorqueLimitPositive_set(PEDAL_REMAPPING(MAX_TORQUE_REQUEST_NM));
    app_canTx_VC_INVRLTorqueLimitPositive_set(PEDAL_REMAPPING(MAX_TORQUE_REQUEST_NM));
    app_canTx_VC_INVRRTorqueLimitPositive_set(PEDAL_REMAPPING(MAX_TORQUE_REQUEST_NM));

    app_canTx_VC_INVFLTorqueLimitNegative_set(-PEDAL_REMAPPING(MAX_TORQUE_REQUEST_NM));
    app_canTx_VC_INVFRTorqueLimitNegative_set(-PEDAL_REMAPPING(MAX_TORQUE_REQUEST_NM));
    app_canTx_VC_INVRLTorqueLimitNegative_set(-PEDAL_REMAPPING(MAX_TORQUE_REQUEST_NM));
    app_canTx_VC_INVRRTorqueLimitNegative_set(-PEDAL_REMAPPING(MAX_TORQUE_REQUEST_NM));
}

static void driveStateRunOnEntry()
{
    app_canTx_VC_State_set(VC_DRIVE_STATE);
    app_powerManager_updateConfig(power_manager_state);
    // Enable inverters
    app_enable_inv();
    app_switchInit();
    app_reset_torqueToMotors(&torqueOutputToMotors);
    app_torqueVectoring_init();
}

static void driveStateRunOnTick100Hz(void)
{
    // pedal mapped changed from [0, 100] to [0.0, 1.0]
    float apps_pedal_percentage = (float)app_canRx_FSM_PappsMappedPedalPercentage_get() * 0.01f;

    // ensure precheck and software bspd are good
    if (!driveStatePassPreCheck() || app_warningHandling_checkSoftwareBspd(apps_pedal_percentage))
    {
        app_canTx_VC_INVFRTorqueSetpoint_set(OFF);
        app_canTx_VC_INVRRTorqueSetpoint_set(OFF);
        app_canTx_VC_INVFLTorqueSetpoint_set(OFF);
        app_canTx_VC_INVRLTorqueSetpoint_set(OFF);
        return;
    }

    // regen switches pedal percentage from [0.0f, 1.0f] to [-0.2f, 0.8f] and then scaled to [-1.0f, 1.0f]
    if (regen_switch_is_on)
    {
        apps_pedal_percentage = app_regen_pedalRemapping(apps_pedal_percentage);
        // sapps_pedal_percentage = app_regen_pedalRemapping(sapps_pedal_percentage);
    }

    app_canTx_VC_RegenMappedPedalPercentage_set(apps_pedal_percentage * 100.0f);
    runDrivingAlgorithm(apps_pedal_percentage);
}

static void driveStateRunOnExit(void)
{
    // Disable inverters and apply zero torque upon exiting drive state
    app_canTx_VC_INVFRbEnable_set(false);
    app_canTx_VC_INVFLbEnable_set(false);
    app_canTx_VC_INVRRbEnable_set(false);
    app_canTx_VC_INVRLbEnable_set(false);

    app_canTx_VC_INVFRTorqueSetpoint_set(OFF);
    app_canTx_VC_INVRRTorqueSetpoint_set(OFF);
    app_canTx_VC_INVFLTorqueSetpoint_set(OFF);
    app_canTx_VC_INVRLTorqueSetpoint_set(OFF);

    app_reset_torqueToMotors(&torqueOutputToMotors);

    // Clear mapped pedal percentage
    app_canTx_VC_RegenMappedPedalPercentage_set(0.0f);
    app_canTx_VC_RegenEnabled_set(false);
    app_canTx_VC_TorqueVectoringEnabled_set(false);
}

    State drive_state = {
        .name              = "DRIVE",
        .run_on_entry      = driveStateRunOnEntry,
        .run_on_tick_100Hz = driveStateRunOnTick100Hz,
        .run_on_exit       = driveStateRunOnExit,
    };