#include "main.h"
#include "states/App_AllStates.h"
#include "states/App_InitState.h"
#include "App_SharedMacros.h"

static void FaultStateRunOnEntry(struct StateMachine *const state_machine)
{
    struct BmsWorld *const          world  = App_SharedStateMachine_GetWorld(state_machine);
    struct BmsCanTxInterface *const can_tx = App_BmsWorld_GetCanTx(world);

    App_CanTx_SetPeriodicSignal_STATE(can_tx, CANMSGS_BMS_STATE_MACHINE_STATE_FAULT_CHOICE);
    HAL_GPIO_WritePin(AIR_EN_GPIO_Port, AIR_EN_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(BMS_OK_GPIO_Port, BMS_OK_Pin, GPIO_PIN_RESET);
}

static void FaultStateRunOnTick1Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick1Hz(state_machine);
}

static void FaultStateRunOnTick100Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick100Hz(state_machine);

    struct BmsWorld *        world       = App_SharedStateMachine_GetWorld(state_machine);
    struct ErrorTable *const error_table = App_BmsWorld_GetErrorTable(world);

    bool is_error_table_cleared = !App_SharedErrorTable_HasAnyAirShutdownErrorSet(error_table);
    bool is_air_negative_open   = !(bool)HAL_GPIO_ReadPin(AIR_POWER_STATUS_GPIO_Port, AIR_POWER_STATUS_Pin);

    if (is_error_table_cleared && is_air_negative_open)
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
