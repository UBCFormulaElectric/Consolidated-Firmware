#include "hw_canConfig.h"
#include <stdbool.h>
#include "App_CanTx.h"
#include "App_CanAlerts.h"
#include "Io_SharedMacros.h"

void hw_canConfig_txOverflowCallback(uint32_t overflow_count)
{
    App_CanTx_DIM_TxOverflowCount_Set(overflow_count);
    App_CanAlerts_DIM_TxOverflowWarning_Set(true);
    BREAK_IF_DEBUGGER_CONNECTED();
}

void hw_canConfig_rxOverflowCallback(uint32_t overflow_count)
{
    App_CanTx_DIM_RxOverflowCount_Set(overflow_count);
    App_CanAlerts_DIM_RxOverflowWarning_Set(true);
    BREAK_IF_DEBUGGER_CONNECTED();
}
