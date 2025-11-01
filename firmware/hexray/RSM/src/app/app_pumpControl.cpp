#include "app_pumpControl.hpp"
#include "io_rPump.hpp"
#include "util_errorCodes.hpp"
extern "C"
{
    #include "app_canRx.h"
}

namespace app::pumpControl
{
    void monitorPumps()
    {
        const ExitCode e = io::rPump::setPercentage(app_canRx_VC_PumpRampUpSetPoint_get());
    }
}