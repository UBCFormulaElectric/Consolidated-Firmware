#include <stdlib.h>
#include <math.h>
#include "App_SharedMacros.h"
#include "states/App_AllStates.h"
#include "states/App_InitState.h"
#include "App_SetPeriodicCanSignals.h"
#include "App_SharedConstants.h"

#define EFFICIENCY_ESTIMATE (0.80f)
#define RPM_TO_RADS(rpm) ((rpm) * (float)M_PI / 30.0f)

void App_SetPeriodicCanSignals_TorqueRequests()
{
    const float bms_available_power =
        0; // TODO: JSONCAN -> App_CanRx_BMS_AVAILABLE_POWER_GetSignal_AVAILABLE_POWER(can_rx);
    const float right_motor_speed_rpm = 0;
    // TODO: JSONCAN -> (float)abs(App_CanRx_INVR_MOTOR_POSITION_INFO_GetSignal_D2_MOTOR_SPEED_INVR(can_rx));
    const float left_motor_speed_rpm = 0;
    // TODO: JSONCAN -> (float)abs(App_CanRx_INVL_MOTOR_POSITION_INFO_GetSignal_D2_MOTOR_SPEED_INVL(can_rx));
    float bms_torque_limit = MAX_TORQUE_REQUEST_NM;
    float fsm_torque_limit = 0; // TODO: JSONCAN -> App_CanRx_FSM_TORQUE_LIMITING_GetSignal_FSM_TORQUE_LIMIT(can_rx);

    if ((right_motor_speed_rpm + left_motor_speed_rpm) > 0.0f)
    {
        // Estimate the maximum torque request to draw the maximum power available from the BMS
        bms_torque_limit = bms_available_power * EFFICIENCY_ESTIMATE /
                           (RPM_TO_RADS(right_motor_speed_rpm) + RPM_TO_RADS(left_motor_speed_rpm));
    }

    // Calculate the maximum torque request to scale pedal percentage off of
    const float max_torque_request = MIN(bms_torque_limit, MAX_TORQUE_REQUEST_NM);

    // Calculate the actual torque request to transmit
    const float torque_request = 0;
    // TODO: JSONCAN
    // const float torque_request =
    // MIN(0.01f * App_CanRx_FSM_PEDAL_POSITION_GetSignal_MAPPED_PEDAL_PERCENTAGE(can_rx) * max_torque_request,
    //     fsm_torque_limit);

    // Transmit torque command to both inverters
    // JSONCAN -> App_CanTx_SetPeriodicSignal_TORQUE_COMMAND_INVL(
    // can_tx, App_CanMsgs_dcm_invl_command_message_torque_command_invl_encode(torque_request));
    // JSONCAN -> App_CanTx_SetPeriodicSignal_TORQUE_COMMAND_INVR(
    // can_tx, App_CanMsgs_dcm_invr_command_message_torque_command_invr_encode(torque_request));
}

static void DriveStateRunOnEntry(struct StateMachine *const state_machine)
{
    struct DcmWorld *world = App_SharedStateMachine_GetWorld(state_machine);

    struct Buzzer *buzzer = App_DcmWorld_GetBuzzer(world);
    App_Buzzer_TurnOn(buzzer);

    // JSONCAN -> App_CanTx_SetPeriodicSignal_STATE(can_tx_interface, CANMSGS_DCM_STATE_MACHINE_STATE_DRIVE_CHOICE);

    // Enable inverters upon entering drive state.
    // JSONCAN -> App_CanTx_SetPeriodicSignal_INVERTER_ENABLE_INVL(
    // can_tx_interface, CANMSGS_DCM_INVL_COMMAND_MESSAGE_INVERTER_ENABLE_INVL_ON_CHOICE);
    // JSONCAN -> App_CanTx_SetPeriodicSignal_INVERTER_ENABLE_INVR(
    // can_tx_interface, CANMSGS_DCM_INVR_COMMAND_MESSAGE_INVERTER_ENABLE_INVR_ON_CHOICE);
}

static void DriveStateRunOnTick1Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick1Hz(state_machine);
}

static void DriveStateRunOnTick100Hz(struct StateMachine *const state_machine)
{
    if (App_AllStatesRunOnTick100Hz(state_machine))
    {
        struct DcmWorld *world = App_SharedStateMachine_GetWorld(state_machine);

        App_SetPeriodicCanSignals_TorqueRequests();

        if (false) // TODO: JSONCAN -> (!App_IsStartSwitchOn(can_rx))
        {
            App_SharedStateMachine_SetNextState(state_machine, App_GetInitState());
        }
    }
}

static void DriveStateRunOnExit(struct StateMachine *const state_machine)
{
    struct DcmWorld *world = App_SharedStateMachine_GetWorld(state_machine);

    // Disable inverters and apply zero torque upon exiting drive state
    // JSONCAN -> App_CanTx_SetPeriodicSignal_INVERTER_ENABLE_INVL(
    // can_tx_interface, CANMSGS_DCM_INVL_COMMAND_MESSAGE_INVERTER_ENABLE_INVL_OFF_CHOICE);
    // JSONCAN -> App_CanTx_SetPeriodicSignal_INVERTER_ENABLE_INVR(
    // can_tx_interface, CANMSGS_DCM_INVR_COMMAND_MESSAGE_INVERTER_ENABLE_INVR_OFF_CHOICE);
    // JSONCAN -> App_CanTx_SetPeriodicSignal_TORQUE_COMMAND_INVL(
    // can_tx_interface, App_CanMsgs_dcm_invl_command_message_torque_command_invl_encode(0.0f));
    // JSONCAN -> App_CanTx_SetPeriodicSignal_TORQUE_COMMAND_INVR(
    // can_tx_interface, App_CanMsgs_dcm_invr_command_message_torque_command_invr_encode(0.0f));
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
