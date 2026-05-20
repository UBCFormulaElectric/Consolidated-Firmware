#include "io_adbms.hpp"
#include "io_adbms_internal.hpp"
#include "util_errorCodes.hpp"

namespace io::adbms
{
result<void> command::wakeup()
{
    return sendCmd(RDCFGA);
}

result<void> command::startSegAdcConversion()
{
    return sendCmd(ADSV_BASE);
}

result<void> command::sendBalanceCmd()
{
    return sendCmd(UNMUTE);
}

result<void> command::sendStopBalanceCmd()
{
    return sendCmd(MUTE);
}

result<void> command::owcCells(const OpenWireSwitch owcSwitch)
{
    const uint16_t cmd = (owcSwitch == OpenWireSwitch::EvenChannels) ? (ADCV_BASE | OW0) : (ADCV_BASE | OW1);
    return sendCmd(cmd);
}
} // namespace io::adbms
