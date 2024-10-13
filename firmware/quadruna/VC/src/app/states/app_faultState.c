#include "app_faultState.h"
#include "app_initState.h"
#include "app_allStates.h"

#include "app_canUtils.h"
#include "app_canTx.h"
#include "app_canRx.h"

#include "app_powerManager.h"
#include "app_faultCheck.h"

// TODO: When should we be entering fault state?

static const PowerStateConfig power_manager_fault = {
    .efuses = {
        [EFUSE_CHANNEL_SHDN] = true,
        [EFUSE_CHANNEL_LV] = true,
        [EFUSE_CHANNEL_PUMP] = false,
        [EFUSE_CHANNEL_AUX] = false,
        [EFUSE_CHANNEL_INV_R] = false,
        [EFUSE_CHANNEL_INV_L] = false,
        [EFUSE_CHANNEL_TELEM] = true,
        [EFUSE_CHANNEL_BUZZER] = false,
    },
};

static void faultStateRunOnEntry(void)
{
    app_canTx_VC_State_set(VC_FAULT_STATE);
    app_powerManager_updateConfig(power_manager_fault);
    app_canTx_VC_Fault_StateMachine_set(true);

    // Disable inverters and apply zero torque upon entering Fault state
    app_canTx_VC_LeftInverterEnable_set(false);
    app_canTx_VC_RightInverterEnable_set(false);
    app_canTx_VC_LeftInverterTorqueCommand_set(0.0f);
    app_canTx_VC_RightInverterTorqueCommand_set(0.0f);
    app_canTx_VC_LeftInverterTorqueLimit_set(0.0f);
    app_canTx_VC_RightInverterTorqueLimit_set(0.0f);
    app_canTx_VC_LeftInverterDirectionCommand_set(INVERTER_REVERSE_DIRECTION);
    app_canTx_VC_RightInverterDirectionCommand_set(INVERTER_FORWARD_DIRECTION);
}

static void faultStateRunOnTick1Hz(void)
{
    app_allStates_runOnTick1Hz();
}

static void faultStateRunOnTick100Hz(void)
{
    const bool any_board_has_fault = app_faultCheck_checkBoards();
    const bool inverter_has_fault  = app_faultCheck_checkInverters();
    const bool all_states_ok       = !(any_board_has_fault || inverter_has_fault);

    app_allStates_runOnTick100Hz();

    if (all_states_ok)
    {
        app_stateMachine_setNextState(app_initState_get());
    }
}

static void faultStateRunOnExit(void)
{
    app_canTx_VC_Fault_StateMachine_set(false);
}

const State *app_faultState_get(void)
{
    static State fault_state = {
        .name              = "FAULT",
        .run_on_entry      = faultStateRunOnEntry,
        .run_on_tick_1Hz   = faultStateRunOnTick1Hz,
        .run_on_tick_100Hz = faultStateRunOnTick100Hz,
        .run_on_exit       = faultStateRunOnExit,
    };

    return &fault_state;
}
