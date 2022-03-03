#include "states/App_SharedStates.h"
#include "states/App_InitState.h"
#include "states/App_DriveState.h"
#include "states/App_FaultState.h"

#include "App_SharedMacros.h"

static void InitStateRunOnEntry(struct StateMachine *const state_machine)
{
    struct DcmWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct DcmCanTxInterface *can_tx_interface = App_DcmWorld_GetCanTx(world);

    App_CanTx_SetPeriodicSignal_STATE(
        can_tx_interface, CANMSGS_DCM_STATE_MACHINE_STATE_INIT_CHOICE);
}

static void InitStateRunOnTick1Hz(struct StateMachine *const state_machine)
{
    App_SharedStatesRunOnTick1Hz(state_machine);
}

static void InitStateRunOnTick100Hz(struct StateMachine *const state_machine)
{
    App_SharedStatesRunOnTick100Hz(state_machine);

    // Holds previous start switch position (true = UP/ON, false = DOWN/OFF)
    static bool prev_start_switch_position =
        true; // Initialize to true to prevent a false start

    struct DcmWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct DcmCanTxInterface *can_tx_interface = App_DcmWorld_GetCanTx(world);
    struct DcmCanRxInterface *can_rx_interface = App_DcmWorld_GetCanRx(world);
    struct ErrorTable *       error_table = App_DcmWorld_GetErrorTable(world);
    const struct State *      next_state  = App_GetInitState();
    struct InverterSwitches * inverter_switches =
        App_DcmWorld_GetInverterSwitches(world);
    const bool current_start_switch_position =
        App_SharedStates_IsStartSwitchOn(can_rx_interface);

    App_CanTx_SetPeriodicSignal_TORQUE_REQUEST(can_tx_interface, 0.0f);

#ifdef DEBUG // Enter drive state directly from init state
    App_SharedStateMachine_SetNextState(state_machine, App_GetDriveState());
    return;
#endif

    // Provide LV to inverters when both AIRS are closed and DC bus voltage ~
    // 400 V
    if (App_SharedStates_AreBothAIRsClosed(can_rx_interface) &&
        !App_InverterSwitches_IsRightOn(inverter_switches))
    {
        App_InverterSwitches_TurnOnRight(inverter_switches);
    }
    if (App_SharedStates_AreBothAIRsClosed(can_rx_interface) &&
        !App_InverterSwitches_IsLeftOn(inverter_switches))
    {
        App_InverterSwitches_TurnOnLeft(inverter_switches);
    }

    // Transition to fault state if an inverter has faulted or
    // a critical error has been set
    if (App_SharedErrorTable_HasAnyCriticalErrorSet(error_table) ||
        App_SharedStates_HasInverterFaulted(can_rx_interface))
    {
        next_state = App_GetFaultState();
    }
    // The DCM will only enter the drive state when the following conditions
    // are met and the start switch is pulled up (based on EV.10.4.3):
    //
    //  - No critical errors are present
    //  - Both AIRs are closed a.k.a tractive system active.
    //  - Brake pedal is pressed
    else if (
        App_SharedStates_AreBothAIRsClosed(can_rx_interface) &&
        App_SharedStates_IsBrakeActuated(can_rx_interface) &&
        App_SharedStates_WasStartSwitchPulledUp(
            prev_start_switch_position, current_start_switch_position))
    {
        next_state = App_GetDriveState();
    }

    App_SharedStateMachine_SetNextState(state_machine, next_state);
    prev_start_switch_position = current_start_switch_position;
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
