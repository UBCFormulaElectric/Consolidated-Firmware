#include "io_ltc6813.h"

#include "io_ltc6813_internal.h"

// OPEN WIRE CHECK
// TODO more configs here probably
ExitCode io_ltc6813CellVoltages_owcPull(const PullDirection pull_direction, const ADCSpeed speed, const bool dcp)
{
// Cell selection for ADC conversion
#define CH (0U)
    const uint16_t MD_SHIFT_7  = (speed & 0x3) << 7;
    const uint16_t DCP_SHIFT_4 = (dcp & 0x1) << 4;
#define ADOW_PU (MD_SHIFT_7 | DCP_SHIFT_4 | CH | 0x0228U)
#define ADOW_PD (MD_SHIFT_7 | DCP_SHIFT_4 | CH | 0x0268U)
    const uint16_t cmd = pull_direction == PULL_UP ? ADOW_PU : ADOW_PD;
    return io_ltc6813_sendCommand(cmd);
}
