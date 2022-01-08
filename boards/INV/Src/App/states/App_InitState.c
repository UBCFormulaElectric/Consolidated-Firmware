#include "states/App_InitState.h"

#include "App_SharedMacros.h"
#include "App_SharedExitCode.h"
#include "App_GateDrive.h"

static const struct StgapFaults *stgap_faults;

static void InitStateRunOnEntry(struct StateMachine *const state_machine)
{
    struct InvWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct InvCanTxInterface *can_tx_interface = App_InvWorld_GetCanTx(world);
    struct GateDrive *        gate_drive  = App_InvWorld_GetGateDrive(world);
    struct PowerStage *       power_stage = App_InvWorld_GetPowerStage(world);

    UNUSED(can_tx_interface);
    App_PowerStage_SetCurrentLimits(power_stage, 10);
    App_PowerStage_StandBy(
        power_stage); // Enable continuous ADC DMA requests from all channels
    App_GateDrive_WriteConfig(gate_drive);
    stgap_faults = App_GateDrive_GetFaults(gate_drive);
    //App_CanTx_SetPeriodicSignal_INVERTER_STATE(can_tx_interface, 1);
}

static void InitStateRunOnTick1Hz(struct StateMachine *const state_machine)
{
    struct InvWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    //    struct RgbLedSequence *rgb_led_sequence =
    //        App_InvWorld_GetRgbLedSequence(world);

    // App_SharedRgbLedSequence_Tick(rgb_led_sequence);
//    App_CanTx_SetPeriodicSignal_STATE(
//    can_tx_interface, CANMSGS_DIM_STATE_MACHINE_STATE_DRIVE_CHOICE);
}

static void InitStateRunOnTick100Hz(struct StateMachine *const state_machine)
{
    struct InvWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct InvCanTxInterface *can_tx_interface = App_InvWorld_GetCanTx(world);
    struct HeartbeatMonitor *heartbeat_monitor =
        App_InvWorld_GetHeartbeatMonitor(world);

    App_CanTx_SetPeriodicSignal_INVERTER_STATE(can_tx_interface, 1);

    App_SharedHeartbeatMonitor_Tick(heartbeat_monitor);

}

static void InitStateRunOnExit(struct StateMachine *const state_machine)
{
    UNUSED(state_machine);
}

const struct State *App_GetInitState(void)
{
    static struct State init_state = {
        .name              = "INIT",
        .run_on_entry      = InitStateRunOnEntry,
        .run_on_tick_1Hz   = InitStateRunOnTick1Hz,
        .run_on_tick_100Hz = InitStateRunOnTick100Hz,
        .run_on_exit       = InitStateRunOnExit,
    };

    return &init_state;
}

