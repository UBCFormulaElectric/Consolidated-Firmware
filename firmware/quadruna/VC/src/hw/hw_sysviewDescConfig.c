#include "SEGGER_SYSVIEW.h"

void hw_sysviewConfig_sendSystemDesc(void)
{
    SEGGER_SYSVIEW_SendSysDesc("N=VC,C=Cortex-M7,O=FreeRTOS,D=STM32H733VG");

    // CONFIGURE INTERRUPTS HERE
    // note: +16 offset from table 140 (reference manual)
    SEGGER_SYSVIEW_SendSysDesc(
        "I#35=FDCAN1_IT0,I#70=SysTick,I#143=AD3_IRQ,I#27=ADC1_DMA,I#49=I2C2_EV,I#47=I2C1_EV,I#28=DMA1_STR1");
}

// U32 SEGGER_SYSVIEW_X_GetTimestamp(void)
// {
//     return;
// };
