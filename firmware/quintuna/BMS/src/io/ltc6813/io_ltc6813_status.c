#include "app_utils.h"
#include "io_ltc6813.h"

#include "io_ltc6813_internal.h"
#include "io_ltc6813_cmds.h"
#include "hw_spis.h"

#include <assert.h>

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
    for (uint8_t i = 0; i < NUM_SEGMENTS; i++)
    {
        success[i] = EXIT_INDETERMINATE;
    }

    const ExitCode poll_ok = io_ltc6813_pollAdcConversions();
    if (IS_EXIT_ERR(poll_ok))
    {
        for (uint8_t i = 0; i < NUM_SEGMENTS; i++)
        {
            success[i] = poll_ok;
        }

        return;
    }

    struct
    {
        StatA stat;
        PEC   pec;
    } reg_stat_a[NUM_SEGMENTS];
    static_assert(sizeof(reg_stat_a) == NUM_SEGMENTS * (sizeof(StatA) + sizeof(PEC)));

    const ltc6813Cmd tx_cmd        = io_ltc6813_buildTxCmd(RDSTATA);
    const ExitCode   stat_a_status = hw_spi_transmitThenReceive(
        &ltc6813_spi_ls, (uint8_t *)&tx_cmd, sizeof(tx_cmd), (uint8_t *)reg_stat_a, sizeof(reg_stat_a));
    if (IS_EXIT_ERR(stat_a_status))
    {
        for (uint8_t i = 0; i < NUM_SEGMENTS; i++)
        {
            success[i] = stat_a_status;
        }
        return;
    }

    struct
    {
        StatB stat;
        PEC   pec;
    } reg_stat_b[NUM_SEGMENTS];
    static_assert(sizeof(reg_stat_b) == NUM_SEGMENTS * (sizeof(StatB) + sizeof(PEC)));

    const ltc6813Cmd tx_cmd_2      = io_ltc6813_buildTxCmd(RDSTATB);
    const ExitCode   stat_b_status = hw_spi_transmitThenReceive(
        &ltc6813_spi_ls, (uint8_t *)&tx_cmd_2, sizeof(tx_cmd_2), (uint8_t *)reg_stat_b, sizeof(reg_stat_b));
    if (IS_EXIT_ERR(stat_b_status))
    {
        for (uint8_t i = 0; i < NUM_SEGMENTS; i++)
        {
            success[i] = stat_b_status;
        }
        return;
    }

    for (int seg = 0; seg < NUM_SEGMENTS; seg++)
    {
        const StatA *reg_a = &reg_stat_a[(NUM_SEGMENTS - 1) - seg].stat;
        const StatB *reg_b = &reg_stat_b[(NUM_SEGMENTS - 1) - seg].stat;

        if (!io_ltc6813_checkPec((uint8_t *)reg_a, sizeof(StatA), &reg_stat_a[(NUM_SEGMENTS - 1) - seg].pec) ||
            !io_ltc6813_checkPec((uint8_t *)reg_b, sizeof(StatB), &reg_stat_b[(NUM_SEGMENTS - 1) - seg].pec))
        {
            success[seg] = EXIT_CODE_CHECKSUM_FAIL;
            continue;
        }

        status[seg].stat_a = *reg_a;
        status[seg].stat_b = *reg_b;
        success[seg]       = EXIT_CODE_OK;
    }
}
