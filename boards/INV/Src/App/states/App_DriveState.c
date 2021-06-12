#include "states/App_DriveState.h"
#include "main.h"
#include "App_SharedMacros.h"
#include "App_SharedExitCode.h"
#include "App_GateDrive.h"

static void DriveStateRunOnEntry(struct StateMachine *const state_machine)
{
    struct InvWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct InvCanTxInterface *can_tx_interface = App_InvWorld_GetCanTx(world);
    struct GateDrive *        gate_drive  = App_InvWorld_GetGateDrive(world);
    struct PowerStage *       power_stage = App_InvWorld_GetPowerStage(world);

    App_PowerStage_Enable(power_stage); // Enable ADC
    // App_GateDrive_StartPwm(gate_drive); // Enable PWM
    App_GateDrive_Enable(gate_drive); // Release Shutdown Pin

    UNUSED(can_tx_interface);

    // TODO: do stuff here. For example, you could set some CAN messages
}

static void DriveStateRunOnTick1Hz(struct StateMachine *const state_machine)
{
    struct InvWorld *  world = App_SharedStateMachine_GetWorld(state_machine);
    struct GateDrive * gate_drive  = App_InvWorld_GetGateDrive(world);
    struct PowerStage *power_stage = App_InvWorld_GetPowerStage(world);
    //        struct RgbLedSequence *rgb_led_sequence =
    //            App_InvWorld_GetRgbLedSequence(world);

    //    App_PowerStage_Enable(power_stage); //Enable ADC
    //    App_GateDrive_StartPwm(gate_drive); // Enable PWM
    //    App_GateDrive_Enable(gate_drive); // Release Shutdown Pin

    //    App_SharedRgbLedSequence_Tick(rgb_led_sequence);
    HAL_GPIO_TogglePin(GPIOD_1_GPIO_Port, GPIOD_1_Pin);
    // TODO: do stuff here. For example, you could set some CAN messages
    // App_CanTx_SetPeriodicSignal_STATE(
    //    can_tx_interface, CANMSGS_DIM_STATE_MACHINE_STATE_DRIVE_CHOICE);
    UNUSED(state_machine);
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
