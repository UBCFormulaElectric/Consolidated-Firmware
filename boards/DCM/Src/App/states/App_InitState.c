#include "states/App_AllStates.h"
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

static void InitStateRunOnTick1Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick1Hz(state_machine);
}

static void InitStateRunOnTick100Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick100Hz(state_machine);

    struct DcmWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct DcmCanRxInterface *can_rx_interface = App_DcmWorld_GetCanRx(world);
    struct DcmCanTxInterface *can_tx_interface = App_DcmWorld_GetCanTx(world);
    struct ErrorTable *error_table = App_DcmWorld_GetErrorTable(world);

    App_CanTx_SetPeriodicSignal_TORQUE_REQUEST(can_tx_interface, 0.0f);

    bool any_critical_errors = App_SharedErrorTable_HasAnyCriticalErrorSet(error_table);
    bool bms_positive_air_closed = App_CanRx_BMS_AIR_STATES_GetSignal_AIR_POSITIVE(can_rx_interface) == 1;
    bool bms_negative_air_closed = App_CanRx_BMS_AIR_STATES_GetSignal_AIR_NEGATIVE(can_rx_interface) == 1;

    if (!any_critical_errors && bms_positive_air_closed && bms_negative_air_closed) {
        App_SharedStateMachine_SetNextState(state_machine, App_GetDriveState());
    }
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
