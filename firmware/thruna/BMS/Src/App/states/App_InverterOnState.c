#include "states/App_AllStates.h"
#include "states/App_PreChargeState.h"
#include "states/App_InverterOnState.h"
#include "App_SetPeriodicCanSignals.h"
#include "App_SharedMacros.h"
#include "App_Timer.h"

#define CHARGING_MILLISECONDS 200
TimerChannel timer;
TimerChannel exit_timer;

static void InverterOnStateRunOnEntry(struct StateMachine *const state_machine)
{
    App_CanTx_BMS_Vitals_CurrentState_Set(BMS_INVERTER_ON_STATE);
    App_Timer_InitTimer(&timer, CHARGING_MILLISECONDS);
    App_Timer_Restart(&timer);
}

static void InverterOnStateRunOnTick1Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick1Hz(state_machine);
}

static void InverterOnStateRunOnTick100Hz(struct StateMachine *const state_machine)
{
    if (App_AllStatesRunOnTick100Hz(state_machine))
    {
        TimerState timer_state = App_Timer_UpdateAndGetState(&timer);

        if (timer_state == TIMER_STATE_EXPIRED)
        {
            App_SharedStateMachine_SetNextState(state_machine, App_GetPreChargeState());
        }
    }
}

static void InverterOnStateRunOnExit(struct StateMachine *const state_machine)
{
    UNUSED(state_machine);
}

const struct State *App_GetInverterOnState(void)
{
    static struct State inverter_state = {
        .name              = "INVERTER_ON",
        .run_on_entry      = InverterOnStateRunOnEntry,
        .run_on_tick_1Hz   = InverterOnStateRunOnTick1Hz,
        .run_on_tick_100Hz = InverterOnStateRunOnTick100Hz,
        .run_on_exit       = InverterOnStateRunOnExit,
    };

    return &inverter_state;
}
