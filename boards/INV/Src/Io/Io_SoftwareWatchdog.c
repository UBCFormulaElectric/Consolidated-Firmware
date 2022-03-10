#ifdef STM32F302xC
#include <stm32f3xx.h>
#elif STM32F765xx
#include <stm32f7xx.h>
#else
#error "No supported device define found."
#endif

#include <string.h>
#include "App_CanTx.h"
#include "Io_SoftwareWatchdog.h"
#include "Io_SharedMacros.h"

extern IWDG_HandleTypeDef        hiwdg;
static struct InvCanTxInterface *_can_tx = NULL;

void Io_SoftwareWatchdog_Init(struct InvCanTxInterface *const can_tx)
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

    App_CanTx_SetPeriodicSignal_WATCHDOG_TIMEOUT(_can_tx, true);

    struct CanMsgs_inv_watchdog_timeout_t payload;

    payload.task_name = Io_SharedSoftwareWatchdog_GetName(watchdog);

    App_CanTx_SendNonPeriodicMsg_INV_WATCHDOG_TIMEOUT(_can_tx, &payload);
}
