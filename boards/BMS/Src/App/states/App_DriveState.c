#include "states/App_DriveState.h"

#include "App_SetPeriodicCanSignals.h"
#include "App_SharedMacros.h"

static void DriveStateRunOnEntry(struct StateMachine *const state_machine)
{
    struct BmsWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct BmsCanTxInterface *can_tx_interface = App_BmsWorld_GetCanTx(world);
    App_CanTx_SetPeriodicSignal_STATE(
        can_tx_interface, CANMSGS_BMS_STATE_MACHINE_STATE_DRIVE_CHOICE);
}

static void DriveStateRunOnTick(struct StateMachine *const state_machine)
{
    struct BmsWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct BmsCanTxInterface *can_tx = App_BmsWorld_GetCanTx(world);
    struct Imd *              imd    = App_BmsWorld_GetImd(world);

    App_Imd_Tick(imd);
    App_SetPeriodicCanSignals_Imd(can_tx, imd);
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
        .run_on_tick_1kHz = DriveStateRunOnTick,
        .run_on_tick_1Hz  = NULL,
        .run_on_exit      = DriveStateRunOnExit,
    };

    return &drive_state;
}
