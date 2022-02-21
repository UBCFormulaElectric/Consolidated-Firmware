#include "states/App_AllStates.h"
#include "states/App_FaultState.h"
#include "states/App_InitState.h"

#include "App_SharedMacros.h"

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
    App_AllStatesRunOnTick1Hz(state_machine);
}

static void FaultStateRunOnTick100Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick100Hz(state_machine);

    struct DcmWorld *  world = App_SharedStateMachine_GetWorld(state_machine);
    struct ErrorTable *error_table   = App_DcmWorld_GetErrorTable(world);
    struct DcmCanRxInterface *can_rx = App_DcmWorld_GetCanRx(world);
    struct ErrorList          critical_errors;

    bool inverter_faulted =
        App_CanRx_INVL_INTERNAL_STATES_GetSignal_D1_VSM_STATE_INVL(can_rx) ==
            CANMSGS_INVL_INTERNAL_STATES_D1_VSM_STATE_INVL_BLINK_FAULT_CODE_STATE_CHOICE ||
        App_CanRx_INVR_INTERNAL_STATES_GetSignal_D1_VSM_STATE_INVR(can_rx) ==
            CANMSGS_INVR_INTERNAL_STATES_D1_VSM_STATE_INVR_BLINK_FAULT_CODE_STATE_CHOICE;
    App_SharedErrorTable_GetAllCriticalErrors(error_table, &critical_errors);

    // Re-enter init state only if critical errors and inverter faults have been
    // cleared
    if (critical_errors.num_errors == 0 && inverter_faulted == false)
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
