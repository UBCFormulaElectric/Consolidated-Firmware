#include "io_adbms.hpp"
#include "io_adbms_internal.hpp"
#include "util_errorCodes.hpp"

namespace io::adbms
{
std::expected<void, ErrorCode> owcCells(const OpenWireSwitch owcSwitch)
{
    const uint16_t cmd = (owcSwitch == OpenWireSwitch::EvenChannels) ? (ADSV_BASE | OW0) : (ADSV_BASE | OW1);
    return sendCmd(cmd);
}

std::expected<void, ErrorCode> owcTherms(const OpenWireSwitch owcSwitch)
{
    const uint16_t cmd = (owcSwitch == OpenWireSwitch::EvenChannels) ? (ADAX_BASE | OW) : (ADAX_BASE | OW | PUP);
    return sendCmd(cmd);
}
}; // namespace io::adbms