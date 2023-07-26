#include "softwareWatchdog.h"
#include <stm32f4xx.h>
#include <string.h>
#include "App_CanAlerts.h"
#include "App_CanTx.h"
#include "Io_SharedMacros.h"

static IWDG_HandleTypeDef watchdog;

void softwareWatchdog_init(IWDG_HandleTypeDef watchdog_handle)
{
    watchdog = watchdog_handle;
}

void softwareWatchdog_refreshWatchdog(void)
{
    HAL_IWDG_Refresh(&watchdog);
}

void softwareWatchdog_timeoutCallback(SoftwareWatchdogHandle_t watchdog)
{
    BREAK_IF_DEBUGGER_CONNECTED();

    const uint8_t watchdog_id = Io_SharedSoftwareWatchdog_GetTaskId(watchdog);
    App_CanAlerts_SetWarning(DIM_WARNING_WATCHDOG_TIMEOUT, true);
    App_CanTx_DIM_AlertsContext_WatchdogTimeoutTaskName_Set((RtosTaskName)watchdog_id);
}
