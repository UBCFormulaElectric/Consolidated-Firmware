#include "io_watchdogConfig.h"
#include "App_CanAlerts.h"
#include "App_CanTx.h"
#include "Io_CanTx.h"
#include "Io_SharedMacros.h"
#include "hw_hal.h"
#include "stm32h7xx_hal_iwdg.h"
#include <stm32h7xx.h>
#include <string.h>

extern IWDG_HandleTypeDef hiwdg1;

void io_watchdogConfig_refresh(void)
{
    HAL_IWDG_Refresh(&hiwdg1);
}

void io_watchdogConfig_timeoutCallback(SoftwareWatchdogHandle_t watchdog)
{
    BREAK_IF_DEBUGGER_CONNECTED();

    const uint8_t watchdog_id = Io_SharedSoftwareWatchdog_GetTaskId(watchdog);
    App_CanAlerts_VC_Warning_WatchdogTimeout_Set(true);
    App_CanTx_VC_WatchdogTimeoutTaskName_Set((RtosTaskName)watchdog_id);
}
