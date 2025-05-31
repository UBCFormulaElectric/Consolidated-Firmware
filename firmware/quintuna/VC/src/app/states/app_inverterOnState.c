#include "app_inverterOnState.h"
#include "app_canUtils.h"
#include "app_canRx.h"
#include "app_canTx.h"
#include "app_stateMachine.h"
#include "app_allStates.h"
#include "app_bmsOnState.h"

#include "hw_gpios.h"

#define INVERTER_DEBOUNCE_CYCLES 10

static void inverterOnStateRunOnEntry(void)
{
    app_canTx_VC_State_set(VC_INVERTER_ON_STATE);
}

static void inverterOnStateRunOnTick1Hz(void)
{
    app_allStates_runOnTick1Hz();
}

static void inverterOnStateRunOnTick100Hz(void)
{
    app_allStates_runOnTick100Hz();

    static uint8_t cycle_count = 0;

    // Stagger inverters
    // Enables rear inverters
    if (cycle_count == 0)
    {
        hw_gpio_writePin(&r_inv_en, true);
    }
    // Wait 10 100Hz cycles (100ms) to enable front inverters.
    else if (cycle_count == INVERTER_DEBOUNCE_CYCLES)
    {
        hw_gpio_writePin(&f_inv_en, true);
    }
    // Both inverters are enables, transition to PCM_ON state.
    else if (cycle_count > INVERTER_DEBOUNCE_CYCLES)
    {
        app_stateMachine_setNextState(&bmsOn_state);
    }
}
static void inverterOnStateRunOnExit(void) {}

State inverterOn_state = { .name              = "INVERTER ON",
                           .run_on_entry      = inverterOnStateRunOnEntry,
                           .run_on_tick_1Hz   = inverterOnStateRunOnTick1Hz,
                           .run_on_tick_100Hz = inverterOnStateRunOnTick100Hz,
                           .run_on_exit       = inverterOnStateRunOnExit };