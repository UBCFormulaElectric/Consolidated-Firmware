#include "io_canQueues.h"

#include "io_log.h"
#include "app_canAlerts.h"
#include "app_canTx.h"

static void tx_overflow_callback(const uint32_t overflow_count)
{
    app_canTx_VC_RxOverflowCount_set(overflow_count);
    app_canAlerts_VC_Warning_RxOverflow_set(true);
    LOG_INFO("CAN1 RX OVERFLOW");
}
static void rx_overflow_callback(const uint32_t overflow_count)
{
    app_canTx_VC_TxOverflowCount_set(overflow_count);
    app_canAlerts_VC_Warning_TxOverflow_set(true);
    LOG_INFO("CAN1 TX OVERFLOW");
}
static void tx_overflow_clear_callback(void)
{
    app_canAlerts_VC_Warning_TxOverflow_set(false);
}
static void rx_overflow_clear_callback(void)
{
    app_canAlerts_VC_Warning_RxOverflow_set(false);
}
CanQueue canQueue1 = {
    .tx_overflow_callback       = tx_overflow_callback,
    .rx_overflow_callback       = rx_overflow_callback,
    .tx_overflow_clear_callback = tx_overflow_clear_callback,
    .rx_overflow_clear_callback = rx_overflow_clear_callback,
    .can_number                 = 1,
    .init_complete              = false,
};
