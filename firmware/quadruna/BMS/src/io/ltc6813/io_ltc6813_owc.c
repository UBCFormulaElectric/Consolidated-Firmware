#include "io_ltc6813.h"

#include "io_ltc6813_internal.h"

// OPEN WIRE CHECK
// TODO more configs here probably
bool io_ltc6813CellVoltages_owcPull(const PullDirection pull_direction)
{
// ADOW mode selection
#define PUP_PU (1U) // Pull-up current
#define PUP_PD (0U) // Pull-down current

// Discharge permitted
#define DCP (0U)

// Cell selection for ADC conversion
#define CH_OWC (0U)

#define MD (0x11U)

// TODO make backwards
#define ADOW_PU ((uint16_t)(((0x028U + (MD << 7U) + (PUP_PU << 6U) + (DCP << 4U) + CH_OWC) << 8U) | 0x0003U))
#define ADOW_PD ((uint16_t)(((0x028U + (MD << 7U) + (PUP_PD << 6U) + (DCP << 4U) + CH_OWC) << 8U) | 0x0003U))

    return pull_direction == PULL_UP ? io_ltc6813_sendCommand(ADOW_PU) : io_ltc6813_sendCommand(ADOW_PD);
}
