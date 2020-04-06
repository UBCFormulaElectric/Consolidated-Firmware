#include <stm32f3xx_hal.h>
#include <string.h>
#include "Io_SoftwareWatchdog.h"
#include "App_FsmWorld.h"
#include "Io_SharedMacros.h"
#include "App_SharedAssert.h"

extern IWDG_HandleTypeDef        hiwdg;
static struct FSMCanTxInterface *_can_tx = NULL;

void Io_SoftwareWatchdog_Init(struct FSMCanTxInterface *const can_tx)
{
    _can_tx = can_tx;
}

void Io_HardwareWatchdog_Refresh(void)
{
    HAL_IWDG_Refresh(&hiwdg);
}

void Io_SoftwareWatchdog_TimeoutCallback(SoftwareWatchdogHandle_t watchdog)
{
    BREAK_IF_DEBUGGER_CONNECTED();

    shared_assert(_can_tx != NULL);
    App_CanTx_SetPeriodicSignal_WATCHDOG_TIMEOUT(_can_tx, true);

    struct CanMsgs_fsm_watchdog_timeout_t payload;
    memcpy(
        &payload.task_name, Io_SharedSoftwareWatchdog_GetName(watchdog),
        sizeof(payload.task_name));

    App_CanTx_SendNonPeriodicMsg_FSM_WATCHDOG_TIMEOUT(_can_tx, &payload);
}
