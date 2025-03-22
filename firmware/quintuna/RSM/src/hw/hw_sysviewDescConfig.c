#include "SEGGER_SYSVIEW.h"

void hw_sysviewConfig_sendSystemDesc(void)
{
    SEGGER_SYSVIEW_SendSysDesc("N=RSM,D=Cortex-M4,O=FreeRTOS,D=STM32F412RG");
<<<<<<< HEAD
    SEGGER_SYSVIEW_SendSysDesc("I#15=SysTick,I#34=ADC,I#42=TIM2,I#72=DMA2_Stream0");
=======
    SEGGER_SYSVIEW_SendSysDesc("I#15=SysTick");
>>>>>>> 5e8451f7b (Squashed commit of the following:)
}
