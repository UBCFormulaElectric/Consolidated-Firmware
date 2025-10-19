#include "utils.h"
#include "io_ltc6813.h"

#include "io_ltc6813_internal.h"

#include <assert.h>
#include <string.h>

ExitCode io_ltc6813_clearStatRegisters(void)
{
    return io_ltc6813_sendCommand(CLRSTAT);
}

ExitCode io_ltc6813_startInternalADCConversions(void)
{
    return io_ltc6813_sendCommand(ADSTAT);
}

void io_ltc6813_getStatus(StatusRegGroups status[NUM_SEGMENTS], ExitCode success[NUM_SEGMENTS])
{
    memset(status, 0, sizeof(StatusRegGroups) * NUM_SEGMENTS);

    // Exit early if ADC conversion fails
    const ExitCode poll_ok = io_ltc6813_pollAdcConversions();
    if (IS_EXIT_ERR(poll_ok))
    {
        for (uint8_t i = 0; i < NUM_SEGMENTS; i++)
        {
            for (uint8_t j = 0; j < CELLS_PER_SEGMENT; j++)
            {
                success[i] = poll_ok;
            }
        }

        return;
    }

    uint16_t regs_a[NUM_SEGMENTS][REGS_PER_GROUP];
    ExitCode success_a[NUM_SEGMENTS];
    io_ltc6813_readRegGroup(RDSTATA, regs_a, success_a);

    uint16_t regs_b[NUM_SEGMENTS][REGS_PER_GROUP];
    ExitCode success_b[NUM_SEGMENTS];
    io_ltc6813_readRegGroup(RDSTATB, regs_b, success_b);

    for (uint8_t seg_idx = 0U; seg_idx < NUM_SEGMENTS; seg_idx++)
    {
        if (IS_EXIT_ERR(success_a[seg_idx]))
        {
            success[seg_idx] = success_a[seg_idx];
            continue;
        }
        else if (IS_EXIT_ERR(success_b[seg_idx]))
        {
            success[seg_idx] = success_b[seg_idx];
            continue;
        }

        memcpy(&status[seg_idx].stat_a, regs_a[seg_idx], sizeof(STATA));
        memcpy(&status[seg_idx].stat_b, regs_b[seg_idx], sizeof(STATB));
        success[seg_idx] = EXIT_CODE_OK;
    }
}
