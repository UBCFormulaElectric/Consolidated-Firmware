#include "states/App_DriveState.h"

#include "App_SharedMacros.h"
#include "App_SevenSegDisplays.h"
#include "App_SocDigits.h"

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
    struct DimCanRxInterface *can_rx = App_DimWorld_GetCanRx(world);
    struct SevenSegDisplays * seven_seg_displays =
        App_DimWorld_GetSevenSegDisplays(world);
    struct SocDigits *soc_digits = App_DimWorld_GetSocDigits(world);

    App_SocDigits_Tick(
        soc_digits,
        App_CanRx_BMS_STATE_OF_CHARGE_GetSignal_STATE_OF_CHARGE(can_rx));
    App_SevenSegDisplays_SetHexDigits(
        seven_seg_displays, App_SocDigits_GetDigits(soc_digits),
        App_SocDigits_GetNumDigits(soc_digits));

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
