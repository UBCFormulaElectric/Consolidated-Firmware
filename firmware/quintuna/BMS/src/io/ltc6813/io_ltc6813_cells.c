#include "io_ltc6813.h"

#include "io_ltc6813_internal.h"
#include "hw_spis.h"

#include <assert.h>
#include <string.h>

// TODO assert that for each speed that the ADCOPT is correct
ExitCode io_ltc6813_startCellsAdcConversion(const ADCSpeed speed)
{
    RETURN_IF_ERR(io_ltc6813_clearCellRegisters());
    const uint16_t adc_speed_factor = (speed & 0x3) << 7;
// Cell selection for ADC conversion
#define CH (000U)
// Discharge permitted
#define DCP (0U)
#define ADCV (0x260 | CH | DCP << 4 | adc_speed_factor)
    return io_ltc6813_sendCommand(ADCV);
}

// as per table 40-45
typedef struct __attribute__((__packed__))
{
    uint16_t a;
    uint16_t b;
    uint16_t c;
    PEC      pec;
} VoltageRegGroup;
static_assert(sizeof(VoltageRegGroup) == REGISTER_GROUP_SIZE + PEC_SIZE);

void io_ltc6813_readVoltageRegisters(
    uint16_t cell_voltage_regs[NUM_SEGMENTS][CELLS_PER_SEGMENT],
    ExitCode comm_success[NUM_SEGMENTS][VOLTAGE_REGISTER_GROUPS])
{
    for (uint8_t i = 0; i < NUM_SEGMENTS; i++)
        for (uint8_t j = 0; j < VOLTAGE_REGISTER_GROUPS; j++)
            comm_success[i][j] = EXIT_INDETERMINATE;
    memset(cell_voltage_regs, 0, NUM_SEGMENTS * CELLS_PER_SEGMENT * sizeof(uint16_t));
    // Exit early if ADC conversion fails
    const ExitCode poll_ok = io_ltc6813_pollAdcConversions();
    if (IS_EXIT_ERR(poll_ok))
    {
        for (uint8_t i = 0; i < NUM_SEGMENTS; i++)
            for (uint8_t j = 0; j < VOLTAGE_REGISTER_GROUPS; j++)
                comm_success[i][j] = poll_ok;
        return;
    }

    for (uint8_t reg_group = 0U; reg_group < VOLTAGE_REGISTER_GROUPS; reg_group++)
    {
        // maps the register group number to the command to read that register group
#define RDCVA (0x04U)
#define RDCVB (0x06U)
#define RDCVC (0x08U)
#define RDCVD (0x0AU)
#define RDCVE (0x09U)
#define RDCVF (0x0BU)
        static const uint16_t cv_read_cmds[6] = { RDCVA, RDCVB, RDCVC, RDCVD, RDCVE, RDCVF };

        // Transmit the command and receive data stored in register group.
        const ltc6813_tx tx_cmd = io_ltc6813_build_tx_cmd(cv_read_cmds[reg_group]);
        VoltageRegGroup  rx_buffer[NUM_SEGMENTS];

        const ExitCode read_vreg_group_ok = hw_spi_transmitThenReceive(
            &ltc6813_spi_ls, (uint8_t *)&tx_cmd, sizeof(tx_cmd), (uint8_t *)rx_buffer, sizeof(rx_buffer));
        if (IS_EXIT_ERR(read_vreg_group_ok))
        {
            for (uint8_t seg_idx = 0U; seg_idx < NUM_SEGMENTS; seg_idx++)
                comm_success[seg_idx][reg_group] = read_vreg_group_ok;
            return;
        }

        for (uint8_t seg_idx = 0U; seg_idx < NUM_SEGMENTS; seg_idx++)
        {
            const VoltageRegGroup *seg_reg_group = &rx_buffer[(NUM_SEGMENTS - 1) - seg_idx];
            // Calculate PEC15 from the data received on rx_buffer
            if (!io_ltc6813_check_pec((uint8_t *)seg_reg_group, 6, &seg_reg_group->pec))
            {
                comm_success[seg_idx][reg_group] = EXIT_CODE_ERROR;
                continue;
            }
            // fuck it we already here
            comm_success[seg_idx][reg_group] = EXIT_CODE_OK;

            // Conversion factor used to convert raw voltages (100µV) to voltages (V)
            cell_voltage_regs[seg_idx][reg_group * 3 + 0] = seg_reg_group->a;
            cell_voltage_regs[seg_idx][reg_group * 3 + 1] = seg_reg_group->b;
            if (reg_group * 3 + 2 >= 14) // TODO find a more elegant stopping condition
                continue;
            cell_voltage_regs[seg_idx][reg_group * 3 + 2] = seg_reg_group->c;
        }
    }
}