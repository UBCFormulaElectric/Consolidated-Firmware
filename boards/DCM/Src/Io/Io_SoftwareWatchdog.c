#include <stm32f3xx_hal.h>
#include <string.h>
#include "auto_generated/App_CanTx.h"
#include "Io/Io_SoftwareWatchdog.h"
#include "App/App_SharedWorld.h"
#include "Io/Io_SharedMacros.h"

extern IWDG_HandleTypeDef hiwdg;
extern struct World *     world;

void Io_HardwareWatchdog_Refresh(void)
{
    HAL_IWDG_Refresh(&hiwdg);
}

void Io_SoftwareWatchdog_TimeoutCallback(SoftwareWatchdogHandle_t watchdog)
{
    BREAK_IF_DEBUGGER_CONNECTED();

    App_CanTx_SetPeriodicSignal_WATCHDOG_TIMEOUT(
        App_SharedWorld_GetCanTx(world), true);

    struct CanMsgs_dcm_watchdog_timeout_t payload;
    memcpy(
        &payload.task_name, Io_SharedSoftwareWatchdog_GetName(watchdog),
        sizeof(payload.task_name));

    App_CanTx_SendNonPeriodicMsg_DCM_WATCHDOG_TIMEOUT(
        App_SharedWorld_GetCanTx(world), &payload);
}
