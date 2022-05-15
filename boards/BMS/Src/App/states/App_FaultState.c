#include "states/App_AllStates.h"
#include "states/App_InitState.h"
#include "App_SharedMacros.h"

static void FaultStateRunOnEntry(struct StateMachine *const state_machine)
{
    struct BmsWorld *const          world  = App_SharedStateMachine_GetWorld(state_machine);
    struct BmsCanTxInterface *const can_tx = App_BmsWorld_GetCanTx(world);
    struct Airs *const              airs   = App_BmsWorld_GetAirs(world);

    App_CanTx_SetPeriodicSignal_STATE(can_tx, CANMSGS_BMS_STATE_MACHINE_STATE_FAULT_CHOICE);
    App_Airs_OpenAirPositive(airs);
    App_CanTx_SetPeriodicSignal_AIR_POSITIVE(can_tx, App_Airs_IsAirPositiveClosed(airs));
}

static void FaultStateRunOnTick1Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick1Hz(state_machine);
}

static void FaultStateRunOnTick100Hz(struct StateMachine *const state_machine)
{
    struct BmsWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct Airs *    airs  = App_BmsWorld_GetAirs(world);

    if (App_AllStatesRunOnTick100Hz(state_machine) && !App_Airs_IsAirNegativeClosed(airs))
    {
        App_SharedStateMachine_SetNextState(state_machine, App_GetInitState());
    }
}

static void FaultStateRunOnExit(struct StateMachine *const state_machine)
{
    UNUSED(state_machine);
}

const struct State *App_GetFaultState()
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
