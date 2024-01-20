#include "io_canConfig.h"
#include <stdbool.h>
#include "App_CanTx.h"
#include "App_CanAlerts.h"
#include "Io_SharedMacros.h"

void io_canConfig_txOverflowCallback(uint32_t overflow_count)
{
    App_CanTx_RSM_TxOverflowCount_Set(overflow_count);
    App_CanAlerts_RSM_Warning_TxOverflow_Set(true);
    BREAK_IF_DEBUGGER_CONNECTED();
}

void io_canConfig_rxOverflowCallback(uint32_t overflow_count)
{
    App_CanTx_RSM_RxOverflowCount_Set(overflow_count);
    App_CanAlerts_RSM_Warning_RxOverflow_Set(true);
    BREAK_IF_DEBUGGER_CONNECTED();
}
