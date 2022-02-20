#include "states/App_StandbyState.h"
#include "states/App_AllStates.h"

static void StandbyStateRunOnEntry(struct StateMachine *const state_machine)
{
    struct InvWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct InvCanTxInterface *can_tx_interface = App_InvWorld_GetCanTx(world);

    App_CanTx_SetPeriodicSignal_STATE(
        can_tx_interface, CANMSGS_INV_STATE_MACHINE_STATE_STANDBY_CHOICE);
}

static void StandbyStateRunOnTick1Hz(struct StateMachine *const state_machine)
{
    struct InvWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct InvCanTxInterface *can_tx_interface = App_InvWorld_GetCanTx(world);
    App_AllStatesRunOnTick1Hz(state_machine);
}

static void StandbyStateRunOnTick100Hz(struct StateMachine *const state_machine)
{
    struct InvWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct InvCanTxInterface *can_tx_interface = App_InvWorld_GetCanTx(world);
    struct HeartbeatMonitor * heartbeat_monitor =
        App_InvWorld_GetHeartbeatMonitor(world);

    App_AllStatesRunOnTick100Hz(state_machine);
    App_CanTx_SetPeriodicSignal_STATE(
        can_tx_interface, CANMSGS_INV_STATE_MACHINE_STATE_STANDBY_CHOICE);
    App_SharedHeartbeatMonitor_Tick(heartbeat_monitor);
}

static void StandbyStateRunOnExit(struct StateMachine *const state_machine)
{
    UNUSED(state_machine);
}

const struct State *App_GetStandbyState(void)
{
    static struct State standby_state = {
        .name              = "STANDBY",
        .run_on_entry      = StandbyStateRunOnEntry,
        .run_on_tick_1Hz   = StandbyStateRunOnTick1Hz,
        .run_on_tick_100Hz = StandbyStateRunOnTick100Hz,
        .run_on_exit       = StandbyStateRunOnExit,
    };

    return &standby_state;
}
