#include "states/App_AllStates.h"
#include "states/App_DriveState.h"
#include "states/App_DriveState.h"
#include "states/App_FaultState.h"
#include "states/App_StandbyState.h"
#include "configs/App_ControlSystemConfig.h"
#include "main.h"
#include "App_SharedMacros.h"
#include "App_SharedExitCode.h"
#include "Io_STGAP1AS.h"
#include "App_Faults.h"

static struct StgapFaults *stgap_faults;

static void DriveStateRunOnEntry(struct StateMachine *const state_machine)
{
    struct InvWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct InvCanTxInterface *can_tx_interface = App_InvWorld_GetCanTx(world);
    // struct InvCanRxInterface *can_rx_interface =
    // App_InvWorld_GetCanRx(world);
    struct GateDrive * gate_drive  = App_InvWorld_GetGateDrive(world);
    struct PowerStage *power_stage = App_InvWorld_GetPowerStage(world);

    if (App_Faults_FaultedMotorShutdown(can_tx_interface))
    {
        App_SharedStateMachine_SetNextState(state_machine, App_GetFaultState());
        App_GateDrive_Shutdown(gate_drive);
    }
    else
    {
        // Configure powerstage
        App_PowerStage_Enable(power_stage); // Set adc to PWM sync mode
        if (fabsf(App_PowerStage_GetPosCurrentLimit() - MAX_INVERTER_CURRENT) <
                0.5f ||
            fabsf(App_PowerStage_GetNegCurrentLimit() + MAX_INVERTER_CURRENT) <
                0.5f)
        {
            App_CanTx_SetPeriodicSignal_CUR_SNS_OFFSET(can_tx_interface, 1);
        }

        App_GateDrive_SetSwitchingFreq(gate_drive, SWITCHING_FREQUENCY);
        App_GateDrive_SetDeadTime(gate_drive, DEAD_TIME);
        App_GateDrive_GlobalReset(gate_drive);
        App_GateDrive_StartPwmTimer(gate_drive); // Enable PWM
        App_GateDrive_Enable(
            gate_drive, can_tx_interface); // Release Shutdown Pin

        App_CanTx_SetPeriodicSignal_STATE(
            can_tx_interface, CANMSGS_INV_STATE_MACHINE_STATE_DRIVE_CHOICE);
    }
}

static void DriveStateRunOnTick1Hz(struct StateMachine *const state_machine)
{
    //App_AllStatesRunOnTick1Hz(state_machine);
}

static void DriveStateRunOnTick100Hz(struct StateMachine *const state_machine)
{
    //App_AllStatesRunOnTick100Hz(state_machine);
    struct InvWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct InvCanTxInterface *can_tx_interface = App_InvWorld_GetCanTx(world);
    struct InvCanRxInterface *can_rx_interface = App_InvWorld_GetCanRx(world);
    struct PowerStage *       power_stage = App_InvWorld_GetPowerStage(world);
    struct HeartbeatMonitor * heartbeat_monitor =
        App_InvWorld_GetHeartbeatMonitor(world);

    App_CanTx_SetPeriodicSignal_STATE(
        can_tx_interface, CANMSGS_INV_STATE_MACHINE_STATE_DRIVE_CHOICE);
    App_SharedHeartbeatMonitor_Tick(heartbeat_monitor);

    if (App_CanRx_INV_STATE_REQ_GetSignal_STATE_REQ(can_rx_interface) ==
        CANMSGS_INV_STATE_MACHINE_STATE_STANDBY_CHOICE)
    {
        //App_PowerStage_StandBy(power_stage);
        App_SharedStateMachine_SetNextState(
            state_machine, App_GetStandbyState());
    }
}

static void DriveStateRunOnExit(struct StateMachine *const state_machine)
{
    struct InvWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct InvCanTxInterface *can_tx_interface = App_InvWorld_GetCanTx(world);
    struct GateDrive *        gate_drive  = App_InvWorld_GetGateDrive(world);
    struct PowerStage *       power_stage = App_InvWorld_GetPowerStage(world);

    App_GateDrive_Shutdown(gate_drive);  // Set GDRV Shutdown Pin
    App_GateDrive_StopPwm(gate_drive);   // Disable PWM
    //App_PowerStage_StandBy(power_stage); // Set ADC to cont. mode
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
