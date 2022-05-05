#include "states/App_AllStates.h"

#include "App_SetPeriodicCanSignals.h"
#include "App_SharedMacros.h"

static void PreChargeStateRunOnEntry(struct StateMachine *const state_machine)
{
    struct BmsWorld *         world            = App_SharedStateMachine_GetWorld(state_machine);
    struct BmsCanTxInterface *can_tx_interface = App_BmsWorld_GetCanTx(world);
    App_CanTx_SetPeriodicSignal_STATE(can_tx_interface, CANMSGS_BMS_STATE_MACHINE_STATE_PRE_CHARGE_CHOICE);
}

static void PreChargeStateRunOnTick1Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick1Hz(state_machine);
}

static void PreChargeStateRunOnTick100Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick100Hz(state_machine);
}

static void PreChargeStateRunOnExit(struct StateMachine *const state_machine)
{
    UNUSED(state_machine);
}

const struct State *App_GetPreChargeState(void)
{
    static struct State pre_charge_state = {
        .name              = "PRE_CHARGE",
        .run_on_entry      = PreChargeStateRunOnEntry,
        .run_on_tick_1Hz   = PreChargeStateRunOnTick1Hz,
        .run_on_tick_100Hz = PreChargeStateRunOnTick100Hz,
        .run_on_exit       = PreChargeStateRunOnExit,
    };

    return &pre_charge_state;
}
