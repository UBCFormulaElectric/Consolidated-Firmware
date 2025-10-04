#include "app_pumpControl.hpp"
#include "io_rPump.hpp"
#include "app_canRx.hpp"

int app_canRx_VC_PumpRampUpSetPoint_get(){return 0;} //stub function

void app_pumpControl_MonitorPumps();
{
    io_rPump_setPercentage(app_canRx_VC_PumpRampUpSetPoint_get());
}