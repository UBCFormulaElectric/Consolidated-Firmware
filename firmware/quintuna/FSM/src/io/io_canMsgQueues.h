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
namespace can1
{
    static void TxQueueOverflowCallback(uint32_t overflow_count)
    {
        app_canTx_FSM_TxOverflowCount_set(overflow_count);
        app_canAlerts_FSM_Warning_TxOverflow_set(true);
        // BREAK_IF_DEBUGGER_CONNECTED()
    }

    static void RxQueueOverflowCallback(uint32_t overflow_count)
    {
        app_canTx_FSM_RxOverflowCount_set(overflow_count);
        app_canAlerts_FSM_Warning_RxOverflow_set(true);
        // BREAK_IF_DEBUGGER_CONNECTED()
    }

    static void TxQueueOverflowClearCallback()
    {
        app_canAlerts_FSM_Warning_TxOverflow_set(false);
    }

    static void RxQueueOverflowClearCallback()
    {
        app_canAlerts_FSM_Warning_RxOverflow_set(false);
    }
} // namespace can1

static CanMsgQueue can1queue{ io_canRx_filterMessageId, can1::TxQueueOverflowCallback, can1::RxQueueOverflowCallback,
                              can1::TxQueueOverflowClearCallback, can1::RxQueueOverflowClearCallback };
} // namespace io