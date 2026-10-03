#include "SEGGER_SYSVIEW.h"

#include "io_adbms.hpp"
#include "app_segments.hpp"

// I# = IRQn + 16 for peripheral IRQs, check stm32h733xx.h for IRQ enum
extern "C" void hw_sysviewConfig_sendSystemDesc()
{
    SEGGER_SYSVIEW_SendSysDesc("N=BMS,O=FreeRTOS,D=STM32H733VG,C=Cortex-M7");
    SEGGER_SYSVIEW_SendSysDesc("I#100=SPI4");
    //SEGGER_SYSVIEW_SendSysDesc("I#15=SysTick,I#100=SPI4_GLOBAL,I#36=FDCAN2_IT0");
    // SEGGER_SYSVIEW_SendSysDesc("I#27=DMA1_STR0,I#66=TIM5_GLOBAL,I#44=TIM2_GLOBAL,I#43=TIM1_CC");

    SEGGER_SYSVIEW_NameMarker(io::adbms::SYSVIEW_MARKER_SPI_BUS_LOCK, "ADBMS SPI bus lock");
    SEGGER_SYSVIEW_NameMarker(SYSVIEW_MARKER_SHARED_LOCK, "Segments shared lock");
}
