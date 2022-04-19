#include "states/App_SharedStates.h"
#include "states/App_FaultState.h"
#include "states/App_InitState.h"

#include "App_SharedMacros.h"

#define CLEAR_INV_FAULT_PARAM_ADDRESS (20U)
#define WRITE_CMD (1U)
#define CLEAR_INV_FAULT_MSG                                            \
    {                                                                  \
        .d1_parameter_address_command = CLEAR_INV_FAULT_PARAM_ADDRESS, \
        .d2_read_write_command = WRITE_CMD, .d3_data_command = 0U      \
    }

static void FaultStateRunOnEntry(struct StateMachine *const state_machine)
{
    struct DcmWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct DcmCanTxInterface *can_tx_interface = App_DcmWorld_GetCanTx(world);
    App_CanTx_SetPeriodicSignal_STATE(
        can_tx_interface, CANMSGS_DCM_STATE_MACHINE_STATE_FAULT_CHOICE);

    // Disable inverters and apply zero torque upon entering fault state
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

static void FaultStateRunOnTick1Hz(struct StateMachine *const state_machine)
{
    struct DcmWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct DcmCanRxInterface *can_rx = App_DcmWorld_GetCanRx(world);
    struct DcmCanTxInterface *can_tx = App_DcmWorld_GetCanTx(world);

    // Clear left/right inverter fault if requested by a PCAN node
    if (App_CanRx_CLEAR_INV_FAULT_CMD_GetSignal_CLEAR_LEFT_FAULT(can_rx))
    {
        // Write a 0 to the fault clear parameter to clear any active faults
        const struct CanMsgs_dcm_invl_read_write_param_command_t invl_cmd_msg =
            CLEAR_INV_FAULT_MSG;

        // Send the clear fault command to the left inverter once
        App_CanTx_SendNonPeriodicMsg_DCM_INVL_READ_WRITE_PARAM_COMMAND(
            can_tx, &invl_cmd_msg);

        App_CanRx_CLEAR_INV_FAULT_CMD_SetSignal_CLEAR_LEFT_FAULT(can_rx, false);
    }
    if (App_CanRx_CLEAR_INV_FAULT_CMD_GetSignal_CLEAR_RIGHT_FAULT(can_rx))
    {
        // Write a 0 to the fault clear parameter to clear any active faults
        const struct CanMsgs_dcm_invr_read_write_param_command_t invr_cmd_msg =
            CLEAR_INV_FAULT_MSG;

        // Send the clear fault command to the right inverter once
        App_CanTx_SendNonPeriodicMsg_DCM_INVR_READ_WRITE_PARAM_COMMAND(
            can_tx, &invr_cmd_msg);

        App_CanRx_CLEAR_INV_FAULT_CMD_SetSignal_CLEAR_RIGHT_FAULT(
            can_rx, false);
    }

    // Open or close the inverter LV switches if requested by a PCAN node
    App_SharedStates_ConfigInverterSwitches(
        can_rx, App_DcmWorld_GetInverterSwitches(world));

    App_SharedStatesRunOnTick1Hz(state_machine);
}

static void FaultStateRunOnTick100Hz(struct StateMachine *const state_machine)
{
    App_SharedStatesRunOnTick100Hz(state_machine);

    struct DcmWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct DcmCanTxInterface *can_tx_interface = App_DcmWorld_GetCanTx(world);
    struct DcmCanRxInterface *can_rx_interface = App_DcmWorld_GetCanRx(world);
    struct ErrorTable *       error_table = App_DcmWorld_GetErrorTable(world);

    App_CanTx_SetPeriodicSignal_TORQUE_REQUEST(can_tx_interface, 0.0f);

    if (!App_SharedStates_HasInverterFaulted(can_rx_interface) &&
        !App_SharedErrorTable_HasAnyCriticalErrorSet(error_table))
    {
        App_SharedStateMachine_SetNextState(state_machine, App_GetInitState());
    }
}

static void FaultStateRunOnExit(struct StateMachine *const state_machine)
{
    UNUSED(state_machine);
}

const struct State *App_GetFaultState(void)
{
    static struct State fault_state = {
        .name              = "FAULT",
        .run_on_entry      = FaultStateRunOnEntry,
        .run_on_tick_1Hz   = FaultStateRunOnTick1Hz,
        .run_on_tick_100Hz = FaultStateRunOnTick100Hz,
        .run_on_exit       = FaultStateRunOnExit,
    };

    return &fault_state;
}
