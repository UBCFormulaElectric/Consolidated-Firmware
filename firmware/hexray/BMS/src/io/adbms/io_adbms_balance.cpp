#include "io_adbms.hpp"
#include "io_adbms_internal.hpp"

namespace io::adbms
{
ExitCode sendBalanceCommand(void)
{
    return io::adbms::sendCommand(io::adbms::UNMUTE);
}

ExitCode sendStopBalanceCommand(void)
{
    return io::adbms::sendCommand(io::adbms::MUTE);
}
} // namespace io::adbms