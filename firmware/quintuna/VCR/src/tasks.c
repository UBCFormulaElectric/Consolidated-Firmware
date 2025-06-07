#include "tasks.h"
#include "hw_hardFaultHandler.h"
#include "io_log.h"
#include "main.h"
#include "SEGGER_SYSVIEW.h"
#include <cmsis_os2.h>

void tasks_preInit()
{
    hw_hardFaultHandler_init();
}
void tasks_init()
{
    // Configure and initialize SEGGER SystemView.
    // NOTE: Needs to be done after clock config!
    SEGGER_SYSVIEW_Conf();
    LOG_INFO("VC reset!");

    __HAL_DBGMCU_FREEZE_IWDG1();
}

_Noreturn void tasks_canTx()
{
    // osDelay(osWaitForever);
    for (;;)
    {
        HAL_GPIO_TogglePin(BOOT_GPIO_Port, BOOT_Pin);
        osDelay(100);
    }
}
_Noreturn void tasks_canRx()
{
    osDelay(osWaitForever);
    for (;;)
    {
    }
}
