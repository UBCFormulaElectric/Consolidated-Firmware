#include "states/App_SharedStates.h"
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
    App_SharedStatesRunOnTick1Hz(state_machine);
}

static void DriveStateRunOnTick100Hz(struct StateMachine *const state_machine)
{
    App_SharedStatesRunOnTick100Hz(state_machine);

    struct DcmWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct DcmCanRxInterface *can_rx      = App_DcmWorld_GetCanRx(world);
    struct ErrorTable *       error_table = App_DcmWorld_GetErrorTable(world);
    const struct State *      next_state  = App_GetDriveState();

    App_SetPeriodicCanSignals_Imu(world);
    App_SetPeriodicCanSignals_TorqueRequests(world);

    if (App_SharedStates_HasInverterFaulted(can_rx) ||
        App_SharedErrorTable_HasAnyCriticalErrorSet(error_table))
    {
        next_state = App_GetFaultState();
    }
    else if (App_SharedStates_IsStartSwitchOff(can_rx))
    {
        next_state = App_GetInitState();
    }

    App_SharedStateMachine_SetNextState(state_machine, next_state);
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
