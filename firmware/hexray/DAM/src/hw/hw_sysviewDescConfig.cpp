#include "SEGGER_SYSVIEW.h"

// I# = IRQn + 16 for peripheral IRQs, check stm32h562xx.h for IRQ enum
extern "C" void hw_sysviewConfig_sendSystemDesc()
{
    SEGGER_SYSVIEW_SendSysDesc("N=DAM,O=FreeRTOS,D=STM32H562RI,C=Cortex-M33");
    SEGGER_SYSVIEW_SendSysDesc("I#15=SysTick");

    // SEGGER_SYSVIEW_SendSysDesc("I#35=FDCAN1_IT0,I#70=UART7_IRQ,I#143=AD3_IRQ,I#27=ADC1_DMA");

    // I'd clarify the values you have commented above they are lowk suspicious
}

// U32 SEGGER_SYSVIEW_X_GetTimestamp(void)
// {
//     return;
// };