#include "states/App_InitState.h"
#include "states/App_DriveState.h"

#include "App_SharedMacros.h"

static void InitStateRunOnEntry(struct StateMachine *const state_machine)
{
    struct DcmWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct DcmCanTxInterface *can_tx_interface = App_DcmWorld_GetCanTx(world);
    App_CanTx_SetPeriodicSignal_STATE(
        can_tx_interface, CANMSGS_DCM_STATE_MACHINE_STATE_INIT_CHOICE);
}

static void InitStateRunOnTick(struct StateMachine *const state_machine)
{
    struct DcmWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct DcmCanTxInterface *can_tx      = App_DcmWorld_GetCanTx(world);
    struct DcmCanRxInterface *can_rx      = App_DcmWorld_GetCanRx(world);
    struct BrakeLight *       brake_light = App_DcmWorld_GetBrakeLight(world);

    bool is_brake_actuated =
        App_CanRx_FSM_BRAKE_GetSignal_BRAKE_IS_ACTUATED(can_rx);
    bool is_regen_active =
        App_CanTx_GetPeriodicSignal_TORQUE_REQUEST(can_tx) < 0.0f;

    App_BrakeLight_SetLightStatus(
        brake_light, is_brake_actuated, is_regen_active);

    // No need for any safety checks, just run! (this is a demo)
    App_SharedStateMachine_SetNextState(state_machine, App_GetDriveState());
}

static void InitStateRunOnExit(struct StateMachine *const state_machine)
{
    UNUSED(state_machine);
}

const struct State *App_GetInitState(void)
{
    static struct State init_state = {
        .name         = "INIT",
        .run_on_entry = InitStateRunOnEntry,
        .run_on_tick  = InitStateRunOnTick,
        .run_on_exit  = InitStateRunOnExit,
    };

    return &init_state;
}
