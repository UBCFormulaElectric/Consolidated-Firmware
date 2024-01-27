#include "hw_watchdogConfig.h"
#include <stm32f4xx.h>
#include <string.h>
#include "hw_hal.h"
#include "App_CanAlerts.h"
#include "App_CanTx.h"
#include "Io_CanTx.h"
#include "hw_utils.h"

extern IWDG_HandleTypeDef hiwdg;

void hw_watchdogConfig_refresh(void)
{
    HAL_IWDG_Refresh(&hiwdg);
}

void hw_watchdogConfig_timeoutCallback(WatchdogHandle *watchdog)
{
    BREAK_IF_DEBUGGER_CONNECTED();

    const uint8_t watchdog_id = hw_watchdog_getTaskId(watchdog);
    App_CanAlerts_PDM_Warning_WatchdogTimeout_Set(true);
    App_CanTx_PDM_WatchdogTimeoutTaskName_Set((RtosTaskName)watchdog_id);
}
