#include "SEGGER_SYSVIEW.h" // Note: This will be updated at a later date.

namespace hw::sysviewDescConfig
{
void sendSystemDesc(void)
{
    SEGGER_SYSVIEW_SendSysDesc("N=BMS,D=Cortex-M7,O=FreeRTOS");
}
} // namespace hw::sysviewDescConfig