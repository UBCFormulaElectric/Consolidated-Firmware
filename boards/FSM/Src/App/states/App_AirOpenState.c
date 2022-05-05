#include "states/App_AllStates.h"
#include "states/App_AirOpenState.h"
#include "states/App_AirClosedState.h"
#include "App_SetPeriodicCanSignals.h"
#include "App_SharedMacros.h"

static void AirOpenStateRunOnEntry(struct StateMachine *const state_machine)
{
    struct FsmWorld *         world            = App_SharedStateMachine_GetWorld(state_machine);
    struct FsmCanTxInterface *can_tx_interface = App_FsmWorld_GetCanTx(world);
    App_CanTx_SetPeriodicSignal_STATE(can_tx_interface, CANMSGS_FSM_STATE_MACHINE_STATE_AIR_OPEN_CHOICE);
}

static void AirOpenStateRunOnTick1Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick1Hz(state_machine);
}

static void AirOpenStateRunOnTick100Hz(struct StateMachine *const state_machine)
{
    struct FsmWorld *         world  = App_SharedStateMachine_GetWorld(state_machine);
    struct FsmCanRxInterface *can_rx = App_FsmWorld_GetCanRx(world);

    App_SetPeriodicSignals_FlowRateInRangeChecks(world);
    App_SetPeriodicSignals_WheelSpeedInRangeChecks(world);
    App_SetPeriodicSignals_SteeringAngleInRangeCheck(world);
    App_SetPeriodicSignals_Brake(world);
    App_SetPeriodicSignals_AcceleratorPedal(world);
    App_SetPeriodicSignals_MotorShutdownFaults(world);

    if (App_CanRx_BMS_AIR_STATES_GetSignal_AIR_POSITIVE(can_rx) == CANMSGS_BMS_AIR_STATES_AIR_POSITIVE_CLOSED_CHOICE &&
        App_CanRx_BMS_AIR_STATES_GetSignal_AIR_NEGATIVE(can_rx) == CANMSGS_BMS_AIR_STATES_AIR_NEGATIVE_CLOSED_CHOICE)
    {
        App_SharedStateMachine_SetNextState(state_machine, App_GetAirClosedState());
    }
}

static void AirOpenStateRunOnExit(struct StateMachine *const state_machine)
{
    UNUSED(state_machine);
}

const struct State *App_GetAirOpenState(void)
{
    static struct State air_open_state = {
        .name              = "AIR OPEN",
        .run_on_entry      = AirOpenStateRunOnEntry,
        .run_on_tick_1Hz   = AirOpenStateRunOnTick1Hz,
        .run_on_tick_100Hz = AirOpenStateRunOnTick100Hz,
        .run_on_exit       = AirOpenStateRunOnExit,
    };

    return &air_open_state;
}
