#include "app_pumpControl.hpp"
#include "io_rPump.hpp"
#include "util_errorCodes.hpp"
#include "app_canRx.hpp"
#include <app_canTx.hpp>

namespace app::pumpControl
{
void monitorPumps()
{
    const std::expected<void, ErrorCode> e =
        io::rPump::setPercentage(static_cast<uint8_t>(can_rx::VC_PumpRampUpSetPoint_get()));
    UNUSED(e);
}

void broadcast()
{
    uint8_t percentage; 
    io::rPump::readPercentage(percentage);
    can_tx::RSM_RPumpPercentage_set(percentage);
}

} // namespace app::pumpControl