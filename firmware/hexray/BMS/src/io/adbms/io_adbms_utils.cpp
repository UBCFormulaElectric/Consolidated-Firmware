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
} // namespace io::adbms
