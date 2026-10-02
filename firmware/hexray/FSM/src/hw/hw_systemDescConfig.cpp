#include "SEGGER_SYSVIEW.h"

// I# = IRQn + 16 for peripheral IRQs, check stm32h562xx.h for IRQ enum
extern "C" void hw_sysviewConfig_sendSystemDesc()
{
    SEGGER_SYSVIEW_SendSysDesc("N=FSM,O=FreeRTOS,D=STM32H562RI,C=Cortex-M33");
    SEGGER_SYSVIEW_SendSysDesc("I#15=SysTick");
}
