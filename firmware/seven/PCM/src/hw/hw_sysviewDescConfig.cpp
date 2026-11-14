#include "SEGGER_SYSVIEW.h"

extern "C" void hw_sysviewConfig_sendSystemDesc()
{
    SEGGER_SYSVIEW_SendSysDesc("N=PCM,O=FreeRTOS,D=STM32F412RG,C=Cortex-M4");
    SEGGER_SYSVIEW_SendSysDesc("I#15=SysTick");
}
