#include <stm32f3xx_hal.h>
#include <string.h>
#include "auto_generated/App_CanTx.h"
#include "App_SoftwareWatchdog.h"
#include "App_SharedWorld.h"
#include "SharedMacros.h"

extern IWDG_HandleTypeDef hiwdg;
extern struct World *     world;

void Io_HardwareWatchdog_Refresh(void)
{
    HAL_IWDG_Refresh(&hiwdg);
}

void App_SoftwareWatchdog_TimeoutCallback(SoftwareWatchdogHandle_t watchdog)
{
    BREAK_IF_DEBUGGER_CONNECTED();

    App_CanTx_SetPeriodicSignal_WATCHDOG_TIMEOUT(
        App_SharedWorld_GetCanTx(world), true);

    struct CanMsgs_fsm_watchdog_timeout_t payload;
    memcpy(
        &payload.task_name, App_SharedSoftwareWatchdog_GetName(watchdog),
        sizeof(payload.task_name));

    App_CanTx_SendNonPeriodicMsg_FSM_WATCHDOG_TIMEOUT(
        App_SharedWorld_GetCanTx(world), &payload);
}
