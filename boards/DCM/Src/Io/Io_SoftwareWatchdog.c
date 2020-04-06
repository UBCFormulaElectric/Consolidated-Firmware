#include <stm32f3xx_hal.h>
#include <string.h>
#include "Io_SoftwareWatchdog.h"
#include "App_SharedAssert.h"
#include "Io_SharedMacros.h"

extern IWDG_HandleTypeDef        hiwdg;
static struct DCMCanTxInterface *_can_tx = NULL;

void Io_SoftwareWatchdog_Init(struct DCMCanTxInterface * const can_tx){
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

    struct CanMsgs_dcm_watchdog_timeout_t payload;
    memcpy(
        &payload.task_name, Io_SharedSoftwareWatchdog_GetName(watchdog),
        sizeof(payload.task_name));

    App_CanTx_SendNonPeriodicMsg_DCM_WATCHDOG_TIMEOUT(_can_tx, &payload);
}
