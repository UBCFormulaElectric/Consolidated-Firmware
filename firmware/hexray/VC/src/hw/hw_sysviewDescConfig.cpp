#include "SEGGER_SYSVIEW.h"

// I# = IRQn + 16 for peripheral IRQs, check stm32h733xx.h for IRQ enum
extern "C" void hw_sysviewConfig_sendSystemDesc()
{
    SEGGER_SYSVIEW_SendSysDesc("N=VC,O=FreeRTOS,D=STM32H733VG,C=Cortex-M7");
    SEGGER_SYSVIEW_SendSysDesc("I#15=SysTick");
}