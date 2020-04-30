#include "states/App_DriveState.h"

#include "App_SharedMacros.h"
#include "App_SevenSegDisplays.h"

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
    struct DimCanTxInterface *can_tx = App_DimWorld_GetCanTx(world);
    struct DimCanRxInterface *can_rx = App_DimWorld_GetCanRx(world);
    struct SevenSegDisplays * seven_seg_displays =
        App_DimWorld_GetSevenSegDisplays(world);
    struct HeartbeatMonitor *heartbeat_monitor =
        App_DimWorld_GetHeartbeatMonitor(world);
    struct RegenPaddle * regen_paddle = App_DimWorld_GetRegenPaddle(world);
    struct BinarySwitch *start_switch = App_DimWorld_GetStartSwitch(world);
    struct BinarySwitch *traction_control_switch =
        App_DimWorld_GetTractionControlSwitch(world);
    struct BinarySwitch *torque_vectoring_switch =
        App_DimWorld_GetTorqueVectoringSwitch(world);

    uint32_t buffer;

    if (EXIT_CODE_OK(
            App_RegenPaddle_GetRawPaddlePosition(regen_paddle, &buffer)))
    {
        App_CanTx_SetPeriodicSignal_RAW_PADDLE_POSITION(can_tx, buffer);
    }

    if (EXIT_CODE_OK(
            App_RegenPaddle_GetMappedPaddlePosition(regen_paddle, &buffer)))
    {
        App_CanTx_SetPeriodicSignal_MAPPED_PADDLE_POSITION(can_tx, buffer);
    }

    App_SevenSegDisplays_SetUnsignedBase10Value(
        seven_seg_displays,
        App_CanRx_BMS_STATE_OF_CHARGE_GetSignal_STATE_OF_CHARGE(can_rx));

    if (App_BinarySwitch_IsTurnedOn(start_switch))
    {
        App_CanTx_SetPeriodicSignal_START_SWITCH_IS_ON(can_tx, true);
    }
    else
    {
        App_CanTx_SetPeriodicSignal_START_SWITCH_IS_ON(can_tx, false);
    }

    if (App_BinarySwitch_IsTurnedOn(traction_control_switch))
    {
        App_CanTx_SetPeriodicSignal_TRACTION_CONTROL_SWITCH_IS_ON(can_tx, true);
    }
    else
    {
        App_CanTx_SetPeriodicSignal_TRACTION_CONTROL_SWITCH_IS_ON(
            can_tx, false);
    }

    if (App_BinarySwitch_IsTurnedOn(torque_vectoring_switch))
    {
        App_CanTx_SetPeriodicSignal_TORQUE_VECTORING_SWITCH_IS_ON(can_tx, true);
    }
    else
    {
        App_CanTx_SetPeriodicSignal_TORQUE_VECTORING_SWITCH_IS_ON(
            can_tx, false);
    }

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
