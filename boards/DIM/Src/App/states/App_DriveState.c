#include "states/App_DriveState.h"

#include "App_SharedMacros.h"

static void DriveStateRunOnEntry(struct StateMachine *const state_machine)
{
    struct DimWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct DimCanTxInterface *can_tx_interface = App_DimWorld_GetCanTx(world);
    App_CanTx_SetPeriodicSignal_STATE(
        can_tx_interface, CANMSGS_DIM_STATE_MACHINE_STATE_DRIVE_CHOICE);
}

static void DriveStateRunOnTick(struct StateMachine *const state_machine)
{
    struct DimWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct DimCanTxInterface *can_tx       = App_DimWorld_GetCanTx(world);
    struct RegenPaddle *      regen_paddle = App_DimWorld_GetRegenPaddle(world);

    uint32_t buffer;

    App_RegenPaddle_GetMappedPaddlePosition(regen_paddle, &buffer);

    App_CanTx_SetPeriodicSignal_RAW_PADDLE_POSITION(
        can_tx, buffer);

    App_CanTx_SetPeriodicSignal_MAPPED_PADDLE_POSITION(
        can_tx, App_RegenPaddle_GetMappedPaddlePosition(regen_paddle));

    struct HeartbeatMonitor *heartbeat_monitor =
        App_DimWorld_GetHeartbeatMonitor(world);
    App_SharedHeartbeatMonitor_Tick(heartbeat_monitor);
}

static void DriveStateRunOnExit(struct StateMachine *const state_machine)
{
    UNUSED(state_machine);
}

const struct State *App_GetDriveState(void)
{
    static struct State drive_state = {
        .name         = "DRIVE",
        .run_on_entry = DriveStateRunOnEntry,
        .run_on_tick  = DriveStateRunOnTick,
        .run_on_exit  = DriveStateRunOnExit,
    };

    return &drive_state;
}
