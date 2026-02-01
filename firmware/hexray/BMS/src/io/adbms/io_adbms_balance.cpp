#include "io_adbms.hpp"
#include "io_adbms_internal.hpp"

namespace io::adbms
{
ExitCode sendBalanceCmd()
{
    return sendCmd(UNMUTE);
}

ExitCode sendStopBalanceCmd()
{
    return sendCmd(MUTE);
}
} // namespace io::adbms
