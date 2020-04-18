#include "states/App_ChargeState.h"

#include "App_SetPeriodicCanSignals.h"
#include "App_SharedMacros.h"

static void ChargeStateRunOnEntry(struct StateMachine *const state_machine)
{
    struct BmsWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct BmsCanTxInterface *can_tx_interface = App_BmsWorld_GetCanTx(world);
    App_CanTx_SetPeriodicSignal_STATE(
        can_tx_interface, CANMSGS_BMS_STATE_MACHINE_STATE_CHARGE_CHOICE);
}

static void ChargeStateRunOnTick(struct StateMachine *const state_machine)
{
    struct BmsWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct BmsCanTxInterface *can_tx = App_BmsWorld_GetCanTx(world);
    struct Imd *              imd    = App_BmsWorld_GetImd(world);

    App_Imd_Tick(imd);
    App_SetPeriodicCanSignals_Imd(can_tx, imd);
}

static void ChargeStateRunOnExit(struct StateMachine *const state_machine)
{
    UNUSED(state_machine);
}

const struct State *App_GetChargeState(void)
{
    static struct State charge_state = {
        .name         = "CHARGE",
        .run_on_entry = ChargeStateRunOnEntry,
        .run_on_tick  = ChargeStateRunOnTick,
        .run_on_exit  = ChargeStateRunOnExit,
    };

    return &charge_state;
}
