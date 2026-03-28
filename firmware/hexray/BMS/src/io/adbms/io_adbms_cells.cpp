#include "io_adbms_internal.hpp"
#include <stdint.h>

namespace io::adbms
{
static constexpr uint16_t cv_read_cmds[VOLTAGE_REGISTER_GROUPS] = { RDCVA, RDCVB, RDCVC, RDCVD, RDCVE };

ExitCode clearCellRegisters(void)
{
    return sendCommand(io::adbms::CLRCELL);
}

ExitCode startCellsAdcConversion(void)
{
    RETURN_IF_ERR(clearCellRegisters());
    return io::adbms::sendCommand(io::adbms::C_ADC);
}

void readVoltageRegisters(
    uint16_t cell_voltage_regs[io::NUM_SEGMENTS][io::CELLS_PER_SEGMENTS],
    ExitCode comm_success[io::NUM_SEGMENTS][io::CELLS_PER_SEGMENTS])
{
    // Exit early if ADC conversion fails
    const ExitCode poll_ok = io::adbms::pollAdcConversions();
    if (IS_EXIT_ERR(poll_ok))
    {
        for (uint8_t i = 0; i < io::NUM_SEGMENTS; i++)
        {
            for (uint8_t j = 0; j < io::CELLS_PER_SEGMENTS; j++)
            {
                comm_success[i][j] = poll_ok;
            }
        }

        return;
    }

    for (uint8_t reg_group = 0U; reg_group < io::adbms::VOLTAGE_REGISTER_GROUPS; reg_group++)
    {
        uint16_t curr_regs[io::NUM_SEGMENTS][io::adbms::REGS_PER_GROUP];
        ExitCode curr_success[io::NUM_SEGMENTS];
        io::adbms::readRegGroup(io::adbms::cv_read_cmds[reg_group], curr_regs, curr_success);

        for (uint8_t seg_idx = 0U; seg_idx < io::NUM_SEGMENTS; seg_idx++)
        {
            for (uint8_t reg_in_group = 0U; reg_in_group < io::adbms::REGS_PER_GROUP; reg_in_group++)
            {
                // Only have 14 cells per segment so ignore the 15th reg reading.
                const int voltage_reg = reg_group * io::adbms::REGS_PER_GROUP + reg_in_group;
                if (voltage_reg >= io::CELLS_PER_SEGMENTS)
                {
                    continue;
                }

                cell_voltage_regs[seg_idx][voltage_reg] = curr_regs[seg_idx][reg_in_group];
                comm_success[seg_idx][voltage_reg]      = curr_success[seg_idx];
            }
        }
    }
}
} // namespace io::adbms