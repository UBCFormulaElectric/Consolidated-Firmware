#include "io_adbms.hpp"
#include "io_adbms_internal.hpp"
#include "util_errorCodes.hpp"

namespace io::adbms::command
{
result<void> wakeup()
{
    return sendCmd(RDCFGA);
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
