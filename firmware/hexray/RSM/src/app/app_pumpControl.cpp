#include "app_pumpControl.hpp"
#include "io_rPump.hpp"
extern "C"
{
    #include "app_canRx.h"
}

namespace app::pumpControl
{
    void monitorPumps()
    {
        io::rPump::setPercentage(app_canRx_VC_PumpRampUpSetPoint_get());
    }
}
