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

    App_CanTx_BMS_Warnings_WatchdogTimeout_Set(true);
}
