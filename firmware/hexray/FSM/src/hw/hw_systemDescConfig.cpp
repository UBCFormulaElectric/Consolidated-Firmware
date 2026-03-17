#include "SEGGER_SYSVIEW.h"

extern "C" void hw_sysviewConfig_sendSystemDesc()
{
    SEGGER_SYSVIEW_SendSysDesc("N=FSM,C=Cortex-M33,O=FreeRTOS,D=STM32H562");
    SEGGER_SYSVIEW_SendSysDesc("I#15=SysTick,I#72=DMA2_Stream0");
}
