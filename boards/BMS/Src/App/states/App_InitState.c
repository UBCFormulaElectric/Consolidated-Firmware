#include "states/App_InitState.h"
#include "states/App_DriveState.h"

#include "App_SetPeriodicCanSignals.h"
#include "App_SharedMacros.h"

static void InitStateRunOnEntry(struct StateMachine *const state_machine)
{
    struct BmsWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct BmsCanTxInterface *can_tx_interface = App_BmsWorld_GetCanTx(world);
    App_CanTx_SetPeriodicSignal_STATE(
        can_tx_interface, CANMSGS_BMS_STATE_MACHINE_STATE_INIT_CHOICE);
}

static void InitStateRunOnTick(struct StateMachine *const state_machine)
{
    struct BmsWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct BmsCanTxInterface *can_tx = App_BmsWorld_GetCanTx(world);
    struct Imd *              imd    = App_BmsWorld_GetImd(world);

    App_Imd_Tick(imd);
    App_SetPeriodicCanSignals_Imd(can_tx, imd);

    App_SharedStateMachine_SetNextState(state_machine, App_GetDriveState());
}

static void InitStateRunOnExit(struct StateMachine *const state_machine)
{
    UNUSED(state_machine);
}

const struct State *App_GetInitState(void)
{
    static struct State init_state = {
        .name             = "INIT",
        .run_on_entry     = InitStateRunOnEntry,
        .run_on_tick_1kHz = InitStateRunOnTick,
        .run_on_tick_1Hz  = NULL,
        .run_on_exit      = InitStateRunOnExit,
    };

    return &init_state;
}
