#include "app_canTx.h"
#include "app_canAlerts.h"
#include "io_log.h"

void canRxQueueOverflowCallBack(uint32_t overflow_count)
{
    app_canTx_BMS_RxOverflowCount_set(overflow_count);
    app_canAlerts_BMS_Info_RxOverflow_set(true);
    // LOG_INFO("CAN RX QUEUE OVERFLOW, overflow_count = %d", overflow_count);
}

void canTxQueueOverflowCallBack(uint32_t overflow_count)
{
    app_canTx_BMS_TxOverflowCount_set(overflow_count);
    app_canAlerts_BMS_Info_TxOverflow_set(true);
    // LOG_INFO("CAN TX QUEUE OVERFLOW, overflow_count = %d", overflow_count);
}

void canRxQueueOverflowClearCallback(void)
{
    app_canAlerts_BMS_Info_RxOverflow_set(false);
}

void canTxQueueOverflowClearCallback(void)
{
    app_canAlerts_BMS_Info_TxOverflow_set(false);
}
