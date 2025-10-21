#include "io/ltc6813.h"

#include "io/ltc6813/ltc6813_internal.h"

// OPEN WIRE CHECK
// TODO more configs here probably
// TODO: Need to set the ADCOPT bit to 1 in the config register!
ExitCode io_ltc6813_owcPull(const PullDirection pull_direction)
{
    // Cell selection for ADC conversion
    const uint16_t cmd = pull_direction == PULL_UP ? ADOW_PU : ADOW_PD;
    return io_ltc6813_sendCommand(cmd);
}
