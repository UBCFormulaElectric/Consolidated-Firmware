#include "app_pumpControl.hpp"
#include "io_rPump.hpp"
#include "util_errorCodes.hpp"
#include "app_canRx.hpp"
#include <app_canTx.hpp>

namespace app::pumpControl
{
void monitorPumps()
{
    const float setpoint = can_rx::VC_PumpRampUpSetPoint_get();
    LOG_IF_ERR(io::rPump::setPercentage(static_cast<uint8_t>(setpoint)));
}

void broadcast()
{
    if (const auto percentage = io::rPump::readPercentage())
    {
        can_tx::RSM_RPumpPercentage_set(percentage.value());
    }
    else
    {
        can_tx::RSM_RPumpPercentage_set(255);
    }
}

} // namespace app::pumpControl