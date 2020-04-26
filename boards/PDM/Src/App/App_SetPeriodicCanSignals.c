#include "App_SetPeriodicCanSignals.h"
#include "App_CanTx.h"
#include "App_InRangeCheck.h"
#include "App_ErrorCode.h"

void App_SetPeriodicCanSignals_InRangeCheck(
    struct PdmCanTxInterface *const can_tx,
    const struct InRangeCheck* const in_range_check,
    void (*const can_signal_setter)(struct PdmCanTxInterface*, float))
{
    float buffer;

    if (EXIT_CODE_OK(App_InRangeCheck_GetValue(in_range_check, &buffer)))
    {
        can_signal_setter(can_tx, buffer);
    }
}
