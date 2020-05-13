#include "states/App_AllStates.h"
#include "states/App_DriveState.h"

#include "App_SharedMacros.h"

static void DriveStateRunOnEntry(struct StateMachine *const state_machine)
{
    struct DcmWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct DcmCanTxInterface *can_tx_interface = App_DcmWorld_GetCanTx(world);
    App_CanTx_SetPeriodicSignal_STATE(
        can_tx_interface, CANMSGS_DCM_STATE_MACHINE_STATE_DRIVE_CHOICE);
}

static void DriveStateRunOnTick(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick(state_machine);
}

static void DriveStateRunOnExit(struct StateMachine *const state_machine)
{
    UNUSED(state_machine);
}

const struct State *App_GetDriveState(void)
{
    static struct State drive_state = {
        .name             = "DRIVE",
        .run_on_entry     = DriveStateRunOnEntry,
        .run_on_tick_1Hz  = NULL,
        .run_on_tick_1kHz = DriveStateRunOnTick,
        .run_on_exit      = DriveStateRunOnExit,
    };

    return &drive_state;
}
