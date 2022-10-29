#include "states/App_DriveState.h"

#include "states/App_FaultState.h"

#include "states/App_AllStates.h"
#include "App_SharedMacros.h"

void App_DriveStateRunOnEntry(struct StateMachine *const state_machine)
{
    return;
}

void App_DriveStateRunOnTick1Hz(struct StateMachine *state_machine)
{
    App_AllStatesRunOnTick1Hz(state_machine);
}
void App_DriveStateRunOnTick100Hz(struct StateMachine *state_machine)
{
    App_AllStatesRunOnTick100Hz(state_machine);

    struct FsmWorld *         world              = App_SharedStateMachine_GetWorld(state_machine);
    struct FsmCanTxInterface *can_tx             = App_FsmWorld_GetCanTx(world);
    struct AcceleratorPedals *accelerator_pedals = App_FsmWorld_GetPappsAndSapps(world);
    struct Brake *            brake              = App_FsmWorld_GetBrake(world);
    struct Coolant *          coolant            = App_FsmWorld_GetCoolant(world);
    struct Steering *         steering           = App_FsmWorld_GetSteering(world);
    struct Wheels *           wheels             = App_FsmWorld_GetWheels(world);

    // NEW ALL STATES CODE
    bool coolantTriggerShutdown = false;
    App_AcceleratorPedals_Broadcast(world);
    App_Brake_Broadcast(world);
    App_Coolant_Broadcast(world, &coolantTriggerShutdown);
    App_Steering_Broadcast(world);
    App_Wheels_Broadcast(world);

    // go to fault state
    if (coolantTriggerShutdown)
    {
        App_SharedStateMachine_SetNextState(state_machine, App_GetFaultState());
    }
}
void App_DriveStateRunOnExit(struct StateMachine *const state_machine)
{
    UNUSED(state_machine);
}

const struct State *App_GetDriveState(void)
{
    static struct State drive_state = {
        .name              = "DRIVE STATE",
        .run_on_entry      = App_DriveStateRunOnEntry,
        .run_on_tick_1Hz   = App_DriveStateRunOnTick1Hz,
        .run_on_tick_100Hz = App_DriveStateRunOnTick100Hz,
        .run_on_exit       = App_DriveStateRunOnExit,
    };
    return &drive_state;
}
