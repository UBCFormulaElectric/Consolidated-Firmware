#include <stm32f4xx.h>
#include <string.h>
#include "App_CanTx.h"
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

    // App_CanTx_SetPeriodicSignal_WATCHDOG_TIMEOUT(_can_tx, true);
    App_CanTx_DCM_Warnings_WatchdogTimeout_Set(true);

    // struct CanMsgs_dcm_watchdog_timeout_t payload;
    // memcpy(&payload.task_name, Io_SharedSoftwareWatchdog_GetName(watchdog), sizeof(payload.task_name));
    // const uint8_t watchdog_id = Io_SharedSoftwareWatchdog_GetTaskId(watchdog);

    // JSONCAN, check if true or false-> App_CanTx_SendNonPeriodicMsg_DCM_WATCHDOG_TIMEOUT(_can_tx, &payload);
    App_CanTx_DCM_Warnings_WatchdogTimeout_Set(true);
}
