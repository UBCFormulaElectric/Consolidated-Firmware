#include "states/App_AllStates.h"
#include "states/App_DriveState.h"
#include "states/App_InitState.h"
#include "states/App_FaultState.h"
#include "App_SetPeriodicCanSignals.h"

#include "App_SharedMacros.h"

static void DriveStateRunOnEntry(struct StateMachine *const state_machine)
{
    struct DcmWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct DcmCanTxInterface *can_tx_interface = App_DcmWorld_GetCanTx(world);

    struct Buzzer *buzzer = App_DcmWorld_GetBuzzer(world);
    App_Buzzer_TurnOn(buzzer);

    App_CanTx_SetPeriodicSignal_STATE(
        can_tx_interface, CANMSGS_DCM_STATE_MACHINE_STATE_DRIVE_CHOICE);
    // Enable inverters upon entering drive state.
    App_CanTx_SetPeriodicSignal_INVERTER_ENABLE_INVL(
        can_tx_interface,
        CANMSGS_DCM_INVL_COMMAND_MESSAGE_INVERTER_ENABLE_INVL_ON_CHOICE);
    App_CanTx_SetPeriodicSignal_INVERTER_ENABLE_INVR(
        can_tx_interface,
        CANMSGS_DCM_INVR_COMMAND_MESSAGE_INVERTER_ENABLE_INVR_ON_CHOICE);
}

static void DriveStateRunOnTick1Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick1Hz(state_machine);
}

static void DriveStateRunOnTick100Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick100Hz(state_machine);

    struct DcmWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct DcmCanRxInterface *can_rx      = App_DcmWorld_GetCanRx(world);
    struct ErrorTable *       error_table = App_DcmWorld_GetErrorTable(world);

    const bool inverter_faulted =
        App_CanRx_INVL_INTERNAL_STATES_GetSignal_D1_VSM_STATE_INVL(can_rx) ==
            CANMSGS_INVL_INTERNAL_STATES_D1_VSM_STATE_INVL_BLINK_FAULT_CODE_STATE_CHOICE ||
        App_CanRx_INVR_INTERNAL_STATES_GetSignal_D1_VSM_STATE_INVR(can_rx) ==
            CANMSGS_INVR_INTERNAL_STATES_D1_VSM_STATE_INVR_BLINK_FAULT_CODE_STATE_CHOICE;

#ifndef DEBUG // Go back to init state if start switch turned off
    if (App_CanRx_DIM_SWITCHES_GetSignal_START_SWITCH(can_rx) ==
        CANMSGS_DIM_SWITCHES_START_SWITCH_OFF_CHOICE)
    {
        App_SharedStateMachine_SetNextState(state_machine, App_GetInitState());
    }
#endif
    // Enter fault state if an inverter has faulted or a motor shutdown error
    // has been set
    if (inverter_faulted ||
            App_SharedErrorTable_HasAnyCriticalErrorSet(error_table))
    {
        App_SharedStateMachine_SetNextState(state_machine, App_GetFaultState());
    }

    App_SetPeriodicCanSignals_Imu(world);
    App_SetPeriodicCanSignals_TorqueRequests(world);
}

static void DriveStateRunOnExit(struct StateMachine *const state_machine)
{
    struct DcmWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct DcmCanTxInterface *can_tx_interface = App_DcmWorld_GetCanTx(world);

    // Disable inverters and apply zero torque upon exiting drive state
    App_CanTx_SetPeriodicSignal_INVERTER_ENABLE_INVL(
        can_tx_interface,
        CANMSGS_DCM_INVL_COMMAND_MESSAGE_INVERTER_ENABLE_INVL_OFF_CHOICE);
    App_CanTx_SetPeriodicSignal_INVERTER_ENABLE_INVR(
        can_tx_interface,
        CANMSGS_DCM_INVR_COMMAND_MESSAGE_INVERTER_ENABLE_INVR_OFF_CHOICE);
    App_CanTx_SetPeriodicSignal_TORQUE_COMMAND_INVL(
        can_tx_interface,
        App_CanMsgs_dcm_invl_command_message_torque_command_invl_encode(0.0f));
    App_CanTx_SetPeriodicSignal_TORQUE_COMMAND_INVR(
        can_tx_interface,
        App_CanMsgs_dcm_invr_command_message_torque_command_invr_encode(0.0f));
}

const struct State *App_GetDriveState(void)
{
    static struct State drive_state = {
        .name              = "DRIVE",
        .run_on_entry      = DriveStateRunOnEntry,
        .run_on_tick_1Hz   = DriveStateRunOnTick1Hz,
        .run_on_tick_100Hz = DriveStateRunOnTick100Hz,
        .run_on_exit       = DriveStateRunOnExit,
    };

    return &drive_state;
}
