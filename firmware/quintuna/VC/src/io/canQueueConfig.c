#include "io/log.h"
#include "app_canAlerts.h"
#include "app_canTx.h"

void tx_overflow_callback(const uint32_t overflow_count)
{
    app_canTx_VC_RxOverflowCount_set(overflow_count);
    app_canAlerts_VC_Info_RxOverflow_set(true);
    LOG_INFO("CAN RX OVERFLOW");
}
void rx_overflow_callback(const uint32_t overflow_count)
{
    app_canTx_VC_TxOverflowCount_set(overflow_count);
    app_canAlerts_VC_Info_TxOverflow_set(true);
    LOG_INFO("CAN TX OVERFLOW");
}
void tx_overflow_clear_callback(void)
{
    app_canAlerts_VC_Info_TxOverflow_set(false);
}
void rx_overflow_clear_callback(void)
{
    app_canAlerts_VC_Info_RxOverflow_set(false);
}
