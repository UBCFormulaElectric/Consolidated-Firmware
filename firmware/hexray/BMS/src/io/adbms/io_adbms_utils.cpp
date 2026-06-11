#include "io_adbms.hpp"
#include "io_adbms_internal.hpp"
#include "util_errorCodes.hpp"

namespace io::adbms::command
{
result<void> detectBreaks()
{
    // Wake both transceivers and select the drive mode (Forward/Reverse/Dual) based on where, if
    // anywhere, the isoSPI chain is broken. probeTopology() issues its own wake pulses per side.
    probeTopology();
    return {};
}

result<void> startBalance()
{
    return sendCmd(UNMUTE);
}

result<void> stopBalance()
{
    return sendCmd(MUTE);
}
} // namespace io::adbms::command
