#include "io_ltc6813.h"
#include "io_ltc6813_internal.h"
#include <stdint.h>

static const uint16_t cv_read_cmds[VOLTAGE_REGISTER_GROUPS] = { RDCVA, RDCVB, RDCVC, RDCVD, RDCVE };

ExitCode io_ltc6813_clearCellRegisters(void)
{
    return io_ltc6813_sendCommand(CLRCELL);
}

ExitCode io_ltc6813_startCellsAdcConversion(void)
{
    RETURN_IF_ERR(io_ltc6813_clearCellRegisters());
    return io_ltc6813_sendCommand(ADCV);
}

void io_ltc6813_readVoltageRegisters(
    uint16_t cell_voltage_regs[NUM_SEGMENTS][CELLS_PER_SEGMENT],
    ExitCode comm_success[NUM_SEGMENTS][CELLS_PER_SEGMENT])
{
    // Exit early if ADC conversion fails
    const ExitCode poll_ok = io_ltc6813_pollAdcConversions();
    if (IS_EXIT_ERR(poll_ok))
    {
        for (uint8_t i = 0; i < NUM_SEGMENTS; i++)
        {
            for (uint8_t j = 0; j < CELLS_PER_SEGMENT; j++)
            {
                comm_success[i][j] = poll_ok;
            }
        }

        return;
    }

    for (uint8_t reg_group = 0U; reg_group < VOLTAGE_REGISTER_GROUPS; reg_group++)
    {
        uint16_t curr_regs[NUM_SEGMENTS][REGS_PER_GROUP];
        ExitCode curr_success[NUM_SEGMENTS];
        io_ltc6813_readRegGroup(cv_read_cmds[reg_group], curr_regs, curr_success);

        for (uint8_t seg_idx = 0U; seg_idx < NUM_SEGMENTS; seg_idx++)
        {
            for (uint8_t reg_in_group = 0U; reg_in_group < REGS_PER_GROUP; reg_in_group++)
            {
                // Only have 14 cells per segment so ignore the 15th reg reading.
                const int voltage_reg = reg_group * REGS_PER_GROUP + reg_in_group;
                if (voltage_reg >= CELLS_PER_SEGMENT)
                {
                    continue;
                }

                cell_voltage_regs[seg_idx][voltage_reg] = curr_regs[seg_idx][reg_in_group];
                comm_success[seg_idx][voltage_reg]      = curr_success[seg_idx];
            }
        }
    }
}
