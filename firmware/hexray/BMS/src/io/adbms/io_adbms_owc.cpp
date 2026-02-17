#include "io_adbms.hpp"
#include "io_adbms_internal.hpp"
#include "util_errorCodes.hpp"

namespace io::adbms
{
std::expected<void, ErrorCode> owcSwitch(const OpenWireSwitch owcSwitch)
{
    const uint16_t cmd = (owcSwitch == OpenWireSwitch::EvenChannels) ? (ADSV_BASE | OW0) : (ADSV_BASE | OW1);
    return sendCmd(cmd);
}
}; // namespace io::adbms