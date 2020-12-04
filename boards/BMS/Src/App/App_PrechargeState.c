#include "App_PrechargeState.h"
#include "App_PrechargeStateMachine.h"

static void App_PreChargeState_RunOnTickInitState(struct BmsWorld *const world)
{
    struct BmsCanTxInterface *can_tx = App_BmsWorld_GetCanTx(world);
    App_CanTx_SetPeriodicSignal_PRECHARGE_STATE(
        can_tx, CANMSGS_BMS_PRECHARGE_STATES_PRECHARGE_STATE_INIT_CHOICE);
}

static void
    App_PreChargeState_RunOnTickWaitAirNegState(struct BmsWorld *const world)
{
    struct BmsCanTxInterface *can_tx = App_BmsWorld_GetCanTx(world);
    struct PreChargeSequence *pre_charge_sequence =
        App_BmsWorld_GetPreChargeSequence(world);
    struct BinaryStatus *air_negative = App_BmsWorld_GetAirNegative(world);

    App_CanTx_SetPeriodicSignal_PRECHARGE_STATE(
        can_tx, CANMSGS_BMS_PRECHARGE_STATES_PRECHARGE_STATE_AIR_CHOICE);

    if (App_SharedBinaryStatus_IsActive(air_negative))
    {
        App_PreChargeStateMachine_SetNextState(
            pre_charge_sequence, App_PreChargeState_GetPrechargingState());
    }
}

static void
    App_PreChargeState_RunOnTickPrechargingState(struct BmsWorld *const world)
{
    struct BmsCanTxInterface *can_tx = App_BmsWorld_GetCanTx(world);
    // struct PreChargeSequence * pre_charge_sequence=
    // App_BmsWorld_GetPreChargeSequence(world); struct Clock *clock =
    // App_BmsWorld_GetClock(world);

    App_CanTx_SetPeriodicSignal_PRECHARGE_STATE(
        can_tx,
        CANMSGS_BMS_PRECHARGE_STATES_PRECHARGE_STATE_PRECHARGING_CHOICE);

    if (App_CanTx_GetPeriodicSignal_PRECHARGING_CONDITION(can_tx) ==
        CANMSGS_BMS_AIR_SHUTDOWN_ERRORS_PRECHARGING_CONDITION_PRECHARGING_CHOICE)
    {
        // enum TSExitCode ts_exit_code;
        // App_TractiveSystem_CheckPrechargeBusVoltage(
        //    tractive_system, &ts_exit_code,
        // App_SharedClock_GetCurrentTimeInMilliseconds(clock));

        // if (ts_exit_code != TS_BUS_VOLTAGE_IN_RANGE)
        //{
        //    App_TractiveSystem_DisablePrecharge(tractive_system);
        //}
        // else
        //{
        //    App_TractiveSystem_EnablePrecharge(tractive_system);
        //}

        // if (ts_exit_code == TS_PRECHARGE_SUCCESS)
        //{
        //    App_CanTx_SetPeriodicSignal_PRECHARGING_CONDITION(
        //        can_tx,
        //        CANMSGS_BMS_AIR_SHUTDOWN_ERRORS_PRECHARGING_CONDITION_SUCCESS_CHOICE);
        //}
        // else if (
        //    ts_exit_code == TS_BUS_OVERVOLTAGE_ERROR ||
        //    ts_exit_code == TS_BUS_UNDERVOLTAGE_ERROR)
        //{
        //    App_CanTx_SetPeriodicSignal_PRECHARGING_CONDITION(
        //        can_tx,
        //        CANMSGS_BMS_AIR_SHUTDOWN_ERRORS_PRECHARGING_CONDITION_FAIL_CHOICE);
        //}
    }
}

struct PreChargeState *App_PreChargeState_GetInitState(void)
{
    static struct PreChargeState state = {
        .name = "INIT", .run_on_tick = App_PreChargeState_RunOnTickInitState
    };

    return &state;
}

struct PreChargeState *App_PreChargeState_GetAIROpenState(void)
{
    static struct PreChargeState state = {
        .name        = "AIR_OPEN",
        .run_on_tick = App_PreChargeState_RunOnTickWaitAirNegState
    };

    return &state;
}

struct PreChargeState *App_PreChargeState_GetPrechargingState(void)
{
    static struct PreChargeState state = {
        .name        = "PRE_CHARGING",
        .run_on_tick = App_PreChargeState_RunOnTickPrechargingState
    };

    return &state;
}

void App_PreChargeState_RunOnTick(
    struct PreChargeState *state,
    struct BmsWorld *      world)
{
    state->run_on_tick(world);
}
