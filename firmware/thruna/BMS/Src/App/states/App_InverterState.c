#include "states/App_AllStates.h"
#include "states/App_DriveState.h"
#include "states/App_InverterState.h"
#include "App_SetPeriodicCanSignals.h"
#include "App_SharedMacros.h"
#include "App_Timer.h"

#define CHARGING_MILLISECONDS 2000
static TimerChannel invertor_timer;

static void InverterStateRunOnEntry(struct StateMachine *const state_machine)
{
    App_CanTx_BMS_Vitals_CurrentState_Set(BMS_DRIVE_STATE);
    App_Timer_InitTimer(&invertor_timer, CHARGING_MILLISECONDS);
}

static void InverterStateRunOnTick1Hz(struct StateMachine *const state_machine)
{
    App_AllStatesRunOnTick1Hz(state_machine);
}

static void InverterStateRunOnTick100Hz(struct StateMachine *const state_machine)
{
    TimerState timerState = App_Timer_UpdateAndGetState(&invertor_timer); 

    if (timerState == TIMER_STATE_EXPIRED)
    {
        App_SharedStateMachine_SetNextState(state_machine, App_GetDriveState());
    }
}

static void InverterStateRunOnExit(struct StateMachine *const state_machine)
{
    UNUSED(state_machine);
}

const struct State *App_GetInverterState(void)
{
    static struct State inverter_state = {
        .name              = "INVERTER",
        .run_on_entry      = InverterStateRunOnEntry,
        .run_on_tick_1Hz   = InverterStateRunOnTick1Hz,
        .run_on_tick_100Hz = InverterStateRunOnTick100Hz,
        .run_on_exit       = InverterStateRunOnExit,
    };

    return &inverter_state;
}
