#include "Io_Hal.h"
#include <string.h>
#include "App_CanTx.h"
#include "Io_CanTx.h"
#include "App_CanAlerts.h"
#include "Io_SoftwareWatchdog.h"
#include "Io_SharedMacros.h"

extern IWDG_HandleTypeDef hiwdg;

void Io_HardwareWatchdog_Refresh(void)
{
    HAL_IWDG_Refresh(&hiwdg);
}

void Io_SoftwareWatchdog_TimeoutCallback(SoftwareWatchdogHandle_t watchdog)
{
    BREAK_IF_DEBUGGER_CONNECTED();

    const uint8_t watchdog_id = Io_SharedSoftwareWatchdog_GetTaskId(watchdog);
    App_CanAlerts_SetWarning(BMS_WARNING_WATCHDOG_TIMEOUT, true);
    App_CanTx_BMS_AlertsContext_WatchdogTimeoutTaskName_Set((RtosTaskName)watchdog_id);
}
