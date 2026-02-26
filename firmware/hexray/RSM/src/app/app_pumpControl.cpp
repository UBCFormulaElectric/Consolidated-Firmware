#include "app_pumpControl.hpp"
#include "io_rPump.hpp"
#include "util_errorCodes.hpp"
#include "app_canRx.hpp"

namespace app::pumpControl
{
void monitorPumps()
{
    const std::expected<void, ErrorCode> e =
        io::rPump::setPercentage(static_cast<uint8_t>(can_rx::VC_PumpRampUpSetPoint_get()));
    UNUSED(e);
}
} // namespace app::pumpControl