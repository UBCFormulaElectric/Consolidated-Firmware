#include "states/App_AllStates.h"
#include "states/App_InitState.h"
#include "App_SharedMacros.h"

static void FaultStateRunOnEntry(struct StateMachine *const state_machine)
{
    struct BmsWorld *const world  = App_SharedStateMachine_GetWorld(state_machine);
    struct Airs *const     airs   = App_BmsWorld_GetAirs(world);
    struct OkStatus *      bms_ok = App_BmsWorld_GetBmsOkStatus(world);

    App_CanTx_BMS_Vitals_CurrentState_Set(BMS_FAULT_STATE);
    App_Airs_OpenAirPositive(airs);
    App_CanTx_BMS_Contactors_AirPositive_Set(
        App_Airs_IsAirPositiveClosed(airs) ? CONTACTOR_STATE_CLOSED : CONTACTOR_STATE_OPEN);
    App_OkStatus_Disable(bms_ok);
}

static void FaultStateRunOnTick1Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick1Hz(state_machine);
}

static void FaultStateRunOnTick100Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick100Hz(state_machine);

    struct BmsWorld *      world       = App_SharedStateMachine_GetWorld(state_machine);
    struct Accumulator *   accumulator = App_BmsWorld_GetAccumulator(world);
    struct TractiveSystem *ts          = App_BmsWorld_GetTractiveSystem(world);
    struct Airs *          airs        = App_BmsWorld_GetAirs(world);

    const bool acc_fault_cleared    = !App_Accumulator_CheckFaults(accumulator, ts);
    const bool ts_fault_cleared     = !App_TractveSystem_CheckFaults(ts);
    const bool is_air_negative_open = !App_Airs_IsAirNegativeClosed(airs);

    if (acc_fault_cleared && ts_fault_cleared && is_air_negative_open)
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
