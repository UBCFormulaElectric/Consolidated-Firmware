#include "io_adbms.hpp"
#include "io_adbms_internal.hpp"

ExitCode io_adbms_sendBalanceCommand(void)
{
    return io_adbms_sendCommand(io::UNMUTE);
}

ExitCode io_adbms_sendStopBalanceCommand(void)
{
    return io_adbms_sendCommand(io::MUTE);
}