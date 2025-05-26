#include "io_ltc6813.h"
#include "io_ltc6813_internal.h"

#include <assert.h>
#include <io_log.h>
#include <stdint.h>

static const uint16_t aux_reg_groups[AUX_REG_GROUPS] = { RDAUXA, RDAUXB, RDAUXC };

ExitCode io_ltc6813_clearAuxRegisters(void)
{
    return io_ltc6813_sendCommand(CLRAUX);
}

ExitCode io_ltc6813_startThermistorsAdcConversion(void)
{
    RETURN_IF_ERR(io_ltc6813_clearAuxRegisters());
    return io_ltc6813_sendCommand(ADAX);
}

void io_ltc6813_readAuxRegisters(
    uint16_t aux_regs[NUM_SEGMENTS][AUX_REGS_PER_SEGMENT],
    ExitCode comm_success[NUM_SEGMENTS][AUX_REGS_PER_SEGMENT])
{
    // Exit early if ADC conversion fails
    const ExitCode poll_ok = io_ltc6813_pollAdcConversions();
    if (IS_EXIT_ERR(poll_ok))
    {
        for (uint8_t i = 0; i < NUM_SEGMENTS; i++)
        {
            for (uint8_t j = 0; j < AUX_REG_GROUPS; j++)
            {
                comm_success[i][j] = poll_ok;
            }
        }

        return;
    }

    // Read thermistor voltages stored in the AUX register groups
    for (uint8_t reg_group = 0U; reg_group < AUX_REG_GROUPS; reg_group++)
    {
        uint16_t curr_regs[NUM_SEGMENTS][REGS_PER_GROUP];
        ExitCode curr_success[NUM_SEGMENTS];
        io_ltc6813_readRegGroup(aux_reg_groups[reg_group], curr_regs, curr_success);

        for (uint8_t seg_idx = 0U; seg_idx < NUM_SEGMENTS; seg_idx++)
        {
            for (uint8_t reg_in_group = 0U; reg_in_group < REGS_PER_GROUP; reg_in_group++)
            {
                const int aux_reg              = reg_group * REGS_PER_GROUP + reg_in_group;
                aux_regs[seg_idx][aux_reg]     = curr_regs[seg_idx][reg_in_group];
                comm_success[seg_idx][aux_reg] = curr_success[seg_idx];
            }
        }
    }
}
