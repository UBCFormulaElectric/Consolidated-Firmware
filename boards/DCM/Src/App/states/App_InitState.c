#include "states/App_AllStates.h"
#include "states/App_InitState.h"
#include "states/App_DriveState.h"
#include "states/App_FaultState.h"

#include "App_SharedMacros.h"

/**
 * @brief Holds previous start switch position (true = UP/ON, false = DOWN/OFF)
 */
static bool prev_start_switch_position;

static void InitStateRunOnEntry(struct StateMachine *const state_machine)
{
    struct DcmWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct DcmCanRxInterface *can_rx_interface = App_DcmWorld_GetCanRx(world);
    struct DcmCanTxInterface *can_tx_interface = App_DcmWorld_GetCanTx(world);

    App_CanTx_SetPeriodicSignal_STATE(
        can_tx_interface, CANMSGS_DCM_STATE_MACHINE_STATE_INIT_CHOICE);

    /* Disable inverter and apply zero torque upon entering init state */
    App_CanTx_SetPeriodicSignal_INVERTER_ENABLE_INVL(
        can_tx_interface,
        CANMSGS_DCM_INVL_COMMAND_MESSAGE_INVERTER_ENABLE_INVL_OFF_CHOICE);
    App_CanTx_SetPeriodicSignal_TORQUE_COMMAND_INVL(
        can_tx_interface,
        App_CanMsgs_dcm_invl_command_message_torque_command_invl_encode(0.0));

    prev_start_switch_position =
        App_CanRx_DIM_SWITCHES_GetSignal_START_SWITCH(can_rx_interface);
}

static void InitStateRunOnTick1Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick1Hz(state_machine);
}

static void InitStateRunOnTick100Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick100Hz(state_machine);

    struct DcmWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct DcmCanRxInterface *can_rx_interface = App_DcmWorld_GetCanRx(world);
    struct ErrorTable *       error_table = App_DcmWorld_GetErrorTable(world);

    /* Transition to fault state if the inverter itself is in the fault state */
    if (App_CanRx_INVL_INTERNAL_STATES_GetSignal_D1_VSM_STATE_INVL(
            can_rx_interface) ==
        CANMSGS_INVL_INTERNAL_STATES_D1_VSM_STATE_INVL_BLINK_FAULT_CODE_STATE_CHOICE)
    {
        App_SharedStateMachine_SetNextState(state_machine, App_GetFaultState());
    }
    else
    {
        /*
         * The DCM will only enter the drive state when the following conditions
         * are met and the start switch is pulled up (based on EV.10.4.3):
         *
         *  - No critical errors are present
         *  - Both AIRs are closed a.k.a tractive system active.
         *  - Brake pedal is pressed
         *  - Both inverters are in the ready state
         */
        bool no_critical_errors =
            !App_SharedErrorTable_HasAnyCriticalErrorSet(error_table);
        bool bms_positive_air_closed =
            App_CanRx_BMS_AIR_STATES_GetSignal_AIR_POSITIVE(can_rx_interface) ==
            CANMSGS_BMS_AIR_STATES_AIR_POSITIVE_CLOSED_CHOICE;
        bool bms_negative_air_closed =
            App_CanRx_BMS_AIR_STATES_GetSignal_AIR_NEGATIVE(can_rx_interface) ==
            CANMSGS_BMS_AIR_STATES_AIR_NEGATIVE_CLOSED_CHOICE;
        bool break_actuated =
            App_CanRx_FSM_BRAKE_GetSignal_BRAKE_IS_ACTUATED(can_rx_interface) ==
            CANMSGS_FSM_BRAKE_BRAKE_IS_ACTUATED_TRUE_CHOICE;
        bool left_inverter_ready =
            App_CanRx_INVL_INTERNAL_STATES_GetSignal_D1_VSM_STATE_INVL(
                can_rx_interface) ==
            CANMSGS_INVL_INTERNAL_STATES_D1_VSM_STATE_INVL_MOTOR__RUNNING__STATE_CHOICE;

        bool current_start_switch_position =
            App_CanRx_DIM_SWITCHES_GetSignal_START_SWITCH(can_rx_interface) ==
            CANMSGS_DIM_SWITCHES_START_SWITCH_ON_CHOICE;
        bool start_switch_pulled_up =
            current_start_switch_position && !prev_start_switch_position;

        if (no_critical_errors && bms_positive_air_closed &&
            bms_negative_air_closed && break_actuated && left_inverter_ready &&
            start_switch_pulled_up)
        {
            App_SharedStateMachine_SetNextState(
                state_machine, App_GetDriveState());
        }

        prev_start_switch_position = current_start_switch_position;
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
