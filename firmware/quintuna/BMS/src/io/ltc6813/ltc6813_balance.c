#include "io/ltc6813.h"
#include "io/ltc6813_internal.h"

ExitCode io_ltc6813_sendBalanceCommand(void)
{
    return io_ltc6813_sendCommand(UNMUTE);
}

ExitCode io_ltc6813_sendStopBalanceCommand(void)
{
    return io_ltc6813_sendCommand(MUTE);
}
