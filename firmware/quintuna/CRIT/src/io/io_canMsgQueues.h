#pragma once

#include "io_canMsgQueue.h"
#include "hw_utils.h"
extern "C"
{
#include "app_canTx.h"
#include "app_canAlerts.h"
#include "io_canRx.h"
}

namespace io
{
namespace can2
{
    static void TxQueueOverflowCallback(uint32_t overflow_count)
    {
        app_canTx_CRIT_TxOverflowCount_set(overflow_count);
        app_canAlerts_CRIT_Warning_TxOverflow_set(true);
        // BREAK_IF_DEBUGGER_CONNECTED()
    }

    static void RxQueueOverflowCallback(uint32_t overflow_count)
    {
        app_canTx_CRIT_RxOverflowCount_set(overflow_count);
        app_canAlerts_CRIT_Warning_RxOverflow_set(true);
        // BREAK_IF_DEBUGGER_CONNECTED()
    }

    static void TxQueueOverflowClearCallback()
    {
        app_canAlerts_CRIT_Warning_TxOverflow_set(false);
    }

    static void RxQueueOverflowClearCallback()
    {
        app_canAlerts_CRIT_Warning_RxOverflow_set(false);
    }
} // namespace can2

static CanMsgQueue can2queue{ io_canRx_filterMessageId, can2::TxQueueOverflowCallback, can2::RxQueueOverflowCallback,
                              can2::TxQueueOverflowClearCallback, can2::RxQueueOverflowClearCallback };
} // namespace io