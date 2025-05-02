#include "io_ltc6813.h"

#include "io_ltc6813_internal.h"

// BALANCING COMMANDS
ExitCode io_ltc6813_sendBalanceCommand(void)
{
#define UNMUTE (0x0029U)
    return io_ltc6813_sendCommand(UNMUTE);
}

ExitCode io_ltc6813_sendStopBalanceCommand(void)
{
#define MUTE (0x0028U)
    return io_ltc6813_sendCommand(MUTE);
}
