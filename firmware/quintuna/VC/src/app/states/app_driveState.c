#include "app_states.h"

#include "app_canTx.h"
#include "app_canRx.h"

#include <app_utils.h>
#include "app_canAlerts.h"
#include "app_powerManager.h"
#include "app_warningHandling.h"

#include "app_vehicleDynamicsConstants.h"
#include "app_torqueVectoring.h"
#include "app_vehicleDynamics.h"
#include "app_startSwitch.h"
#include "app_controls.h"

static bool launch_control_switch_is_on;
static bool regen_switch_is_on;

static PowerManagerConfig power_manager_state = {
    .efuse_configs = { [EFUSE_CHANNEL_F_INV]   = { .efuse_enable = true, .timeout = 0, .max_retry = 5 },
                       [EFUSE_CHANNEL_RSM]     = { .efuse_enable = true, .timeout = 0, .max_retry = 5 },
                       [EFUSE_CHANNEL_BMS]     = { .efuse_enable = true, .timeout = 0, .max_retry = 5 },
                       [EFUSE_CHANNEL_R_INV]   = { .efuse_enable = true, .timeout = 0, .max_retry = 5 },
                       [EFUSE_CHANNEL_DAM]     = { .efuse_enable = true, .timeout = 0, .max_retry = 5 },
                       [EFUSE_CHANNEL_FRONT]   = { .efuse_enable = true, .timeout = 0, .max_retry = 5 },
                       [EFUSE_CHANNEL_RL_PUMP] = { .efuse_enable = true, .timeout = 200, .max_retry = 5 },
                       [EFUSE_CHANNEL_R_RAD]   = { .efuse_enable = true, .timeout = 200, .max_retry = 5 } }
};

static bool driveStatePassPreCheck(void)
{
    // All states module checks for faults, and returns whether or not a fault was detected.
    // const WarningType warning_check = app_warningHandling_globalWarningCheck();

    // should take priority over inverter fault as this will set torque to 0 and then
    // inverter retry can happen the next time the user enters drive state if inverters have faulted

    if (app_startSwitch_hasRisingEdge()) // wtf
    {
        app_stateMachine_setNextState(&hv_state);
        return false;
    }

    if (app_warningHandling_inverterStatus())
    {
        app_canAlerts_VC_Info_InverterRetry_set(true);
        // Go to inverter on state to unset the fault on the inverters and restart the sequence
        app_stateMachine_setNextState(&hvInit_state);
        return false;
    }

    if (app_warningHandling_boardWarningCheck())
    {
        return false;
    }

    return true;
}

static void app_enable_inv(void)
{
    app_canTx_VC_INVFRbEnable_set(true);
    app_canTx_VC_INVFLbEnable_set(true);
    app_canTx_VC_INVRRbEnable_set(true);
    app_canTx_VC_INVRLbEnable_set(true);

    app_canTx_VC_INVFLTorqueLimitPositive_set(NM_TO_INVERTER_TORQUE(MAX_TORQUE_REQUEST_NM));
    app_canTx_VC_INVFRTorqueLimitPositive_set(NM_TO_INVERTER_TORQUE(MAX_TORQUE_REQUEST_NM));
    app_canTx_VC_INVRLTorqueLimitPositive_set(NM_TO_INVERTER_TORQUE(MAX_TORQUE_REQUEST_NM));
    app_canTx_VC_INVRRTorqueLimitPositive_set(NM_TO_INVERTER_TORQUE(MAX_TORQUE_REQUEST_NM));

    app_canTx_VC_INVFLTorqueLimitNegative_set(-NM_TO_INVERTER_TORQUE(MAX_TORQUE_REQUEST_NM));
    app_canTx_VC_INVFRTorqueLimitNegative_set(-NM_TO_INVERTER_TORQUE(MAX_TORQUE_REQUEST_NM));
    app_canTx_VC_INVRLTorqueLimitNegative_set(-NM_TO_INVERTER_TORQUE(MAX_TORQUE_REQUEST_NM));
    app_canTx_VC_INVRRTorqueLimitNegative_set(-NM_TO_INVERTER_TORQUE(MAX_TORQUE_REQUEST_NM));
}

static void driveStateRunOnEntry(void)
{
    app_canTx_VC_State_set(VC_DRIVE_STATE);
    app_powerManager_updateConfig(power_manager_state);
    // Enable inverters
    app_enable_inv();
    app_torqueVectoring_init();

    // capture the state of the switches the moment you enter the drive state
    launch_control_switch_is_on = SWITCH_ON == app_canRx_CRIT_LaunchControlSwitch_get();
    regen_switch_is_on          = SWITCH_ON == app_canRx_CRIT_RegenSwitch_get();
}

static void driveStateRunOnTick100Hz(void)
{
    // Make sure you can only turn on VD in init and not during drive, only able to turn off
    regen_switch_is_on &= app_canRx_CRIT_RegenSwitch_get() == SWITCH_ON;
    launch_control_switch_is_on &= app_canRx_CRIT_LaunchControlSwitch_get() == SWITCH_ON;
    // Update Regen + TV LEDs
    app_canTx_VC_RegenEnabled_set(regen_switch_is_on);
    app_canTx_VC_TorqueVectoringEnabled_set(launch_control_switch_is_on);

    // ensure precheck and software bspd are good
    if (!driveStatePassPreCheck() ||
        app_warningHandling_checkSoftwareBspd(app_canRx_FSM_PappsMappedPedalPercentage_get() * 0.01f))
    {
        app_canTx_VC_INVFRTorqueSetpoint_set(0);
        app_canTx_VC_INVRRTorqueSetpoint_set(0);
        app_canTx_VC_INVFLTorqueSetpoint_set(0);
        app_canTx_VC_INVRLTorqueSetpoint_set(0);
        return;
    }

    const TorqueAllocationOutputs torqueToMotors = app_controls_calculateTorqueOutput();
    const int16_t nm_lb = NM_TO_INVERTER_TORQUE(MAX_REGEN_Nm), nm_ub = NM_TO_INVERTER_TORQUE(MAX_TORQUE_REQUEST_NM);
    app_canTx_VC_INVFLTorqueSetpoint_set(CLAMP(NM_TO_INVERTER_TORQUE(torqueToMotors.torque_fl), nm_ub, nm_ub));
    app_canTx_VC_INVFRTorqueSetpoint_set(CLAMP(NM_TO_INVERTER_TORQUE(torqueToMotors.torque_fr), nm_lb, nm_ub));
    app_canTx_VC_INVRLTorqueSetpoint_set(CLAMP(NM_TO_INVERTER_TORQUE(torqueToMotors.torque_rl), nm_lb, nm_ub));
    app_canTx_VC_INVRRTorqueSetpoint_set(CLAMP(NM_TO_INVERTER_TORQUE(torqueToMotors.torque_rr), nm_lb, nm_ub));
}

static void driveStateRunOnExit(void)
{
    // Disable inverters and apply zero torque upon exiting drive state
    app_canTx_VC_INVFRbEnable_set(false);
    app_canTx_VC_INVFLbEnable_set(false);
    app_canTx_VC_INVRRbEnable_set(false);
    app_canTx_VC_INVRLbEnable_set(false);

    app_canTx_VC_INVFRTorqueSetpoint_set(0);
    app_canTx_VC_INVRRTorqueSetpoint_set(0);
    app_canTx_VC_INVFLTorqueSetpoint_set(0);
    app_canTx_VC_INVRLTorqueSetpoint_set(0);

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