#include "io_ltc6813.h"

#include "io_ltc6813_internal.h"
#include "hw_spis.h"

#include <assert.h>
#include <io_log.h>
#include <string.h>

ExitCode io_ltc6813_clearAuxRegisters()
{
#define CLRAUX (0x0712)
    return io_ltc6813_sendCommand(CLRAUX);
}

ExitCode io_ltc6813_startThermistorsAdcConversion(const ADCSpeed speed)
{
    RETURN_IF_ERR(io_ltc6813_clearAuxRegisters());
    const uint16_t adc_speed_factor = (speed & 0x3) << 7;
// GPIO Selection for ADC conversion
#define CHG (0x000U)
#define ADAX (0x460 | adc_speed_factor | CHG)
    return io_ltc6813_sendCommand(ADAX);
}

// as per table 46-48
typedef struct __attribute__((__packed__))
{
    uint16_t a;
    uint16_t b;
    uint16_t c;
    PEC      pec;
} AuxRegGroup;
static_assert(sizeof(AuxRegGroup) == REGISTER_GROUP_SIZE + PEC_SIZE);

void io_ltc6813_readAuxRegisters(
    uint16_t aux_regs[NUM_SEGMENTS][AUX_REGS_PER_SEGMENT],
    ExitCode comm_success[NUM_SEGMENTS][AUX_REG_GROUPS])
{
    for (uint8_t i = 0; i < NUM_SEGMENTS; i++)
        for (uint8_t j = 0; j < AUX_REG_GROUPS; j++)
            comm_success[i][j] = EXIT_INDETERMINATE;
    memset(aux_regs, 0, NUM_SEGMENTS * AUX_REGS_PER_SEGMENT * sizeof(uint16_t));

    const ExitCode poll_ok = io_ltc6813_pollAdcConversions();
    if (IS_EXIT_ERR(poll_ok))
    {
        for (uint8_t i = 0; i < NUM_SEGMENTS; i++)
            for (uint8_t j = 0; j < AUX_REG_GROUPS; j++)
                comm_success[i][j] = poll_ok;
    }

    // Read thermistor voltages stored in the AUX register groups
    for (uint8_t reg_group = 0U; reg_group < AUX_REG_GROUPS; reg_group++)
    {
        // pack command
#define RDAUXA (0x000CU)
#define RDAUXB (0x000EU)
#define RDAUXC (0x000DU)
        static const uint16_t aux_reg_group_cmds[3] = { RDAUXA, RDAUXB, RDAUXC };
        const ltc6813_tx      tx_cmd                = io_ltc6813_build_tx_cmd(aux_reg_group_cmds[reg_group]);

        AuxRegGroup rx_buffer[NUM_SEGMENTS];

        // send command and receive data
        const ExitCode tx_status = hw_spi_transmitThenReceive(
            &ltc6813_spi_ls, (uint8_t *)&tx_cmd, sizeof(tx_cmd), (uint8_t *)rx_buffer, sizeof(rx_buffer));
        if (IS_EXIT_ERR(tx_status))
        {
            for (uint8_t segment = 0; segment < NUM_SEGMENTS; segment++)
                comm_success[segment][reg_group] = tx_status;
            continue;
        }

        // process data
        for (uint8_t seg_idx = 0U; seg_idx < NUM_SEGMENTS; seg_idx++)
        {
            // look for data for the current segment from the back
            const AuxRegGroup *seg_reg_group = &rx_buffer[(NUM_SEGMENTS - 1) - seg_idx];
            if (!io_ltc6813_check_pec((uint8_t *)seg_reg_group, 6, &seg_reg_group->pec))
            {
                comm_success[seg_idx][reg_group] = EXIT_CODE_CHECKSUM_FAIL;
                continue;
            }

            comm_success[seg_idx][reg_group]     = EXIT_CODE_OK;
            aux_regs[seg_idx][reg_group * 3 + 0] = seg_reg_group->a;
            aux_regs[seg_idx][reg_group * 3 + 1] = seg_reg_group->b;
            aux_regs[seg_idx][reg_group * 3 + 2] = seg_reg_group->c;
        }
    }
}