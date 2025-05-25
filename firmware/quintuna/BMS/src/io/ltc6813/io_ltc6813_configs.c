#include "io_ltc6813.h"

#include <string.h>

#include "io_ltc6813_internal.h"
#include "io_ltc6813_cmds.h"
#include "hw_spis.h"

typedef struct __attribute__((__packed__))
{
    CFGAR config;
    PEC   pec;
} CFGAR_msg;
static_assert(sizeof(CFGAR) == REGISTER_GROUP_SIZE);
static_assert(sizeof(CFGAR_msg) == REGISTER_GROUP_SIZE + sizeof(PEC));

typedef struct __attribute__((__packed__))
{
    CFGBR config;
    PEC   pec;
} CFGBR_msg;
static_assert(sizeof(CFGBR) == REGISTER_GROUP_SIZE);
static_assert(sizeof(CFGBR_msg) == REGISTER_GROUP_SIZE + sizeof(PEC));

/**
 * @param config Configuration for the LTC
 * @attention For more information on how to configure the LTC, see table 56
 * @return Success if both succeeded. Fail if at least one failed.
 */
ExitCode io_ltc6813_writeConfigurationRegisters(const SegmentConfig config[NUM_SEGMENTS])
{
    // as per table 33
    struct __attribute__((__packed__))
    {
        ltc6813Cmd cmd;
        CFGAR_msg  segment_configs[NUM_SEGMENTS]; // note these must be shifted in backwards (shift register style)
    } tx_msg_a = { 0 };
    static_assert(sizeof(tx_msg_a) == 4 + (sizeof(CFGAR) + sizeof(PEC)) * NUM_SEGMENTS);
    memset(&tx_msg_a, 0, sizeof(tx_msg_a));
    tx_msg_a.cmd = io_ltc6813_buildTxCmd(WRCFGA);

    // as per table 33`
    struct __attribute__((__packed__))
    {
        ltc6813Cmd cmd;
        CFGBR_msg  segment_configs[NUM_SEGMENTS];
    } tx_msg_b = { 0 };
    static_assert(sizeof(tx_msg_b) == 4 + (sizeof(CFGBR) + sizeof(PEC)) * NUM_SEGMENTS);
    memset(&tx_msg_b, 0, sizeof(tx_msg_b));
    tx_msg_b.cmd = io_ltc6813_buildTxCmd(WRCFGB);

    for (uint8_t curr_segment = 0U; curr_segment < NUM_SEGMENTS; curr_segment++)
    {
        // Data used to configure the last segment on the daisy chain needs to be sent first
        const uint8_t    seg_idx = NUM_SEGMENTS - curr_segment - 1;
        CFGAR_msg *const seg_a   = &tx_msg_a.segment_configs[seg_idx];
        CFGBR_msg *const seg_b   = &tx_msg_b.segment_configs[seg_idx];
        seg_a->config            = config[seg_idx].reg_a;
        seg_b->config            = config[seg_idx].reg_b;

        // Calculate and pack the PEC15 bytes into data to write to the configuration register
        seg_a->pec = io_ltc6813_buildDataPec((uint8_t *)&seg_a->config, sizeof(CFGAR));
        seg_b->pec = io_ltc6813_buildDataPec((uint8_t *)&seg_b->config, sizeof(CFGBR));
    }

    // Write to configuration registers
    RETURN_IF_ERR(hw_spi_transmit(&ltc6813_spi_ls, (uint8_t *)&tx_msg_a, sizeof(tx_msg_a)))
    RETURN_IF_ERR(hw_spi_transmit(&ltc6813_spi_ls, (uint8_t *)&tx_msg_b, sizeof(tx_msg_b)))
    return EXIT_CODE_OK;
}

void io_ltc6813_readConfigurationRegisters(SegmentConfig configs[NUM_SEGMENTS], ExitCode success[NUM_SEGMENTS])
{
    ltc6813Cmd tx_msg_a = io_ltc6813_buildTxCmd(RDCFGA);
    CFGAR_msg  rx_buf_a[NUM_SEGMENTS];
    ltc6813Cmd tx_msg_b = io_ltc6813_buildTxCmd(RDCFGB);
    CFGBR_msg  rx_buf_b[NUM_SEGMENTS];

    const ExitCode com1 = hw_spi_transmitThenReceive(
        &ltc6813_spi_ls, (uint8_t *)&tx_msg_a, sizeof(tx_msg_a), (uint8_t *)rx_buf_a, sizeof(rx_buf_a));
    if (IS_EXIT_ERR(com1))
    {
        for (uint8_t s = 0U; s < NUM_SEGMENTS; s++)
            success[s] = com1;
        return;
    }

    const ExitCode com2 = hw_spi_transmitThenReceive(
        &ltc6813_spi_ls, (uint8_t *)&tx_msg_b, sizeof(tx_msg_b), (uint8_t *)rx_buf_b, sizeof(rx_buf_b));
    if (IS_EXIT_ERR(com2))
    {
        for (uint8_t s = 0U; s < NUM_SEGMENTS; s++)
            success[s] = com2;
        return;
    }

    for (uint8_t s = 0U; s < NUM_SEGMENTS; s++)
    {
        const uint8_t seg_idx = (NUM_SEGMENTS - 1) - s;
        const bool    chk1    = io_ltc6813_checkPec(
            (uint8_t *)&rx_buf_a[seg_idx].config, sizeof(rx_buf_a[seg_idx].config), &rx_buf_a[seg_idx].pec);
        const bool chk2 = io_ltc6813_checkPec(
            (uint8_t *)&rx_buf_b[seg_idx].config, sizeof(rx_buf_b[seg_idx].config), &rx_buf_b[seg_idx].pec);
        success[seg_idx] = chk1 && chk2 ? EXIT_CODE_OK : EXIT_CODE_CHECKSUM_FAIL;
    }

    for (uint8_t s = 0U; s < NUM_SEGMENTS; s++)
    {
        const uint8_t seg_idx  = (NUM_SEGMENTS - 1) - s;
        configs[seg_idx].reg_a = rx_buf_a[seg_idx].config;
        configs[seg_idx].reg_b = rx_buf_b[seg_idx].config;
    }
}
