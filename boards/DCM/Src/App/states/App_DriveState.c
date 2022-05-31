#include "states/App_AllStates.h"
#include "states/App_InitState.h"
#include "App_SetPeriodicCanSignals.h"

// TODO: Have separate maximum torque requests for motoring and generating
#define MAX_TORQUE_REQUEST_NM (90.0f)

void App_SetPeriodicCanSignals_TorqueRequests(struct DcmCanTxInterface *can_tx, struct DcmCanRxInterface *can_rx)
{
    float torque_request = 0.0f;

    //              Accelerator Pedal Percentage
    //  Torque =  -------------------------------  * MAX_TORQUE_REQUEST_NM
    //                        100%
    //
    torque_request =
        0.01f * App_CanRx_FSM_PEDAL_POSITION_GetSignal_MAPPED_PEDAL_PERCENTAGE(can_rx) * MAX_TORQUE_REQUEST_NM;

    // Transmit torque command to both inverters
    App_CanTx_SetPeriodicSignal_TORQUE_COMMAND_INVL(
        can_tx, App_CanMsgs_dcm_invl_command_message_torque_command_invl_encode(torque_request));
    App_CanTx_SetPeriodicSignal_TORQUE_COMMAND_INVR(
        can_tx, App_CanMsgs_dcm_invr_command_message_torque_command_invr_encode(torque_request));
}

static void DriveStateRunOnEntry(struct StateMachine *const state_machine)
{
    struct DcmWorld *         world            = App_SharedStateMachine_GetWorld(state_machine);
    struct DcmCanTxInterface *can_tx_interface = App_DcmWorld_GetCanTx(world);

    struct Buzzer *buzzer = App_DcmWorld_GetBuzzer(world);
    App_Buzzer_TurnOn(buzzer);

    App_CanTx_SetPeriodicSignal_STATE(can_tx_interface, CANMSGS_DCM_STATE_MACHINE_STATE_DRIVE_CHOICE);

    // Enable inverters upon entering drive state.
    App_CanTx_SetPeriodicSignal_INVERTER_ENABLE_INVL(
        can_tx_interface, CANMSGS_DCM_INVL_COMMAND_MESSAGE_INVERTER_ENABLE_INVL_ON_CHOICE);
    App_CanTx_SetPeriodicSignal_INVERTER_ENABLE_INVR(
        can_tx_interface, CANMSGS_DCM_INVR_COMMAND_MESSAGE_INVERTER_ENABLE_INVR_ON_CHOICE);
}

static void DriveStateRunOnTick1Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick1Hz(state_machine);
}

static void DriveStateRunOnTick100Hz(struct StateMachine *const state_machine)
{
    if (App_AllStatesRunOnTick100Hz(state_machine))
    {
        struct DcmWorld *         world  = App_SharedStateMachine_GetWorld(state_machine);
        struct DcmCanTxInterface *can_tx = App_DcmWorld_GetCanTx(world);
        struct DcmCanRxInterface *can_rx = App_DcmWorld_GetCanRx(world);

        App_SetPeriodicCanSignals_Imu(world);
        App_SetPeriodicCanSignals_TorqueRequests(can_tx, can_rx);

        if (!App_IsStartSwitchOn(can_rx))
        {
            App_SharedStateMachine_SetNextState(state_machine, App_GetInitState());
        }
    }
}

static void DriveStateRunOnExit(struct StateMachine *const state_machine)
{
    struct DcmWorld *         world            = App_SharedStateMachine_GetWorld(state_machine);
    struct DcmCanTxInterface *can_tx_interface = App_DcmWorld_GetCanTx(world);

    // Disable inverters and apply zero torque upon exiting drive state
    App_CanTx_SetPeriodicSignal_INVERTER_ENABLE_INVL(
        can_tx_interface, CANMSGS_DCM_INVL_COMMAND_MESSAGE_INVERTER_ENABLE_INVL_OFF_CHOICE);
    App_CanTx_SetPeriodicSignal_INVERTER_ENABLE_INVR(
        can_tx_interface, CANMSGS_DCM_INVR_COMMAND_MESSAGE_INVERTER_ENABLE_INVR_OFF_CHOICE);
    App_CanTx_SetPeriodicSignal_TORQUE_COMMAND_INVL(
        can_tx_interface, App_CanMsgs_dcm_invl_command_message_torque_command_invl_encode(0.0f));
    App_CanTx_SetPeriodicSignal_TORQUE_COMMAND_INVR(
        can_tx_interface, App_CanMsgs_dcm_invr_command_message_torque_command_invr_encode(0.0f));
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
