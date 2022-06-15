#include "states/App_AllStates.h"
#include "states/App_FaultState.h"
#include "states/App_InitState.h"

#include "App_SharedMacros.h"

#define CLEAR_FAULT_CMD                                                                      \
    {                                                                                        \
        .d1_parameter_address_command = 20, .d2_read_write_command = 1, .d3_data_command = 0 \
    }

static void FaultStateRunOnEntry(struct StateMachine *const state_machine)
{
    struct DcmWorld *         world            = App_SharedStateMachine_GetWorld(state_machine);
    struct DcmCanTxInterface *can_tx_interface = App_DcmWorld_GetCanTx(world);
    App_CanTx_SetPeriodicSignal_STATE(can_tx_interface, CANMSGS_DCM_STATE_MACHINE_STATE_FAULT_CHOICE);

    // Disable inverters and apply zero torque upon entering fault state
    App_CanTx_SetPeriodicSignal_INVERTER_ENABLE_INVL(
        can_tx_interface, CANMSGS_DCM_INVL_COMMAND_MESSAGE_INVERTER_ENABLE_INVL_OFF_CHOICE);
    App_CanTx_SetPeriodicSignal_INVERTER_ENABLE_INVR(
        can_tx_interface, CANMSGS_DCM_INVR_COMMAND_MESSAGE_INVERTER_ENABLE_INVR_OFF_CHOICE);
    App_CanTx_SetPeriodicSignal_TORQUE_COMMAND_INVL(
        can_tx_interface, App_CanMsgs_dcm_invl_command_message_torque_command_invl_encode(0.0f));
    App_CanTx_SetPeriodicSignal_TORQUE_COMMAND_INVR(
        can_tx_interface, App_CanMsgs_dcm_invr_command_message_torque_command_invr_encode(0.0f));
}

static void FaultStateRunOnTick1Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick1Hz(state_machine);
}

static void FaultStateRunOnTick100Hz(struct StateMachine *const state_machine)
{
    struct DcmWorld *         world            = App_SharedStateMachine_GetWorld(state_machine);
    struct DcmCanTxInterface *can_tx = App_DcmWorld_GetCanTx(world);
    struct DcmCanRxInterface *can_rx = App_DcmWorld_GetCanRx(world);

    App_CanTx_SetPeriodicSignal_TORQUE_REQUEST(can_tx, 0.0f);

    if (App_HasInverterFault(can_rx) && App_IsBmsInDriveState(can_rx))
    {
        // Try clearing inverter fault
        const struct CanMsgs_debug_invl_read_write_param_command_t clear_fault_cmd_l = CLEAR_FAULT_CMD;
        const struct CanMsgs_debug_invr_read_write_param_command_t clear_fault_cmd_r = CLEAR_FAULT_CMD;

        App_CanTx_SendNonPeriodicMsg_DEBUG_INVL_READ_WRITE_PARAM_COMMAND(can_tx, &clear_fault_cmd_l);
        App_CanTx_SendNonPeriodicMsg_DEBUG_INVR_READ_WRITE_PARAM_COMMAND(can_tx, &clear_fault_cmd_r);
    }
    if (App_AllStatesRunOnTick100Hz(state_machine))
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
