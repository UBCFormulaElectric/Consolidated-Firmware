#include "states/App_DriveState.h"
#include "main.h"
#include "App_SharedMacros.h"
#include "App_SharedExitCode.h"
#include "App_GateDrive.h"

static void DriveStateRunOnEntry(struct StateMachine *const state_machine)
{
    struct InvWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct InvCanTxInterface *can_tx_interface = App_InvWorld_GetCanTx(world);

    UNUSED(can_tx_interface);

    // TODO: do stuff here. For example, you could set some CAN messages
}

static void DriveStateRunOnTick1Hz(struct StateMachine *const state_machine)
{
    struct InvWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct RgbLedSequence *rgb_led_sequence =
        App_InvWorld_GetRgbLedSequence(world);

    App_SharedRgbLedSequence_Tick(rgb_led_sequence);

    // TODO: do stuff here. For example, you could set some CAN messages
    // App_CanTx_SetPeriodicSignal_STATE(
    //    can_tx_interface, CANMSGS_DIM_STATE_MACHINE_STATE_DRIVE_CHOICE);
}

static void DriveStateRunOnTick100Hz(struct StateMachine *const state_machine)
{
    struct InvWorld * world = App_SharedStateMachine_GetWorld(state_machine);
    //struct GateDrive *gate_drive = App_InvWorld_GetGateDrive(world);

    struct HeartbeatMonitor *heartbeat_monitor =
        App_InvWorld_GetHeartbeatMonitor(world);


    App_SharedHeartbeatMonitor_Tick(heartbeat_monitor);


    // TODO: do some more stuff here
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
