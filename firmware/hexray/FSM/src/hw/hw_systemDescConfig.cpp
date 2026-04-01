extern "C"
{
#include "SEGGER_SYSVIEW.h"
}

namespace hw::sysviewConfig
{
void sendSystemDesc(void)
{
    SEGGER_SYSVIEW_SendSysDesc("N=FSM,C=Cortex-M33,O=FreeRTOS,D=STM32H562");
    SEGGER_SYSVIEW_SendSysDesc("I#15=SysTick,I#72=DMA2_Stream0");

    // CONFIGURE INTERRUPTS HERE
    SEGGER_SYSVIEW_SendSysDesc("");
}
} // namespace hw::sysviewConfig
