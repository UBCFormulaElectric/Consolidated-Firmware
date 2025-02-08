#include <stddef.h> 
#include "app_tractionControlState.h"
#include "app_canRx.h"
#include "app_canTx.h"


static void app_tractionControl_run_on_entry(void)
{
    traction_control_inputs.pid->error = 0;
    app_canTx_VC_TVInternalState_set(TRACTION_CONTROL);
    return; 
}

static void app_tractionControl_run_on_exit()
{
    traction_control_inputs.pid->integral = 0; 
    return; 
}


const TVinternalState *app_tractionControlOnState_get(void)
{
    static TVinternalState TractionControlOnState = 
    {
        .name = "TRACTION CONTROL",
        .run_on_entry = app_tractionControl_run_on_entry,
        .run_on_exit = app_tractionControl_run_on_exit,
    };

    return &TractionControlOnState;
} 
