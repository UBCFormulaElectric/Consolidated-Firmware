#include "app_yawRateController.h"
#include "app_canRx.h"
#include "app_canTx.h"

static void app_yawRateController_run_on_entry()
{
    app_canTx_VC_TVInternalState_set(ACTIVE_DIFF);
    return;
} 

const TVinternalState *app_yawRateController_onState_get()
{
    static TVinternalState yawRateControllerState = 
    {
        .name = "YAWRATE CONTROLLER",
        .run_on_entry = app_yawRateController_run_on_entry,
        .run_on_exit  = NULL
    };

    return &yawRateControllerState;
}