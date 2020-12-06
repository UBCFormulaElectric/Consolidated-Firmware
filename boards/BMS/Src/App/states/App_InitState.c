#include "states/App_AllStates.h"
#include "states/App_InitState.h"
#include "states/App_ChargeState.h"
#include "states/App_FaultState.h"
#include "states/App_DriveState.h"

#include "App_SetPeriodicCanSignals.h"
#include "App_SharedMacros.h"

static void InitStateRunOnEntry(struct StateMachine *const state_machine)
{
    struct BmsWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct BmsCanTxInterface *can_tx_interface = App_BmsWorld_GetCanTx(world);
    struct Accumulator *      accumulator = App_BmsWorld_GetAccumulator(world);

    App_Accumulator_ConfigureCellMonitors(accumulator);

    App_CanTx_SetPeriodicSignal_STATE(
        can_tx_interface, CANMSGS_BMS_STATE_MACHINE_STATE_INIT_CHOICE);
}

static void InitStateRunOnTick1Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick1Hz(state_machine);
}

static void InitStateRunOnTick100Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick100Hz(state_machine);

    struct BmsWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct BmsCanTxInterface *can_tx = App_BmsWorld_GetCanTx(world);
    struct Imd *              imd    = App_BmsWorld_GetImd(world);
    struct PreChargeSequence *pre_charge_sequence =
        App_BmsWorld_GetPreChargeSequence(world);

    App_SetPeriodicCanSignals_Imd(can_tx, imd);

    enum PreChargingStatus pre_charging_status =
        App_PreChargeSequence_GetPreChargingStatus(pre_charge_sequence);
    if (pre_charging_status == PRE_CHARGING_SUCCESS)
    {
        App_SharedStateMachine_SetNextState(
            state_machine, App_GetChargeState());
    }
    else if (
        pre_charging_status == PRE_CHARGING_OVERVOLTAGE_ERROR ||
        pre_charging_status == PRE_CHARGING_UNDERVOLTAGE_ERROR)
    {
        App_SharedStateMachine_SetNextState(state_machine, App_GetFaultState());
    }
}

static void InitStateRunOnExit(struct StateMachine *const state_machine)
{
    UNUSED(state_machine);
}

const struct State *App_GetInitState(void)
{
    static struct State init_state = {
        .name              = "INIT",
        .run_on_entry      = InitStateRunOnEntry,
        .run_on_tick_1Hz   = InitStateRunOnTick1Hz,
        .run_on_tick_100Hz = InitStateRunOnTick100Hz,
        .run_on_exit       = InitStateRunOnExit,
    };

    return &init_state;
}
