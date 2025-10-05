#include "app_pumpControl.hpp"
#include "io_rPump.hpp"
extern "C"
{
    #include "app_canRx.h"
}

void app_pumpControl_MonitorPumps()
{
    io_rPump_setPercentage(app_canRx_VC_PumpRampUpSetPoint_get());
}