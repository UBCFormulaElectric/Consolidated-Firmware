#include "tasks.h"
#include "main.h"
#include <cmsis_os2.h>

void tasks_preInit() {}
void tasks_init() {}

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