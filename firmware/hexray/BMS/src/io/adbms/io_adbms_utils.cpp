#include "io_adbms.hpp"
#include "io_adbms_internal.hpp"
#include "util_errorCodes.hpp"

namespace io::adbms
{
result<void> command::wakeup()
{
    return sendCmd(RDCFGA);
}

result<void> command::startBalance()
{
    return sendCmd(UNMUTE);
}

result<void> command::stopBalance()
{
    return sendCmd(MUTE);
}

result<void> command::owcCells(const OpenWireSwitch owcSwitch)
{
    const uint16_t cmd = (owcSwitch == OpenWireSwitch::EvenChannels) ? (ADCV_BASE | OW0) : (ADCV_BASE | OW1);
    return sendCmd(cmd);
}
} // namespace io::adbms
