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
}

static void DriveStateRunOnTick1Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick1Hz(state_machine);
}

static void DriveStateRunOnTick100Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick100Hz(state_machine);

    struct DcmWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct DcmCanRxInterface *can_rx = App_DcmWorld_GetCanRx(world);

    if (App_CanRx_DIM_SWITCHES_GetSignal_START_SWITCH(can_rx) ==
        CANMSGS_DIM_SWITCHES_START_SWITCH_OFF_CHOICE)
    {
        App_SharedStateMachine_SetNextState(state_machine, App_GetInitState());
    }

    // Shut down motor if motor shutdown fault is received from other boards.
    if (App_CanRx_BMS_MOTOR_SHUTDOWN_ERRORS_GetSignal_DUMMY_MOTOR_SHUTDOWN(can_rx) ==
        CANMSGS_BMS_MOTOR_SHUTDOWN_ERRORS_DUMMY_MOTOR_SHUTDOWN_SHUTDOWN_CHOICE ||
        App_CanRx_PDM_MOTOR_SHUTDOWN_ERRORS_GetSignal_DUMMY_MOTOR_SHUTDOWN(can_rx) ==
        CANMSGS_PDM_MOTOR_SHUTDOWN_ERRORS_DUMMY_MOTOR_SHUTDOWN_SHUTDOWN_CHOICE ||
        App_CanRx_DIM_MOTOR_SHUTDOWN_ERRORS_GetSignal_DUMMY_MOTOR_SHUTDOWN(can_rx) ==
        CANMSGS_DIM_MOTOR_SHUTDOWN_ERRORS_DUMMY_MOTOR_SHUTDOWN_SHUTDOWN_CHOICE)
    {
        App_SharedStateMachine_SetNextState(state_machine, App_GetFaultState());
    }
    if (App_CanRx_FSM_MOTOR_SHUTDOWN_ERRORS_GetSignal_APPS_HAS_DISAGREEMENT (can_rx) ==
        CANMSGS_FSM_MOTOR_SHUTDOWN_ERRORS_APPS_HAS_DISAGREEMENT_TRUE_CHOICE ||
        App_CanRx_FSM_MOTOR_SHUTDOWN_ERRORS_GetSignal_PAPPS_ALARM_IS_ACTIVE (can_rx) ==
        CANMSGS_FSM_MOTOR_SHUTDOWN_ERRORS_PAPPS_ALARM_IS_ACTIVE_TRUE_CHOICE ||
        App_CanRx_FSM_MOTOR_SHUTDOWN_ERRORS_GetSignal_SAPPS_ALARM_IS_ACTIVE (can_rx) ==
        CANMSGS_FSM_MOTOR_SHUTDOWN_ERRORS_SAPPS_ALARM_IS_ACTIVE_TRUE_CHOICE ||
        App_CanRx_FSM_MOTOR_SHUTDOWN_ERRORS_GetSignal_PLAUSIBILITY_CHECK_HAS_FAILED (can_rx) ==
        CANMSGS_FSM_MOTOR_SHUTDOWN_ERRORS_PLAUSIBILITY_CHECK_HAS_FAILED_TRUE_CHOICE ||
        App_CanRx_FSM_MOTOR_SHUTDOWN_ERRORS_GetSignal_PRIMARY_FLOW_RATE_HAS_UNDERFLOW (can_rx) ==
        CANMSGS_FSM_MOTOR_SHUTDOWN_ERRORS_PRIMARY_FLOW_RATE_HAS_UNDERFLOW_TRUE_CHOICE ||
        App_CanRx_FSM_MOTOR_SHUTDOWN_ERRORS_GetSignal_SECONDARY_FLOW_RATE_HAS_UNDERFLOW (can_rx) ==
        CANMSGS_FSM_MOTOR_SHUTDOWN_ERRORS_SECONDARY_FLOW_RATE_HAS_UNDERFLOW_TRUE_CHOICE)
    {
        App_SharedStateMachine_SetNextState(state_machine, App_GetFaultState());
    }

    App_SetPeriodicCanSignals_Imu(world);
    App_SetPeriodicCanSignals_TorqueRequests(world);
}

static void DriveStateRunOnExit(struct StateMachine *const state_machine)
{
    UNUSED(state_machine);
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
