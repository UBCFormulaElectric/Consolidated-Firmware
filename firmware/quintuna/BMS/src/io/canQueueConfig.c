// #include "app_canTx.h"
// #include "app_canAlerts.h"
#include "io/log.h"

void rx_overflow_callback(uint32_t overflow_count)
{
    // app_canTx_BMS_RxOverflowCount_set(overflow_count);
    // app_canAlerts_BMS_Warning_RxOverflow_set(true);
    LOG_INFO("CAN RX QUEUE OVERFLOW, overflow_count = %d", overflow_count);
}

void tx_overflow_callback(uint32_t overflow_count)
{
    // app_canTx_BMS_TxOverflowCount_set(overflow_count);
    // app_canAlerts_BMS_Warning_TxOverflow_set(true);
    LOG_INFO("CAN TX QUEUE OVERFLOW, overflow_count = %d", overflow_count);
}

void tx_overflow_clear_callback(void)
{
    // app_canAlerts_BMS_Warning_TxOverflow_set(false);
}

void rx_overflow_clear_callback(void)
{
    // app_canAlerts_BMS_Warning_RxOverflow_set(false);
}