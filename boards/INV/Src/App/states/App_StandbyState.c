#include "states/App_StandbyState.h"
#include "states/App_AllStates.h"
#include "states/App_FaultState.h"
#include "states/App_DriveState.h"
#include "App_Faults.h"
#include "App_GateDrive.h"

static void StandbyStateRunOnEntry(struct StateMachine *const state_machine)
{
    struct InvWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct InvCanTxInterface *can_tx_interface = App_InvWorld_GetCanTx(world);
    struct InvCanRxInterface *can_rx_interface = App_InvWorld_GetCanRx(world);
    struct GateDrive *        gate_drive = App_InvWorld_GetGateDrive(world);

    if (App_Faults_FaultedMotorShutdown(can_tx_interface, can_rx_interface))
    {
        App_SharedStateMachine_SetNextState(state_machine, App_GetFaultState());
        App_GateDrive_Shutdown(gate_drive);
    }
    App_CanTx_SetPeriodicSignal_STATE(
        can_tx_interface, CANMSGS_INV_STATE_STATE_STANDBY_CHOICE);
}

static void StandbyStateRunOnTick1Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick1Hz(state_machine);
    struct InvWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct InvCanTxInterface *can_tx_interface = App_InvWorld_GetCanTx(world);
}

static void StandbyStateRunOnTick100Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick100Hz(state_machine);
    struct InvWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct InvCanTxInterface *can_tx_interface = App_InvWorld_GetCanTx(world);
    struct InvCanRxInterface *can_rx_interface = App_InvWorld_GetCanRx(world);
    struct HeartbeatMonitor * heartbeat_monitor =
        App_InvWorld_GetHeartbeatMonitor(world);

    App_CanTx_SetPeriodicSignal_STATE(
        can_tx_interface, CANMSGS_INV_STATE_STATE_STANDBY_CHOICE);
    App_SharedHeartbeatMonitor_Tick(heartbeat_monitor);

    // If drive state is commanded, go to the drive state
    if (App_CanRx_INV_STATE_REQ_GetSignal_STATE_MACHINE_REQ(can_rx_interface) ==
        CANMSGS_INV_STATE_STATE_DRIVE_CHOICE)
    {
        App_SharedStateMachine_SetNextState(state_machine, App_GetDriveState());
    }
}

static void StandbyStateRunOnExit(struct StateMachine *const state_machine)
{
    UNUSED(state_machine);
}

const struct State *App_GetStandbyState(void)
{
    static struct State standby_state = {
        .name              = "STANDBY",
        .run_on_entry      = StandbyStateRunOnEntry,
        .run_on_tick_1Hz   = StandbyStateRunOnTick1Hz,
        .run_on_tick_100Hz = StandbyStateRunOnTick100Hz,
        .run_on_exit       = StandbyStateRunOnExit,
    };

    return &standby_state;
}
