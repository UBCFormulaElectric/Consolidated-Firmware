#include "states/App_DriveState.h"

#include "App_SharedMacros.h"

static void DriveStateDriveOnEntry(struct StateMachine *state_machine)
{
    struct DcmWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct DcmCanTxInterface *can_tx_interface = App_DcmWorld_GetCanTx(world);
    App_CanTx_SetPeriodicSignal_STATE(
        can_tx_interface, CANMSGS_DCM_STATE_MACHINE_STATE_DRIVE_CHOICE);
}

static void DriveStateDriveOnTick(struct StateMachine *state_machine)
{
    UNUSED(state_machine);
}

static void DriveStateDriveOnExit(struct StateMachine *state_machine)
{
    UNUSED(state_machine);
}

const struct State *App_GetDriveState(void)
{
    static struct State drive_state = {
        .name         = "DRIVE",
        .run_on_entry = DriveStateDriveOnEntry,
        .run_on_tick  = DriveStateDriveOnTick,
        .run_on_exit  = DriveStateDriveOnExit,
    };

    return &drive_state;
}
