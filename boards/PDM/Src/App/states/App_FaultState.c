
#include "/home/formulae/Documents/Consolidated-Firmware/boards/PDM/Inc/App/states/App_AllStates.h"
#include "/home/formulae/Documents/Consolidated-Firmware/boards/PDM/Inc/App/states/App_InitState.h"
#include "/home/formulae/Documents/Consolidated-Firmware/boards/PDM/Inc/App/states/App_FaultState.h"
#include "/home/formulae/Documents/Consolidated-Firmware/boards/PDM/Inc/App/states/App_DriveState.h"
#include "App_SharedMacros.h"

static void FaultStateRunOnEntry(struct StateMachine *const state_machine)
{
    struct PdmWorld *const          world  = App_SharedStateMachine_GetWorld(state_machine);
    struct PdmCanTxInterface *const can_tx = App_PdmWorld_GetCanTx(world);
    // App_CanTx_SetPeriodicSignal_STATE(can_tx, CANMSGS_BMS_STATE_MACHINE_STATE_FAULT_CHOICE);
}

static void FaultStateRunOnTick1Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick1Hz(state_machine);
}

static void FaultStateRunOnTick100Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick100Hz(state_machine);

    struct PdmWorld *world = App_SharedStateMachine_GetWorld(state_machine);

    if ()
    {
        App_SharedStateMachine_SetNextState(state_machine, App_GetDriveState());
    }
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
