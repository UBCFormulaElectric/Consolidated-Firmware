#include "states/App_DriveState.h"
#include "main.h"
#include "App_SharedMacros.h"
#include "App_SharedExitCode.h"
#include "Io_STGAP1AS.h"

static struct StgapFaults *stgap_faults;

static void DriveStateRunOnEntry(struct StateMachine *const state_machine)
{
    struct InvWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct InvCanTxInterface *can_tx_interface = App_InvWorld_GetCanTx(world);
    struct GateDrive *        gate_drive  = App_InvWorld_GetGateDrive(world);
    struct PowerStage *       power_stage = App_InvWorld_GetPowerStage(world);

    App_GateDrive_SetSwitchingFreq(gate_drive, 10000);
    App_GateDrive_SetDeadTime(gate_drive, 1000);
    App_GateDrive_WriteConfig(gate_drive);
    App_GateDrive_GlobalReset(gate_drive);
    App_PowerStage_CorrectCurrentOffset(power_stage);
    App_PowerStage_Enable(power_stage); // Enable ADC & DAC
    App_PowerStage_SetCurrentLimits(power_stage, 10);
    App_GateDrive_StartPwm(gate_drive); // Enable PWM
    App_GateDrive_Enable(gate_drive);   // Release Shutdown Pin
    App_CanRx_DCM_TORQUE_REQUEST_SetSignal_TORQUE_REQUEST()
    UNUSED(can_tx_interface);
}

static void DriveStateRunOnTick1Hz(struct StateMachine *const state_machine)
{
    struct InvWorld *  world = App_SharedStateMachine_GetWorld(state_machine);
    struct GateDrive * gate_drive  = App_InvWorld_GetGateDrive(world);
    struct PowerStage *power_stage = App_InvWorld_GetPowerStage(world);

    // App_CanTx_SetPeriodicSignal_STATE(
    //    can_tx_interface, CANMSGS_DIM_STATE_MACHINE_STATE_DRIVE_CHOICE);
    UNUSED(state_machine);
    //App_GateDrive_GetFaults(gate_drive, stgap_faults);
}

static void DriveStateRunOnTick100Hz(struct StateMachine *const state_machine)
{
    struct InvWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    //     struct GateDrive *gate_drive = App_InvWorld_GetGateDrive(world);
    //
    //    struct HeartbeatMonitor *heartbeat_monitor =
    //        App_InvWorld_GetHeartbeatMonitor(world);
    HAL_GPIO_TogglePin(GPIOD_2_GPIO_Port, GPIOD_2_Pin);
    //    App_SharedHeartbeatMonitor_Tick(heartbeat_monitor);
    UNUSED(state_machine);

    // TODO: do some more stuff here
}

static void DriveStateRunOnExit(struct StateMachine *const state_machine)
{
    struct InvWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct InvCanTxInterface *can_tx_interface = App_InvWorld_GetCanTx(world);
    struct GateDrive *        gate_drive  = App_InvWorld_GetGateDrive(world);
    struct PowerStage *       power_stage = App_InvWorld_GetPowerStage(world);

    App_GateDrive_Shutdown(gate_drive);  // Set GDRV Shutdown Pin
    App_GateDrive_StopPwm(gate_drive);   // Disable PWM
    App_PowerStage_Disable(power_stage); // Disable ADC
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
