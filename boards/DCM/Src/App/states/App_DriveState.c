#include <stdlib.h>
#include <math.h>
#include "states/App_AllStates.h"
#include "states/App_InitState.h"
#include "App_SetPeriodicCanSignals.h"

#define MAX_TORQUE_REQUEST_NM (90.0f)

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define RPM_TO_RADS(rpm) ((rpm) * (float)M_PI / 30.0f)

struct EfficiencyLUTEntry
{
    float speed_rpm;
    float efficiency_estimate;
};

struct EfficiencyLUTEntry EfficiencyLUT[14] = {
    [0]  = { .speed_rpm = 500.0f, .efficiency_estimate = 80.0f },
    [1]  = { .speed_rpm = 1000.0f, .efficiency_estimate = 80.0f },
    [2]  = { .speed_rpm = 1500.0f, .efficiency_estimate = 80.0f },
    [3]  = { .speed_rpm = 2000.0f, .efficiency_estimate = 80.0f },
    [4]  = { .speed_rpm = 2500.0f, .efficiency_estimate = 80.0f },
    [5]  = { .speed_rpm = 3000.0f, .efficiency_estimate = 80.0f },
    [6]  = { .speed_rpm = 3500.0f, .efficiency_estimate = 80.0f },
    [7]  = { .speed_rpm = 4000.0f, .efficiency_estimate = 80.0f },
    [8]  = { .speed_rpm = 4500.0f, .efficiency_estimate = 80.0f },
    [9]  = { .speed_rpm = 5000.0f, .efficiency_estimate = 80.0f },
    [10] = { .speed_rpm = 5500.0f, .efficiency_estimate = 80.0f },
    [11] = { .speed_rpm = 6000.0f, .efficiency_estimate = 80.0f },
    [12] = { .speed_rpm = 6500.0f, .efficiency_estimate = 80.0f },
    [13] = { .speed_rpm = 7000.0f, .efficiency_estimate = 80.0f } // Maximum motor speed = 7000 rpm
};

void App_SetPeriodicCanSignals_TorqueRequests(struct DcmCanTxInterface *can_tx, struct DcmCanRxInterface *can_rx)
{
    uint8_t lut_idx;
    float   efficiency_estimate;
    float   max_torque_request, torque_request;
    float   bms_available_power = App_CanRx_BMS_AVAILABLE_POWER_GetSignal_AVAILABLE_POWER(can_rx);
    float right_motor_speed_rpm = (float)abs(App_CanRx_INVR_MOTOR_POSITION_INFO_GetSignal_D2_MOTOR_SPEED_INVR(can_rx));
    float left_motor_speed_rpm  = (float)App_CanRx_INVL_MOTOR_POSITION_INFO_GetSignal_D2_MOTOR_SPEED_INVL(can_rx);

    // Get the estimated efficiency for the current motor speed
    for (lut_idx = 0U; right_motor_speed_rpm < EfficiencyLUT[lut_idx].speed_rpm; lut_idx++)
        ;
    efficiency_estimate = EfficiencyLUT[lut_idx].efficiency_estimate;

    // Estimate the maximum torque request to draw the maximum power available from the BMS
    // Note that the motors can not exceed a torque of MAX_TORQUE_REQUEST_NM
    max_torque_request =
        MIN(bms_available_power * efficiency_estimate /
                (RPM_TO_RADS(right_motor_speed_rpm) + RPM_TO_RADS(left_motor_speed_rpm)),
            MAX_TORQUE_REQUEST_NM);

    // Calculate the actual torque request to transmit
    torque_request =
        MIN(0.01f * App_CanRx_FSM_PEDAL_POSITION_GetSignal_MAPPED_PEDAL_PERCENTAGE(can_rx) * MAX_TORQUE_REQUEST_NM,
            max_torque_request);

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
