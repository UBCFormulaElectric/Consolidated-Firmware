#include "hw_watchdogConfig.h"
#include "hw_hal.h"
#include <string.h>
#include "app_canAlerts.h"
#include "app_canTx.h"
#include "io_canTx.h"
#include "hw_utils.h"
#include "io_log.h"

void hw_watchdogConfig_refresh(void)
{
    HAL_IWDG_Refresh(&hiwdg1);
}

void hw_watchdogConfig_timeoutCallback(WatchdogHandle *watchdog)
{
    BREAK_IF_DEBUGGER_CONNECTED();

    const uint8_t watchdog_id = hw_watchdog_getTaskId(watchdog);
    LOG_ERROR("Watchdog timed out on task ID %d", watchdog_id);
    // app_canAlerts_BMS_Warning_WatchdogTimeout_set(true);
    // app_canTx_BMS_WatchdogTimeoutTaskName_set((RtosTaskName)watchdog_id);
}
