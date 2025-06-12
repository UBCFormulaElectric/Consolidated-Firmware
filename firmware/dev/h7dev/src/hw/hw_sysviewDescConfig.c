#include "SEGGER_SYSVIEW.h"

void hw_sysviewConfig_sendSystemDesc(void)
{
    SEGGER_SYSVIEW_SendSysDesc("N=H7Dev,D=Cortex-M7,O=FreeRTOS");
    SEGGER_SYSVIEW_SendSysDesc("I#15=SysTick");
}