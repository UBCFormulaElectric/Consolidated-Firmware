#include "SEGGER_SYSVIEW.h"

void hw_sysviewConfig_sendSystemDesc(void)
{
    SEGGER_SYSVIEW_SendSysDesc("N=FSM,C=Cortex-M4,O=FreeRTOS,D=STM32F412RG");
    SEGGER_SYSVIEW_SendSysDesc("I#15=SysTick");

    // CONFIGURE INTERRUPTS HERE
    SEGGER_SYSVIEW_SendSysDesc("");
}
