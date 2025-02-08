#include "stddef.h"
#include "app_offState.h"
#include "app_canRx.h"
#include "app_canTx.h"

static void app_offState_run_on_entry()
{
    app_canTx_VC_TVInternalState_set(TV_OFF);

}

const TVinternalState *app_torqueVectoringOff_get()
{
    static TVinternalState offState =
    {
        .name = "TV OFF",
        .run_on_entry = app_offState_run_on_entry,
        .run_on_exit = NULL
    };

    return &offState;
}