#include "SEGGER_SYSVIEW.h"

void hw_sysviewConfig_sendSystemDesc(void)
{
    SEGGER_SYSVIEW_SendSysDesc("N=FSM_Boot,D=Cortex-M4,O=FreeRTOS");
    SEGGER_SYSVIEW_SendSysDesc("I#15=SysTick");
}
