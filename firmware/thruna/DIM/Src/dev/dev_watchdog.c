#include <stm32f4xx.h>
#include <string.h>
#include "App_CanAlerts.h"
#include "App_CanTx.h"
#include "Io_CanTx.h"
#include "dev_watchdog.h"
#include "Io_SharedMacros.h"

extern IWDG_HandleTypeDef hiwdg;

void dev_watchdog_feedWatchdog()
{
    HAL_IWDG_Refresh(&hiwdg);
}

void dev_watchdog_timeoutCallback(SoftwareWatchdogHandle_t watchdog)
{
    BREAK_IF_DEBUGGER_CONNECTED();

    const uint8_t watchdog_id = Io_SharedSoftwareWatchdog_GetTaskId(watchdog);
    App_CanAlerts_SetWarning(DIM_WARNING_WATCHDOG_TIMEOUT, true);
    App_CanTx_DIM_AlertsContext_WatchdogTimeoutTaskName_Set((RtosTaskName)watchdog_id);
}
