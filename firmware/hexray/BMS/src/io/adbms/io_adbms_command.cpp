#include "io_adbms.hpp"
#include "io_adbms_internal.hpp"

namespace io::adbms::command
{
result<void> startCadc(const bool redundant)
{
    return reversible::sendCmd(ADCV_BASE | CONT | (redundant ? RD : 0U));
}

// Parity uses 0-indexed cell numbering so EVEN sets OW1 (odd channels C1, C3, ...) and ODD sets OW0 (even channels C2,
// C4, ...)
result<void> startSadc(const bool continuous, const OpenWireParity parity)
{
    return reversible::sendCmd(
        ADSV_BASE | (continuous ? CONT : 0U) |
        (parity == OpenWireParity::EVEN  ? OW1
         : parity == OpenWireParity::ODD ? OW0
                                         : 0U));
}

result<void> startAuxadc()
{
    return reversible::sendCmd(ADAX_BASE);
}

result<void> pollSadc()
{
    return reversible::poll(PLSADC);
}

result<void> pollXadc()
{
    return reversible::poll(PLAUX);
}

result<void> snap()
{
    return reversible::sendCmd(SNAP);
}

result<void> unsnap()
{
    return reversible::sendCmd(UNSNAP);
}

result<void> startBalance()
{
    return reversible::sendCmd(UNMUTE);
}

result<void> stopBalance()
{
    return reversible::sendCmd(MUTE);
}
} // namespace io::adbms::command
