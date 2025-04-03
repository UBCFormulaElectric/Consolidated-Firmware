// TODO: make it so that configs are stored here, and are enforced
#include "io_ltc6813.h"

#include "io_ltc6813_internal.h"
#include "hw_spis.h"

#include <stdint.h>
#include <assert.h>
#include <string.h>

// as per table 38
typedef struct __attribute__((__packed__))
{
    // byte 1
    uint8_t adcopt : 1;
    uint8_t dten : 1;
    uint8_t refon : 1;
    uint8_t gpio_1_5 : 5;
    // byte 2
    uint8_t vuv_0_7;
    // byte 3
    uint8_t vuv_8_11 : 4;
    uint8_t vov_0_3 : 4;
    // byte 4
    uint8_t vov_4_11;
    // byte 5
    uint8_t dcc_1_8;
    // byte 6
    uint8_t dcc_9_12 : 4;
    uint8_t dcto : 4;
    // byte 7/8
    PEC pec;
} CFGAR;

// as per table 39
typedef struct __attribute__((__packed__))
{
    // byte 1
    uint8_t gpio_6_9 : 4;
    uint8_t dcc_13_16 : 4;
    // byte 2
    uint8_t dcc_17 : 1;
    uint8_t dcc_18 : 1;
    uint8_t dcc_0 : 1;
    uint8_t dtmen : 1;
    uint8_t ps : 2;
    uint8_t fdrf : 1;
    uint8_t mute : 1;
    // byte 3-6
    uint32_t reserved;
    // byte 7/8
    PEC pec;
} CFGBR;

/**
 * @param balance_config Configuration for the balancing
 * @attention For more information on how to configure the LTC, see table 56
 * @return Success if both succeeded. Fail if at least one failed.
 */
bool io_ltc6813_writeConfigurationRegisters(bool balance_config[NUM_SEGMENTS][CELLS_PER_SEGMENT])
{
    // as per table 33
    struct __attribute__((__packed__))
    {
        ltc6813_tx cmd;
        CFGAR      segment_configs[NUM_SEGMENTS]; // note these must be shifted in backwards (shift register style)
    } tx_msg_a = { 0 };
    static_assert(sizeof(tx_msg_a) == 4 + 8 * NUM_SEGMENTS);

    // as per table 33`
    struct __attribute__((__packed__))
    {
        ltc6813_tx cmd;
        CFGBR      segment_configs[NUM_SEGMENTS];
    } tx_msg_b = { 0 };
    static_assert(sizeof(tx_msg_b) == 4 + 8 * NUM_SEGMENTS);

    // just in case
    memset(&tx_msg_a, 0, sizeof(tx_msg_a));
    memset(&tx_msg_b, 0, sizeof(tx_msg_b));

#define WRCFGA (0x0001)
#define WRCFGB (0x0024)
    tx_msg_a.cmd = io_ltc6813_build_tx_cmd(WRCFGA);
    tx_msg_b.cmd = io_ltc6813_build_tx_cmd(WRCFGB);

    for (uint8_t curr_segment = 0U; curr_segment < NUM_SEGMENTS; curr_segment++)
    {
        // Data used to configure the last segment on the daisy chain needs to be sent first
        const uint8_t tx_cfg_idx = NUM_SEGMENTS - curr_segment - 1;
        CFGAR *const  seg_a      = &tx_msg_a.segment_configs[tx_cfg_idx];
        CFGBR *const  seg_b      = &tx_msg_b.segment_configs[tx_cfg_idx];

        seg_a->gpio_1_5 = 0x1F;
        seg_b->gpio_6_9 = 0xF;
        seg_a->refon    = 0;
        seg_a->dten     = 0;
        seg_a->adcopt   = 1;

#define VUV (0x4E1U) // Under-voltage comparison voltage, (VUV + 1) * 16 * 100uV
        seg_a->vuv_0_7  = VUV & 0xFF;
        seg_a->vuv_8_11 = VUV >> 8 & 0xF;
#define VOV (0x8CAU) // Over-voltage comparison voltage, VOV * 16 * 100uV
        seg_a->vov_0_3  = VOV & 0xF;
        seg_a->vov_4_11 = VOV >> 4 & 0xFF;

        // Write to configuration registers DCC bits
        if (balance_config != NULL)
        {
            uint32_t dcc_bits = 0U;
            // Get dcc bits to write for the current segment (which cells to balance)
            for (uint8_t cell = 0; cell < CELLS_PER_SEGMENT; cell++)
            {
                dcc_bits |= (uint32_t)(balance_config[curr_segment][cell] << cell);
            }
            seg_b->dcc_0     = 0;
            seg_a->dcc_1_8   = dcc_bits & 0xFF;
            seg_a->dcc_9_12  = dcc_bits >> 8 & 0xF;
            seg_b->dcc_13_16 = dcc_bits >> 12 & 0xF;
            seg_b->dcc_17    = 0;
            seg_b->dcc_18    = 0;
        }
        else
        {
            seg_b->dcc_0     = 0;
            seg_a->dcc_1_8   = 0;
            seg_a->dcc_9_12  = 0;
            seg_b->dcc_13_16 = 0;
            seg_b->dcc_17    = 0;
            seg_b->dcc_18    = 0;
        }

        // Calculate and pack the PEC15 bytes into data to write to the configuration register
        seg_a->pec = io_ltc6813_build_data_pec((uint8_t *)seg_a, sizeof(CFGAR) - sizeof(PEC));
        seg_b->pec = io_ltc6813_build_data_pec((uint8_t *)seg_b, sizeof(CFGBR) - sizeof(PEC));
    }
    // Write to configuration registers
    if (!hw_spi_transmit(&ltc6813_spi, (uint8_t *)&tx_msg_a, sizeof(tx_msg_a)))
        return false;
    if (!hw_spi_transmit(&ltc6813_spi, (uint8_t *)&tx_msg_b, sizeof(tx_msg_b)))
        return false;
    return true;
}

bool io_ltc6813_readConfigurationRegisters()
{
#define RDCFGA (0x0002)
#define RDCFGB (0x0026)
    ltc6813_tx tx_msg_a = io_ltc6813_build_tx_cmd(RDCFGA);
    CFGAR      rx_buf_a[NUM_SEGMENTS];
    if (!hw_spi_transmitThenReceive(
            &ltc6813_spi, (uint8_t *)&tx_msg_a, sizeof(tx_msg_a), (uint8_t *)&rx_buf_a, sizeof(rx_buf_a)))
    {
        return false;
    }

    for (uint8_t curr_segment = 0U; curr_segment < NUM_SEGMENTS; curr_segment++)
    {
        assert(io_ltc6813_check_pec((uint8_t *)&rx_buf_a[curr_segment], 6, &rx_buf_a[curr_segment].pec));
    }

    ltc6813_tx tx_msg_b = io_ltc6813_build_tx_cmd(RDCFGA);
    CFGBR      rx_buf_b[NUM_SEGMENTS];
    if (!hw_spi_transmitThenReceive(
            &ltc6813_spi, (uint8_t *)&tx_msg_b, sizeof(tx_msg_b), (uint8_t *)&rx_buf_b, sizeof(rx_buf_b)))
    {
        return false;
    }

    for (uint8_t curr_segment = 0U; curr_segment < NUM_SEGMENTS; curr_segment++)
    {
        assert(io_ltc6813_check_pec((uint8_t *)&rx_buf_b[curr_segment], 6, &rx_buf_b[curr_segment].pec));
    }

    return true;
}
