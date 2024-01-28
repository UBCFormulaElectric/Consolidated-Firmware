#include "hw_watchdogConfig.h"
#include <stm32h7xx.h>
#include <string.h>
#include "hw_hal.h"
#include "app_canAlerts.h"
#include "app_canTx.h"
#include "io_canTx.h"
#include "stm32h7xx_hal_iwdg.h"
#include "hw_utils.h"

extern IWDG_HandleTypeDef hiwdg1;

void hw_watchdogConfig_refresh(void)
{
    HAL_IWDG_Refresh(&hiwdg1);
}

void hw_watchdogConfig_timeoutCallback(WatchdogHandle *watchdog)
{
    BREAK_IF_DEBUGGER_CONNECTED();

    const uint8_t watchdog_id = hw_watchdog_getTaskId(watchdog);
    app_canAlerts_VC_Warning_WatchdogTimeout_set(true);
    app_canTx_VC_WatchdogTimeoutTaskName_set((RtosTaskName)watchdog_id);
}
