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
    void init(){
        hw::can::can1.init();
    }

    struct CanMsgInit(){
        hw::can::CanMsg tx_msg{};
    }

    //should I be doing struct or unit32_t
    const bool popCan1TxMsgFromQueue(struct &tx_msg){
        transmit_status = hw::can::can1.transmit(tx_msg);  
        return transmit_status;  
    }

    struct popRxMsgFromCan1Queue(){
        hw::can::CanMsg rx_msg = io::can1queue.popRxMsgFromQueue();
    }


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


} // namespace can1

static CanMsgQueue can1queue{ io_canRx_filterMessageId, can1::TxQueueOverflowCallback, can1::RxQueueOverflowCallback,
                              can1::TxQueueOverflowClearCallback, can1::RxQueueOverflowClearCallback };
} // namespace io