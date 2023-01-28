#include "states/App_AllStates.h"
#include "states/App_DriveState.h"
#include "App_SharedMacros.h"

static void InitStateRunOnEntry(struct StateMachine *const state_machine)
{
    struct DcmWorld *        world             = App_SharedStateMachine_GetWorld(state_machine);
    struct InverterSwitches *inverter_switches = App_DcmWorld_GetInverterSwitches(world);

    // JSONCAN -> App_CanTx_SetPeriodicSignal_STATE(can_tx_interface, CANMSGS_DCM_STATE_MACHINE_STATE_INIT_CHOICE);

    // Disable inverters and apply zero torque upon entering init state
    // JSONCAN -> App_CanTx_SetPeriodicSignal_INVERTER_ENABLE_INVL(
    // can_tx_interface, CANMSGS_DCM_INVL_COMMAND_MESSAGE_INVERTER_ENABLE_INVL_OFF_CHOICE);
    // JSONCAN -> App_CanTx_SetPeriodicSignal_INVERTER_ENABLE_INVR(
    // can_tx_interface, CANMSGS_DCM_INVR_COMMAND_MESSAGE_INVERTER_ENABLE_INVR_OFF_CHOICE);
    // JSONCAN -> App_CanTx_SetPeriodicSignal_TORQUE_COMMAND_INVL(
    // can_tx_interface, App_CanMsgs_dcm_invl_command_message_torque_command_invl_encode(0.0f));
    // JSONCAN -> App_CanTx_SetPeriodicSignal_TORQUE_COMMAND_INVR(
    // can_tx_interface, App_CanMsgs_dcm_invr_command_message_torque_command_invr_encode(0.0f));

    // Turn on inverter load switches
    App_InverterSwitches_TurnOnRight(inverter_switches);
    App_InverterSwitches_TurnOnLeft(inverter_switches);
}

static void InitStateRunOnTick1Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick1Hz(state_machine);
}

static void InitStateRunOnTick100Hz(struct StateMachine *const state_machine)
{
    if (App_AllStatesRunOnTick100Hz(state_machine))
    {
        struct DcmWorld *world = App_SharedStateMachine_GetWorld(state_machine);

        // Holds previous start switch position (true = UP/ON, false = DOWN/OFF)
        // Initialize to true to prevent a false start
        static bool prev_start_switch_pos = true;
        // TODO: JSONCAN -> const bool  curr_start_switch_pos      = App_IsStartSwitchOn(can_rx_interface);
        const bool curr_start_switch_pos      = false;
        const bool was_start_switch_pulled_up = !prev_start_switch_pos && curr_start_switch_pos;
        // const bool  is_brake_actuated          =
        // App_CanRx_FSM_BRAKE_FLAGS_GetSignal_BRAKE_IS_ACTUATED(can_rx_interface) ==
        //                                CANMSGS_FSM_BRAKE_FLAGS_BRAKE_IS_ACTUATED_TRUE_CHOICE;
        const bool is_brake_actuated = false;

        prev_start_switch_pos = curr_start_switch_pos;

        // TODO: JSONCAN
        //         if ((App_IsBmsInDriveState(can_rx_interface) && is_brake_actuated && was_start_switch_pulled_up)
        // #ifndef NDEBUG
        //             || App_CanRx_CAN_DEBUGGING_SIGNALS_GetSignal_FORCE_DCM_TO_DRIVE_FROM_INIT(can_rx_interface)
        // #endif
        //         )
        if (false)
        {
            // Transition to drive state when start-up conditions are passed (see
            // EV.10.4.3):
            App_SharedStateMachine_SetNextState(state_machine, App_GetDriveState());
        }
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
