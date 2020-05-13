#include "states/App_AirClosedState.h"
#include "App_SetPeriodicCanSignals.h"
#include "App_SharedMacros.h"

static void AirClosedStateRunOnEntry(struct StateMachine *const state_machine)
{
    struct FsmWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct FsmCanTxInterface *can_tx_interface = App_FsmWorld_GetCanTx(world);
    App_CanTx_SetPeriodicSignal_STATE(
        can_tx_interface, CANMSGS_FSM_STATE_MACHINE_STATE_AIR_CLOSED_CHOICE);
}

static void AirClosedStateRunOnTick(struct StateMachine *const state_machine)
{
    struct FsmWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct FsmCanTxInterface *can_tx_interface = App_FsmWorld_GetCanTx(world);
    struct FlowMeter *        primary_flow_meter =
        App_FsmWorld_GetPrimaryFlowMeter(world);
    struct FlowMeter *secondary_flow_meter =
        App_FsmWorld_GetSecondaryFlowMeter(world);

    App_CanTx_SetPeriodicSignal_PRIMARY_FLOW_RATE(
        can_tx_interface, App_FlowMeter_GetFlowRate(primary_flow_meter));
    App_CanTx_SetPeriodicSignal_SECONDARY_FLOW_RATE(
        can_tx_interface, App_FlowMeter_GetFlowRate(secondary_flow_meter));
}

static void AirClosedStateRunOnExit(struct StateMachine *const state_machine)
{
    UNUSED(state_machine);
}

const struct State *App_GetAirClosedState(void)
{
    static struct State air_closed_state = {
        .name             = "AIR CLOSED",
        .run_on_entry     = AirClosedStateRunOnEntry,
        .run_on_tick_1kHz = AirClosedStateRunOnTick,
        .run_on_tick_1Hz  = NULL,
        .run_on_exit      = AirClosedStateRunOnExit,
    };

    return &air_closed_state;
}
