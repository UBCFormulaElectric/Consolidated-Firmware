#include "states/app_states.h"
#include "app_prechargeLatchState.h"
#include "app_precharge.h"

static void prechargeLatchStateRunOnEntry(void)
{
    app_canTx_BMS_State_set(BMS_PRECHARGE_LATCH_STATE);


}

static void prechargeLatchStateRunOnTick100Hz(void)
{
    osDelay(3000U); // Wait for 3 seconds to allow precharge circuit resistors to 
    app_stateMachine_setNextState(app_initState_get());
    
}

static void prechargeLatchStateRunOnExit(void)
{
    // TODO: Reset precharge limit exceeded
    
}

const State precharge_latch_state = {
    .name              = "PRECHARGE_LATCH",
    .run_on_entry      = prechargeLatchStateRunOnEntry,
    .run_on_tick_100Hz = prechargeLatchStateRunOnTick100Hz,
    .run_on_exit       = prechargeLatchStateRunOnExit,
};