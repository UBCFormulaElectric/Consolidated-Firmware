#include "states/App_AllStates.h"
#include "states/App_InitState.h"
#include "states/App_DriveState.h"
#include "states/App_PreChargeState.h"

#include "App_SetPeriodicCanSignals.h"
#include "App_SharedMacros.h"

#define TS_DISCHARGED_THRESHOLD_V (10.0f)

static void InitStateRunOnEntry(struct StateMachine *const state_machine)
{
    struct BmsWorld *   world         = App_SharedStateMachine_GetWorld(state_machine);
    struct Clock *      clock         = App_BmsWorld_GetClock(world);
    struct Accumulator *accumulator   = App_BmsWorld_GetAccumulator(world);
    struct OkStatus *   bms_ok_status = App_BmsWorld_GetBmsOkStatus(world);
    struct Airs *       airs          = App_BmsWorld_GetAirs(world);

    App_CanTx_BMS_Vitals_CurrentState_Set(BMS_INIT_STATE);
    App_SharedClock_SetPreviousTimeInMilliseconds(clock, App_SharedClock_GetCurrentTimeInMilliseconds(clock));
    App_Accumulator_InitRunOnEntry(accumulator);
    App_OkStatus_Enable(bms_ok_status);
    App_Airs_OpenAirPositive(airs);
}

static void InitStateRunOnTick1Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick1Hz(state_machine);
}

static void InitStateRunOnTick100Hz(struct StateMachine *const state_machine)
{
    if (App_AllStatesRunOnTick100Hz(state_machine))
    {
        struct BmsWorld *      world   = App_SharedStateMachine_GetWorld(state_machine);
        struct TractiveSystem *ts      = App_BmsWorld_GetTractiveSystem(world);
        struct Airs *          airs    = App_BmsWorld_GetAirs(world);
        struct Charger *       charger = App_BmsWorld_GetCharger(world);

        bool is_charger_connected = App_Charger_IsConnected(charger);

#ifndef BSPD_DEMO_MODE
        struct TractiveSystem *ts   = App_BmsWorld_GetTractiveSystem(world);
        struct Airs *          airs = App_BmsWorld_GetAirs(world);
        // don't allow pre_charge if in BSPD_DEMO_MODE
        if (App_Airs_IsAirNegativeClosed(airs) && (App_TractiveSystem_GetVoltage(ts) < TS_DISCHARGED_THRESHOLD_V))
        {
            // If the charger is connected, the CAN message must also be set
            // to continue into Pre-Charge State, then Charge State
            if (is_charger_connected && App_CanRx_DEBUG_ChargingSwitch_StartCharging_Get())
            {
                App_SharedStateMachine_SetNextState(state_machine, App_GetPreChargeState());
            }
            // If charger isn't connected, go into Pre-Charge State, then Drive State
            else if (!is_charger_connected)
            {
                App_SharedStateMachine_SetNextState(state_machine, App_GetPreChargeState());
            }
        }
#endif
        //      #else?
        struct HeartbeatMonitor *hb_monitor    = App_BmsWorld_GetHeartbeatMonitor(world);
        const bool               is_missing_hb = !App_SharedHeartbeatMonitor_Tick(hb_monitor);
        App_CanTx_BMS_Warnings_MissingHeartBeat_Set(is_missing_hb);
        if (is_missing_hb)
        {
            App_SharedStateMachine_SetNextState(state_machine, App_GetFaultState());
        }
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
