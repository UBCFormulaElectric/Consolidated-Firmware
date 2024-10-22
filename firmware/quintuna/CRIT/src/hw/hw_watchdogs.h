#pragma once
#include "hw_watchdog.h"
#include "hw_hal.h"
#include "main.h"
#include "app_canUtils.h"
#include "hw_utils.h" //this is in the main.c but can I just add it to the main.h and take it out the c one so we can just include that? 
extern "C"
{
#include "app_canTx.h"
#include "app_canAlerts.h"
#include "io_canRx.h"
}

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
    app_canAlerts_CRIT_Warning_WatchdogTimeout_set(true);
    app_canTx_CRIT_WatchdogTimeoutTaskName_set(static_cast<RtosTaskName>(watchdog_id));
}
} // namespace hw::watchdogConfig