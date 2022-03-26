#include "states/App_InitState.h"
#include "states/App_AllStates.h"
#include "App_SharedMacros.h"
#include "App_SharedExitCode.h"
#include "configs/App_ControlSystemConfig.h"
#include "App_GateDrive.h"
#include "states/App_DriveState.h"
#include "states/App_FaultState.h"
#include "states/App_StandbyState.h"
#include <math.h>
#include "App_Faults.h"

static const struct StgapFaults *stgap_faults;

static void InitStateRunOnEntry(struct StateMachine *const state_machine)
{
    struct InvWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct InvCanTxInterface *can_tx_interface = App_InvWorld_GetCanTx(world);
    struct GateDrive *        gate_drive  = App_InvWorld_GetGateDrive(world);
    struct PowerStage *       power_stage = App_InvWorld_GetPowerStage(world);

    App_GateDrive_WriteConfig(gate_drive);

    App_CanTx_SetPeriodicSignal_STATE(
        can_tx_interface, CANMSGS_INV_STATE_MACHINE_STATE_INIT_CHOICE);
    App_PowerStage_SetCurrentLimits(power_stage, MAX_INVERTER_CURRENT);

    App_PowerStage_Enable(power_stage); // Enable ADC

    App_GateDrive_StartPwmTimer(gate_drive);

    if (App_Faults_FaultedMotorShutdown(can_tx_interface))
    {
        App_SharedStateMachine_SetNextState(state_machine, App_GetFaultState());
        App_GateDrive_Shutdown(gate_drive);
    }
}

static void InitStateRunOnTick1Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick1Hz(state_machine);
    struct InvWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct InvCanTxInterface *can_tx_interface = App_InvWorld_GetCanTx(world);
}

static void InitStateRunOnTick100Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick100Hz(state_machine);
    struct InvWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct InvCanTxInterface *can_tx_interface = App_InvWorld_GetCanTx(world);
    struct InvCanRxInterface *can_rx_interface = App_InvWorld_GetCanRx(world);
    struct HeartbeatMonitor * heartbeat_monitor =
        App_InvWorld_GetHeartbeatMonitor(world);

    App_CanTx_SetPeriodicSignal_STATE(
        can_tx_interface, CANMSGS_INV_STATE_MACHINE_STATE_INIT_CHOICE);

    App_SharedHeartbeatMonitor_Tick(heartbeat_monitor);

    // Standby state is only feasible choice
    if (App_CanRx_INV_STATE_REQ_GetSignal_STATE_REQ(can_rx_interface) !=
            CANMSGS_INV_STATE_MACHINE_STATE_INIT_CHOICE &&
        App_PowerStage_PhaseCurOffsetComplete())
    {
        App_SharedStateMachine_SetNextState(
            state_machine, App_GetStandbyState());
    }
}

static void InitStateRunOnExit(struct StateMachine *const state_machine)
{
    // TODO check for faults that could have occured during initialization here,
    // for example, ADC/periph faults - are adc/dac numbers plausible, for
    // example?
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
