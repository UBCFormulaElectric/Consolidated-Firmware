#include "states/App_DriveState.h"

#include "App_SharedMacros.h"

static void DriveStateRunOnEntry(struct StateMachine *state_machine)
{
    struct FsmWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct FsmCanTxInterface *can_tx_interface = App_FsmWorld_GetCanTx(world);
    App_CanTx_SetPeriodicSignal_STATE(
        can_tx_interface, CANMSGS_DCM_STATE_MACHINE_STATE_DRIVE_CHOICE);
}

static void DriveStateRunOnTick(struct StateMachine *state_machine)
{
    UNUSED(state_machine);
}

static void DriveStateRunOnExit(struct StateMachine *state_machine)
{
    UNUSED(state_machine);
}

const struct State *App_GetDriveState()
{
    static struct State drive_state = {
        .name         = "DRIVE",
        .run_on_enter = DriveStateRunOnEntry,
        .run_on_tick  = DriveStateRunOnTick,
        .run_on_exit  = DriveStateRunOnExit,
    };

    return &drive_state;
}
