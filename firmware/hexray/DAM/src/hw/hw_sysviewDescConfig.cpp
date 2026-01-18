extern "C"
{
    #include "SEGGER_SYSVIEW.h"
}

void hw_sysviewConfig_sendSystemDesc(void)
{
    SEGGER_SYSVIEW_SendSysDesc("N=DAM,C=Cortex-M7,O=FreeRTOS,D=STM32H562RI");
    SEGGER_SYSVIEW_SendSysDesc("I#15=SysTick");

    // CONFIGURE INTERRUPTS HERE
    // note: +16 offset from table 140 (reference manual)
    // SEGGER_SYSVIEW_SendSysDesc("I#35=FDCAN1_IT0,I#70=UART7_IRQ,I#143=AD3_IRQ,I#27=ADC1_DMA");
}

// U32 SEGGER_SYSVIEW_X_GetTimestamp(void)
// {
//     return;
// };