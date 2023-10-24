#include "io_watchdogConfig.h"
#include <stm32f4xx.h>
#include <string.h>
#include "App_CanAlerts.h"
#include "App_CanTx.h"
#include "Io_CanTx.h"
#include "Io_SharedMacros.h"

extern IWDG_HandleTypeDef hiwdg;

void io_watchdogConfig_refresh(void)
{
    HAL_IWDG_Refresh(&hiwdg);
}

void io_watchdogConfig_timeoutCallback(SoftwareWatchdogHandle_t watchdog)
{
    BREAK_IF_DEBUGGER_CONNECTED();

    const uint8_t watchdog_id = Io_SharedSoftwareWatchdog_GetTaskId(watchdog);
    App_CanAlerts_SetWarning(DIM_WARNING_WATCHDOG_TIMEOUT, true);
    App_CanTx_DIM_WatchdogTimeoutTaskName_Set((RtosTaskName)watchdog_id);
}
