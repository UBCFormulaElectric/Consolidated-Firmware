#include "app_pumpControl.h"

#include "io_rPump.h"
#include "app_canRx.h"

void app_pumpControl_MonitorPumps(void)
{
    io_rPump_setPercentage(app_canRx_VC_PumpRampUpSetPoint_get());
}