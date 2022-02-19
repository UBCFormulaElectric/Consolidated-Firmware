#include "states/App_FaultState.h"
#include "states/App_AllStates.h"
#include "App_SharedMacros.h"
#include "App_SharedExitCode.h"

static void FaultStateRunOnEntry(struct StateMachine *const state_machine)
{
    struct InvWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct InvCanTxInterface *can_tx_interface = App_InvWorld_GetCanTx(world);

    App_CanTx_SetPeriodicSignal_STATE(
        can_tx_interface, CANMSGS_INV_STATE_MACHINE_STATE_FAULT_CHOICE);
}

static void FaultStateRunOnTick1Hz(struct StateMachine *const state_machine)
{
    struct InvWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct InvCanTxInterface *can_tx_interface = App_InvWorld_GetCanTx(world);
    App_AllStatesRunOnTick1Hz(state_machine);
}

static void FaultStateRunOnTick100Hz(struct StateMachine *const state_machine)
{
    struct InvWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct InvCanTxInterface *can_tx_interface = App_InvWorld_GetCanTx(world);
    struct HeartbeatMonitor *heartbeat_monitor =
        App_InvWorld_GetHeartbeatMonitor(world);

    App_AllStatesRunOnTick100Hz(state_machine);
    App_CanTx_SetPeriodicSignal_STATE(
            can_tx_interface, CANMSGS_INV_STATE_MACHINE_STATE_FAULT_CHOICE);
    App_SharedHeartbeatMonitor_Tick(heartbeat_monitor);
}

static void FaultStateRunOnExit(struct StateMachine *const state_machine)
{
    UNUSED(state_machine);
}

const struct State *App_GetFaultState(void)
{
    static struct State fault_state = {
        .name              = "FAULT",
        .run_on_entry      = FaultStateRunOnEntry,
        .run_on_tick_1Hz   = FaultStateRunOnTick1Hz,
        .run_on_tick_100Hz = FaultStateRunOnTick100Hz,
        .run_on_exit       = FaultStateRunOnExit,
    };

    return &fault_state;
}
