#include <stdlib.h>
#include <math.h>
#include "App_SharedMacros.h"
#include "states/App_AllStates.h"
#include "states/App_InitState.h"
#include "App_SetPeriodicCanSignals.h"
#include "App_SharedConstants.h"

#define EFFICIENCY_ESTIMATE (0.80f)
#define RPM_TO_RADS(rpm) ((rpm) * (float)M_PI / 30.0f)

// EV.5.7.1 The power to the Motor(s) must be immediately and completely shut down when both of the following exist at
// the same time: • The mechanical brakes are actuated • The APPS signals more than 25% pedal travel EV.5.7.2 The Motor
// shut down must remain active until the APPS signals less than 5% pedal travel, with or without brake operation.
#define BRAKE_PLAUSIBILITY_LATCH_SET_THRESH (25.0f)
#define BRAKE_PLAUSIBILITY_LATCH_RESET_THRESH (5.0f)

static void
    App_SetPeriodicCanSignals_TorqueRequests(struct DcmCanTxInterface *can_tx, struct DcmCanRxInterface *can_rx);

static void App_SetPeriodicCanSignals_TorqueRequests(struct DcmCanTxInterface *can_tx, struct DcmCanRxInterface *can_rx)
{
    // Set the latch high when the brake is pressed. Clear the latch when the brake is not actuated and the APPS returns
    // to a position that is lower than 5%
    static bool brake_actuated_latch = false;

    // Acquire the motor RPM from the FSM via CAN
    const float right_motor_speed_rad_s =
        RPM_TO_RADS((float)abs(App_CanRx_INVR_MOTOR_POSITION_INFO_GetSignal_D2_MOTOR_SPEED_INVR(can_rx)));
    const float left_motor_speed_rad_s =
        RPM_TO_RADS((float)abs(App_CanRx_INVL_MOTOR_POSITION_INFO_GetSignal_D2_MOTOR_SPEED_INVL(can_rx)));

    // Calculate the maximum torque request to draw the maximum power available from the BMS
    float calculated_torque_limit = MAX_TORQUE_REQUEST_NM;
    if ((right_motor_speed_rad_s + left_motor_speed_rad_s) > 0.0f)
    {
        calculated_torque_limit = App_CanRx_BMS_AVAILABLE_POWER_GetSignal_AVAILABLE_POWER(can_rx) *
                                  EFFICIENCY_ESTIMATE / (right_motor_speed_rad_s + left_motor_speed_rad_s);
    }

    const float apps_pct = App_CanRx_FSM_PEDAL_POSITION_GetSignal_MAPPED_PEDAL_PERCENTAGE(can_rx);
    if (App_CanRx_FSM_BRAKE_GetSignal_BRAKE_IS_ACTUATED(can_rx) && (apps_pct >= BRAKE_PLAUSIBILITY_LATCH_SET_THRESH))
    {
        // Set the brake actuated latch high only when the brake is actuated
        brake_actuated_latch = true;
    }
    else if (brake_actuated_latch && (apps_pct < BRAKE_PLAUSIBILITY_LATCH_RESET_THRESH))
    {
        brake_actuated_latch = false;
    }

    // Calculate the torque request to transmit to the inverter
    float torque_request = 0.0f;
    if (!brake_actuated_latch)
    {
        const float max_torque_request = min(calculated_torque_limit, MAX_TORQUE_REQUEST_NM);
        const float fsm_torque_limit   = App_CanRx_FSM_TORQUE_LIMITING_GetSignal_FSM_TORQUE_LIMIT(can_rx);
        torque_request                 = min(0.01f * apps_pct * max_torque_request, fsm_torque_limit);
    }

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
