extern "C"
{
    #include "SEGGER_SYSVIEW.h"
}

namespace hw::sysviewConfig
{
    void sendSystemDesc(void)
    {
        SEGGER_SYSVIEW_SendSysDesc("N=RSM,D=Cortex-M33,O=FreeRTOS,D=STM32H562");
        SEGGER_SYSVIEW_SendSysDesc("I#15=SysTick,I#34=ADC,I#72=DMA1_Stream0");
    }
}
