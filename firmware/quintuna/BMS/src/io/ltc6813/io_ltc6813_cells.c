#include "io_ltc6813.h"

#include <string.h>

#include "io_ltc6813_cmds.h"
#include "io_ltc6813_internal.h"
#include "hw_spis.h"

// as per table 40-45
typedef struct __attribute__((__packed__))
{
    uint16_t a;
    uint16_t b;
    uint16_t c;
    PEC      pec;
} VoltageRegGroup;
static_assert(sizeof(VoltageRegGroup) == REGISTER_GROUP_SIZE + PEC_SIZE);

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
    ExitCode comm_success[NUM_SEGMENTS][VOLTAGE_REGISTER_GROUPS])
{
    memset(cell_voltage_regs, 0, NUM_SEGMENTS * CELLS_PER_SEGMENT * sizeof(uint16_t));
    for (uint8_t i = 0; i < NUM_SEGMENTS; i++)
    {
        for (uint8_t j = 0; j < VOLTAGE_REGISTER_GROUPS; j++)
        {
            comm_success[i][j] = EXIT_INDETERMINATE;
        }
    }

    // Exit early if ADC conversion fails
    const ExitCode poll_ok = io_ltc6813_pollAdcConversions();
    if (IS_EXIT_ERR(poll_ok))
    {
        for (uint8_t i = 0; i < NUM_SEGMENTS; i++)
        {
            for (uint8_t j = 0; j < VOLTAGE_REGISTER_GROUPS; j++)
            {
                comm_success[i][j] = poll_ok;
            }
        }

        return;
    }

    for (uint8_t reg_group = 0U; reg_group < VOLTAGE_REGISTER_GROUPS; reg_group++)
    {
        static const uint16_t cv_read_cmds[VOLTAGE_REGISTER_GROUPS] = { RDCVA, RDCVB, RDCVC, RDCVD, RDCVE };

        // Transmit the command and receive data stored in register group.
        const ltc6813Cmd tx_cmd = io_ltc6813_buildTxCmd(cv_read_cmds[reg_group]);
        VoltageRegGroup  rx_buffer[NUM_SEGMENTS];

        const ExitCode read_vreg_group_ok = hw_spi_transmitThenReceive(
            &ltc6813_spi_ls, (uint8_t *)&tx_cmd, sizeof(tx_cmd), (uint8_t *)rx_buffer, sizeof(rx_buffer));
        if (IS_EXIT_ERR(read_vreg_group_ok))
        {
            for (uint8_t seg_idx = 0U; seg_idx < NUM_SEGMENTS; seg_idx++)
            {
                comm_success[seg_idx][reg_group] = read_vreg_group_ok;
            }

            continue;
        }

        for (uint8_t seg_idx = 0U; seg_idx < NUM_SEGMENTS; seg_idx++)
        {
            // Calculate PEC15 from the data received on rx_buffer
            const VoltageRegGroup *seg_reg_group = &rx_buffer[(NUM_SEGMENTS - 1) - seg_idx];
            if (!io_ltc6813_checkPec((uint8_t *)seg_reg_group, 6, &seg_reg_group->pec))
            {
                comm_success[seg_idx][reg_group] = EXIT_CODE_CHECKSUM_FAIL;
                continue;
            }

            // Conversion factor used to convert raw voltages (100µV) to voltages (V)
            cell_voltage_regs[seg_idx][reg_group * REGS_PER_GROUP + 0] = seg_reg_group->a;
            cell_voltage_regs[seg_idx][reg_group * REGS_PER_GROUP + 1] = seg_reg_group->b;

            // Only have 14 cells per segment so ignore the 15th reg reading.
            if (reg_group * REGS_PER_GROUP + 2 < CELLS_PER_SEGMENT)
            {
                cell_voltage_regs[seg_idx][reg_group * REGS_PER_GROUP + 2] = seg_reg_group->c;
            }

            comm_success[seg_idx][reg_group] = EXIT_CODE_OK;
        }
    }
}
