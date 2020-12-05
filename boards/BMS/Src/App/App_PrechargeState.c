#include "App_PrechargeState.h"
#include "App_PrechargeStateMachine.h"
#include "App_SharedMacros.h"

#define MAX_NAME_LENGTH 16U

struct PreChargeState
{
    char name[MAX_NAME_LENGTH];
    void (*run_on_entry)(struct BmsWorld *);
    void (*run_on_tick)(struct BmsWorld *);
};

static void App_PreChargeState_RunOnEntryInitState(struct BmsWorld *const world)
{
    struct BmsCanTxInterface *can_tx = App_BmsWorld_GetCanTx(world);
    App_CanTx_SetPeriodicSignal_PRECHARGE_STATE(
        can_tx, CANMSGS_BMS_PRECHARGE_STATES_PRECHARGE_STATE_INIT_CHOICE);
}

static void App_PreChargeState_RunOnTickInitState(struct BmsWorld *const world)
{
    UNUSED(world);
}

static void
    App_PreChargeState_RunOnEntryAirOpenState(struct BmsWorld *const world)
{
    struct BmsCanTxInterface *can_tx = App_BmsWorld_GetCanTx(world);
    App_CanTx_SetPeriodicSignal_PRECHARGE_STATE(
        can_tx, CANMSGS_BMS_PRECHARGE_STATES_PRECHARGE_STATE_AIR_CHOICE);
}

static void
    App_PreChargeState_RunOnTickAirOpenState(struct BmsWorld *const world)
{
    struct PreChargeSequence *pre_charge_sequence =
        App_BmsWorld_GetPreChargeSequence(world);
    struct BinaryStatus *air_negative = App_BmsWorld_GetAirNegative(world);

    if (App_SharedBinaryStatus_IsActive(air_negative))
    {
        App_PreChargeStateMachine_SetNextState(
            pre_charge_sequence, App_PreChargeState_GetPrechargingState());
    }
}

static void
    App_PreChargeState_RunOnEntryPrechargingState(struct BmsWorld *const world)
{
    struct BmsCanTxInterface *can_tx = App_BmsWorld_GetCanTx(world);
    struct PreChargeSequence *pre_charge_sequence =
        App_BmsWorld_GetPreChargeSequence(world);
    struct Clock *clock = App_BmsWorld_GetClock(world);

    App_CanTx_SetPeriodicSignal_PRECHARGE_STATE(
        can_tx,
        CANMSGS_BMS_PRECHARGE_STATES_PRECHARGE_STATE_PRECHARGING_CHOICE);
    App_PreChargeSequence_SetInitialPrevTimeMs(
        pre_charge_sequence,
        App_SharedClock_GetCurrentTimeInMilliseconds(clock));
}

static void
    App_PreChargeState_RunOnTickPrechargingState(struct BmsWorld *const world)
{
    struct BmsCanTxInterface *can_tx = App_BmsWorld_GetCanTx(world);
    struct PreChargeSequence *pre_charge_sequence =
        App_BmsWorld_GetPreChargeSequence(world);
    struct Clock *clock = App_BmsWorld_GetClock(world);

    if (App_CanTx_GetPeriodicSignal_PRECHARGING_CONDITION(can_tx) ==
        CANMSGS_BMS_AIR_SHUTDOWN_ERRORS_PRECHARGING_CONDITION_PRECHARGING_CHOICE)
    {
        enum PreChargeExitCode pre_charge_exit_code =
            App_PreChargeSequence_CheckPreChargeBusVoltage(
                pre_charge_sequence,
                App_SharedClock_GetCurrentTimeInMilliseconds(clock));

        if (pre_charge_exit_code != PRE_CHARGE_VOLTAGE_IN_RANGE)
        {
            App_PreChargeSequence_Disable(pre_charge_sequence);
        }
        else
        {
            App_PreChargeSequence_Enable(pre_charge_sequence);
        }

        if (pre_charge_exit_code == PRE_CHARGE_SUCCESS)
        {
            App_CanTx_SetPeriodicSignal_PRECHARGING_CONDITION(
                can_tx,
                CANMSGS_BMS_AIR_SHUTDOWN_ERRORS_PRECHARGING_CONDITION_SUCCESS_CHOICE);
        }
        else if (
            pre_charge_exit_code == PRE_CHARGE_OVERVOLTAGE_ERROR ||
            pre_charge_exit_code == PRE_CHARGE_UNDERVOLTAGE_ERROR)
        {
            App_CanTx_SetPeriodicSignal_PRECHARGING_CONDITION(
                can_tx,
                CANMSGS_BMS_AIR_SHUTDOWN_ERRORS_PRECHARGING_CONDITION_FAIL_CHOICE);
        }
    }
}

struct PreChargeState *App_PreChargeState_GetInitState(void)
{
    static struct PreChargeState state = {
        .name         = "INIT",
        .run_on_entry = App_PreChargeState_RunOnEntryInitState,
        .run_on_tick  = App_PreChargeState_RunOnTickInitState
    };

    return &state;
}

struct PreChargeState *App_PreChargeState_GetAirOpenState(void)
{
    static struct PreChargeState state = {
        .name         = "AIR_OPEN",
        .run_on_entry = App_PreChargeState_RunOnEntryAirOpenState,
        .run_on_tick  = App_PreChargeState_RunOnTickAirOpenState
    };

    return &state;
}

struct PreChargeState *App_PreChargeState_GetPrechargingState(void)
{
    static struct PreChargeState state = {
        .name         = "PRE_CHARGING",
        .run_on_entry = App_PreChargeState_RunOnEntryPrechargingState,
        .run_on_tick  = App_PreChargeState_RunOnTickPrechargingState
    };

    return &state;
}

void App_PreChargeState_RunOnEntry(
    struct PreChargeState *const state,
    struct BmsWorld *const       world)
{
    state->run_on_entry(world);
}

void App_PreChargeState_RunOnTick(
    struct PreChargeState *const state,
    struct BmsWorld *const       world)
{
    state->run_on_tick(world);
}
