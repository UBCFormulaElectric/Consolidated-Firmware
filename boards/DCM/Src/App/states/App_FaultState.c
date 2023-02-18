#include "states/App_AllStates.h"
#include "states/App_FaultState.h"
#include "states/App_InitState.h"

#include "App_SharedMacros.h"

static void FaultStateRunOnEntry(struct StateMachine *const state_machine)
{
    struct DcmWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    // TODO check -> App_CanTx_SetPeriodicSignal_STATE(can_tx_interface, CANMSGS_DCM_STATE_MACHINE_STATE_FAULT_CHOICE);
    App_CanTx_DCM_Vitals_CurrentState_Set(DCM_FAULT_STATE);

    // Disable inverters and apply zero torque upon entering fault state
    // TODO check -> App_CanTx_SetPeriodicSignal_INVERTER_ENABLE_INVL(
    // can_tx_interface, CANMSGS_DCM_INVL_COMMAND_MESSAGE_INVERTER_ENABLE_INVL_OFF_CHOICE);
    App_CanTx_DCM_LeftInverterCommand_EnableInverter_Set(false);
    // //     // TODO check -> App_CanTx_SetPeriodicSignal_INVERTER_ENABLE_INVR(
    // // //     // can_tx_interface, CANMSGS_DCM_INVR_COMMAND_MESSAGE_INVERTER_ENABLE_INVR_OFF_CHOICE);
        App_CanTx_DCM_RightInverterCommand_EnableInverter_Set(false);
    // //     // TODO check -> App_CanTx_SetPeriodicSignal_TORQUE_COMMAND_INVL(
    // //     // can_tx_interface, App_CanMsgs_dcm_invl_command_message_torque_command_invl_encode(0.0f));
        App_CanTx_DCM_LeftInverterCommand_TorqueCommand_Set(0.0f);
    // //     // TODO check -> App_CanTx_SetPeriodicSignal_TORQUE_COMMAND_INVR(
    // //     // can_tx_interface, App_CanMsgs_dcm_invr_command_message_torque_command_invr_encode(0.0f));
        App_CanTx_DCM_RightInverterCommand_TorqueCommand_Set(0.0f);
}

static void FaultStateRunOnTick1Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick1Hz(state_machine);
}

static void FaultStateRunOnTick100Hz(struct StateMachine *const state_machine)
{
    struct DcmWorld *world = App_SharedStateMachine_GetWorld(state_machine);

    // JSONCAN add to tx json-> App_CanTx_SetPeriodicSignal_TORQUE_REQUEST(can_tx_interface, 0.0f);

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
