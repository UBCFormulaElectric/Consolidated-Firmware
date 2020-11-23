#include "states/App_AllStates.h"
#include "states/App_FaultState.h"

#include "App_SetPeriodicCanSignals.h"
#include "App_SharedMacros.h"

static void FaultStateRunOnEntry(struct StateMachine *const state_machine)
{
    struct BmsWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct BmsCanTxInterface *can_tx_interface = App_BmsWorld_GetCanTx(world);
    struct Accumulator *      accumulator = App_BmsWorld_GetAccumulator(world);

    App_Accumulator_SetDefaultCellTemperatureInRangeCheckMaxValue(accumulator);

    App_CanTx_SetPeriodicSignal_STATE(
        can_tx_interface, CANMSGS_BMS_STATE_MACHINE_STATE_FAULT_CHOICE);
}

static void FaultStateRunOnTick1Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick1Hz(state_machine);
}

static void FaultStateRunOnTick100Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick100Hz(state_machine);

    struct BmsWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct BmsCanTxInterface *can_tx = App_BmsWorld_GetCanTx(world);
    struct Imd *              imd    = App_BmsWorld_GetImd(world);

    App_SetPeriodicCanSignals_Imd(can_tx, imd);
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
