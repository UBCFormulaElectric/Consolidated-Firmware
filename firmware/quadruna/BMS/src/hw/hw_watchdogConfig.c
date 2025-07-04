#include "hw_watchdogConfig.h"
#include "hw_hal.h"
#include <string.h>
#include "app_canAlerts.h"
#include "app_canTx.h"
#include "io_canTx.h"
#include "hw_utils.h"

extern IWDG_HandleTypeDef hiwdg1;

void hw_watchdogConfig_refresh(void)
{
    HAL_IWDG_Refresh(&hiwdg1);
}

void hw_watchdogConfig_timeoutCallback(WatchdogHandle *watchdog)
{
    BREAK_IF_DEBUGGER_CONNECTED();
