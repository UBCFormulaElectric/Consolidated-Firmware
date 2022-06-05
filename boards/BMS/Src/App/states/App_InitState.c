#include "states/App_AllStates.h"
#include "states/App_InitState.h"
#include "states/App_PreChargeState.h"
#include "states/App_BalanceState.h"
#include "App_SharedMacros.h"

#define TS_DISCHARGED_THRESHOLD_V (10.0f)

static void InitStateRunOnEntry(struct StateMachine *const state_machine)
{
    struct BmsWorld *         world            = App_SharedStateMachine_GetWorld(state_machine);
    struct BmsCanTxInterface *can_tx_interface = App_BmsWorld_GetCanTx(world);
    struct Clock *            clock            = App_BmsWorld_GetClock(world);
    struct Accumulator *      accumulator      = App_BmsWorld_GetAccumulator(world);
    struct OkStatus *         bms_ok_status    = App_BmsWorld_GetBmsOkStatus(world);

    App_CanTx_SetPeriodicSignal_STATE(can_tx_interface, CANMSGS_BMS_STATE_MACHINE_STATE_INIT_CHOICE);

    App_SharedClock_SetPreviousTimeInMilliseconds(clock, App_SharedClock_GetCurrentTimeInMilliseconds(clock));
    App_Accumulator_InitRunOnEntry(accumulator);
    App_OkStatus_Enable(bms_ok_status);
}

static void InitStateRunOnTick1Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick1Hz(state_machine);
}

static void InitStateRunOnTick100Hz(struct StateMachine *const state_machine)
{
    if (App_AllStatesRunOnTick100Hz(state_machine))
    {
        struct BmsWorld *         world       = App_SharedStateMachine_GetWorld(state_machine);
        struct BmsCanRxInterface *can_rx      = App_BmsWorld_GetCanRx(world);
        struct TractiveSystem *   ts          = App_BmsWorld_GetTractiveSystem(world);
        struct Airs *             airs        = App_BmsWorld_GetAirs(world);
        struct Accumulator *      accumulator = App_BmsWorld_GetAccumulator(world);
        struct Charger *          charger     = App_BmsWorld_GetCharger(world);
        const struct State *      next_state  = App_GetInitState();

#ifndef BSPD_DEMO_MODE
        // Cell balancing may be overriden by the injection of a CAN message. This will allow the state machine to idle
        // in the init state. If charging is completed and the charger is connected, remain in the init state.
        if (App_Airs_IsAirNegativeClosed(airs) && (App_TractiveSystem_GetVoltage(ts) < TS_DISCHARGED_THRESHOLD_V) &&
            (!App_Accumulator_IsPackFullyCharged(accumulator) || !App_Charger_IsConnected(charger)) &&
            !App_CanRx_CAN_DEBUGGING_SIGNALS_GetSignal_DISABLE_CELL_BALANCING(can_rx))
        {
            next_state = App_GetPreChargeState();
        }
        else if (App_CanRx_CAN_DEBUGGING_SIGNALS_GetSignal_ENABLE_BALANCING_FROM_INIT(can_rx))
        {
            // Balancing may be enabled directly from the init state
            next_state = App_GetBalanceState();
        }
#endif

        App_SharedStateMachine_SetNextState(state_machine, next_state);
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
