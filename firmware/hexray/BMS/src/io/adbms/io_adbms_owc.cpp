#include "io_adbms.hpp"
#include "io_adbms_internal.hpp"
#include "util_errorCodes.hpp"

namespace io::adbms
{

std::expected<void, ErrorCode> owcCells(const OpenWireSwitch owcSwitch)
{
    const uint16_t cmd = (owcSwitch == OpenWireSwitch::EvenChannels) ? (ADCV_BASE | OW0) : (ADCV_BASE | OW1);
    return sendCmd(cmd);
}

}; // namespace io::adbms
