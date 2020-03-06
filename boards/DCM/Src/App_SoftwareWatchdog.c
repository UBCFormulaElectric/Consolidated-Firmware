#include <stm32f3xx_hal.h>
#include <string.h>
#include "auto_generated/App_CanTx.h"
#include "App_SoftwareWatchdog.h"
#include "SharedMacros.h"

extern IWDG_HandleTypeDef hiwdg;

void Io_HardwareWatchdog_Refresh(void)
{
    HAL_IWDG_Refresh(&hiwdg);
}

void App_SoftwareWatchdog_TimeoutCallback(SoftwareWatchdogHandle_t watchdog)
{
    BREAK_IF_DEBUGGER_CONNECTED();

    App_CanTx_SetPeriodicSignal_WATCHDOG_TIMEOUT(true);

    struct CanMsgs_dcm_watchdog_timeout_t payload;
    memcpy(
        &payload.task_name, App_SharedSoftwareWatchdog_GetName(watchdog),
        sizeof(payload.task_name));

    App_CanTx_EnqueueNonPeriodicMsg_DCM_WATCHDOG_TIMEOUT(&payload);
}
