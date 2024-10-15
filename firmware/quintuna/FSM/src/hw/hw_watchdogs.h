#pragma once
#include "hw_watchdog.h"

namespace hw::watchdogConfig
{
void refresh_hardware_watchdog()
{
    HAL_IWDG_Refresh(&hiwdg);
}
void timeout_callback(hw::watchdog::WatchdogInstance *watchdog)
{
    BREAK_IF_DEBUGGER_CONNECTED();
    const uint8_t watchdog_id = watchdog->task_id;
    app_canAlerts_FSM_Warning_WatchdogTimeout_set(true);
    app_canTx_FSM_WatchdogTimeoutTaskName_set(static_cast<RtosTaskName>(watchdog_id));
}
} // namespace hw::watchdogConfig