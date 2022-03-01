#include "states/App_AllStates.h"
#include "states/App_DriveState.h"
#include "states/App_InitState.h"
#include "states/App_FaultState.h"
#include "App_SetPeriodicCanSignals.h"

#include "App_SharedMacros.h"

static inline bool
    HasInverterFaulted(const struct DcmCanRxInterface *can_rx_interface);
static inline bool
    IsStartSwitchOff(const struct DcmCanRxInterface *can_rx_interface);

static void DriveStateRunOnEntry(struct StateMachine *const state_machine)
{
    struct DcmWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct DcmCanTxInterface *can_tx_interface = App_DcmWorld_GetCanTx(world);

    struct Buzzer *buzzer = App_DcmWorld_GetBuzzer(world);
    App_Buzzer_TurnOn(buzzer);

    App_CanTx_SetPeriodicSignal_STATE(
        can_tx_interface, CANMSGS_DCM_STATE_MACHINE_STATE_DRIVE_CHOICE);
}

static void DriveStateRunOnTick1Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick1Hz(state_machine);
}

static void DriveStateRunOnTick100Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick100Hz(state_machine);

    struct DcmWorld *world = App_SharedStateMachine_GetWorld(state_machine);
    struct DcmCanRxInterface *can_rx      = App_DcmWorld_GetCanRx(world);
    struct ErrorTable *       error_table = App_DcmWorld_GetErrorTable(world);
    const struct State *      next_state  = App_GetDriveState();

    const bool has_inverter_faulted = HasInverterFaulted(can_rx);
    const bool is_critical_error_present =
        App_SharedErrorTable_HasAnyCriticalErrorSet(error_table);
    const bool is_start_switch_off = IsStartSwitchOff(can_rx);

    App_SetPeriodicCanSignals_Imu(world);
    App_SetPeriodicCanSignals_TorqueRequests(world);

    if (has_inverter_faulted || is_critical_error_present)
    {
        next_state = App_GetFaultState();
    }
    else if (is_start_switch_off)
    {
        next_state = App_GetInitState();
    }

    App_SharedStateMachine_SetNextState(state_machine, next_state);
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

static inline bool
    HasInverterFaulted(const struct DcmCanRxInterface *can_rx_interface)
{
    return App_CanRx_INVL_INTERNAL_STATES_GetSignal_D1_VSM_STATE_INVL(
               can_rx_interface) ==
               CANMSGS_INVL_INTERNAL_STATES_D1_VSM_STATE_INVL_BLINK_FAULT_CODE_STATE_CHOICE ||
           App_CanRx_INVR_INTERNAL_STATES_GetSignal_D1_VSM_STATE_INVR(
               can_rx_interface) ==
               CANMSGS_INVR_INTERNAL_STATES_D1_VSM_STATE_INVR_BLINK_FAULT_CODE_STATE_CHOICE;
}

static inline bool
    IsStartSwitchOff(const struct DcmCanRxInterface *can_rx_interface)
{
    return App_CanRx_DIM_SWITCHES_GetSignal_START_SWITCH(can_rx_interface) ==
           CANMSGS_DIM_SWITCHES_START_SWITCH_OFF_CHOICE;
}