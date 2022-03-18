#include "states/App_SharedStates.h"
#include "states/App_InitState.h"
#include "states/App_DriveState.h"
#include "states/App_FaultState.h"

#include "App_SharedMacros.h"

/**
 * Check if the brake pedal is actuated
 * @param can_rx_interface The CAN Rx interface to get the CAN signals from
 * @return true if the brake is actuated, false otherwise
 */
static inline bool App_SharedStates_IsBrakeActuated(
    const struct DcmCanRxInterface *can_rx_interface)
{
    return App_CanRx_FSM_BRAKE_GetSignal_BRAKE_IS_ACTUATED(can_rx_interface) ==
           CANMSGS_FSM_BRAKE_BRAKE_IS_ACTUATED_TRUE_CHOICE;
}

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

    struct DcmWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct DcmCanTxInterface *can_tx_interface = App_DcmWorld_GetCanTx(world);
    struct DcmCanRxInterface *can_rx_interface = App_DcmWorld_GetCanRx(world);
    struct ErrorTable *       error_table = App_DcmWorld_GetErrorTable(world);
    struct InverterSwitches * inverter_switches =
        App_DcmWorld_GetInverterSwitches(world);

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

    // Holds previous start switch position (true = UP/ON, false = DOWN/OFF)
    // Initialize to true to prevent a false start
    static bool prev_start_switch_position = true;
    const bool  current_start_switch_position =
        App_SharedStates_IsStartSwitchOn(can_rx_interface);
    const bool was_start_switch_pulled_up =
        !prev_start_switch_position && current_start_switch_position;
    const struct State *next_state = App_GetInitState();

    if (App_SharedErrorTable_HasAnyCriticalErrorSet(error_table) ||
        App_SharedStates_HasInverterFaulted(can_rx_interface))
    {
        // Transition to fault state if an inverter has faulted or
        // a critical error has been set
        next_state = App_GetFaultState();
    }
    else if (
        App_SharedStates_AreBothAIRsClosed(can_rx_interface) &&
        App_SharedStates_IsBrakeActuated(can_rx_interface) &&
        was_start_switch_pulled_up)
    {
        // Transition to drive state when start-up conditions are passed (see
        // EV.10.4.3):
        next_state = App_GetDriveState();
    }

    prev_start_switch_position = current_start_switch_position;
    App_SharedStateMachine_SetNextState(state_machine, next_state);
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
