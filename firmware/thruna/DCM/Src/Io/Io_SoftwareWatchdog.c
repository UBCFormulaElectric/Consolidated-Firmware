#include "Io_SoftwareWatchdog.h"
#include "App_CanAlerts.h"
#include "App_CanTx.h"
#include "Io_SharedMacros.h"
#include <stm32f4xx.h>
#include <string.h>

extern IWDG_HandleTypeDef hiwdg;

void Io_HardwareWatchdog_Refresh(void)
{
    HAL_IWDG_Refresh(&hiwdg);
}

void Io_SoftwareWatchdog_TimeoutCallback(SoftwareWatchdogHandle_t watchdog)
{
    BREAK_IF_DEBUGGER_CONNECTED();

    const uint8_t watchdog_id = Io_SharedSoftwareWatchdog_GetTaskId(watchdog);
    App_CanAlerts_DCM_Warning_WatchdogTimeout_Set(true);
    App_CanTx_DCM_WatchdogTimeoutTaskName_Set((RtosTaskName)watchdog_id);
}
