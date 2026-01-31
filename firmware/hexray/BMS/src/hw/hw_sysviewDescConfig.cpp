#include "SEGGER_SYSVIEW.h" // Note: Are we planning on making an hpp version?

namespace hw::sysviewDescConfig
{
void sendSystemDesc(void)
{
    SEGGER_SYSVIEW_SendSysDesc("N=BMS,D=Cortex-M7,O=FreeRTOS");
}
} // namespace hw::sysviewDescConfig