// TODO: make it so that configs are stored here, and are enforced
#include "app_utils.h"
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
} CFGAR;
typedef struct __attribute__((__packed__))
{
    CFGAR config;
    PEC   pec;
} CFGAR_msg;
static_assert(sizeof(CFGAR) == REGISTER_GROUP_SIZE);
static_assert(sizeof(CFGAR_msg) == REGISTER_GROUP_SIZE + sizeof(PEC));

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
} CFGBR;
typedef struct __attribute__((__packed__))
{
    CFGBR config;
    PEC   pec;
} CFGBR_msg;
static_assert(sizeof(CFGBR) == REGISTER_GROUP_SIZE);
static_assert(sizeof(CFGBR_msg) == REGISTER_GROUP_SIZE + sizeof(PEC));

typedef struct
{
    CFGAR a;
    CFGBR b;
} SegmentConfig;

static SegmentConfig build_config()
{
    SegmentConfig out       = { 0 };
    CFGAR *const  seg_a_cfg = &out.a;
    CFGBR *const  seg_b_cfg = &out.b;

    // enables GPIOs for thermistors (reading values)
    seg_a_cfg->gpio_1_5 = 0x1F;
    seg_b_cfg->gpio_6_9 = 0xF;

    seg_a_cfg->refon = 0;
    seg_a_cfg->dten  = 0;

    // very important setting, determines which speeds are valid
    // TODO configure based on desired ADC speed
    seg_a_cfg->adcopt = 1;

#define VUV (0x4E1U) // Under-voltage comparison voltage, (VUV + 1) * 16 * 100uV
    seg_a_cfg->vuv_0_7  = VUV & 0xFF;
    seg_a_cfg->vuv_8_11 = VUV >> 8 & 0xF;
#define VOV (0x9C4U) // Over-voltage comparison voltage, VOV * 16 * 100uV
    seg_a_cfg->vov_0_3  = VOV & 0xF;
    seg_a_cfg->vov_4_11 = VOV >> 4 & 0xFF;
    return out;
}

/**
 * @param balance_config Configuration for the balancing
 * @attention For more information on how to configure the LTC, see table 56
 * @return Success if both succeeded. Fail if at least one failed.
 */
ExitCode io_ltc6813_writeConfigurationRegisters(bool balance_config[NUM_SEGMENTS][CELLS_PER_SEGMENT])
{
    // as per table 33
    struct __attribute__((__packed__))
    {
        ltc6813_tx cmd;
        CFGAR_msg  segment_configs[NUM_SEGMENTS]; // note these must be shifted in backwards (shift register style)
    } tx_msg_a = { 0 };
    memset(&tx_msg_a, 0, sizeof(tx_msg_a));
    static_assert(sizeof(tx_msg_a) == 4 + (sizeof(CFGAR) + sizeof(PEC)) * NUM_SEGMENTS);

    // as per table 33`
    struct __attribute__((__packed__))
    {
        ltc6813_tx cmd;
        CFGBR_msg  segment_configs[NUM_SEGMENTS];
    } tx_msg_b = { 0 };
    memset(&tx_msg_b, 0, sizeof(tx_msg_b));
    static_assert(sizeof(tx_msg_b) == 4 + (sizeof(CFGBR) + sizeof(PEC)) * NUM_SEGMENTS);

#define WRCFGA (0x0001)
#define WRCFGB (0x0024)
    tx_msg_a.cmd = io_ltc6813_build_tx_cmd(WRCFGA);
    tx_msg_b.cmd = io_ltc6813_build_tx_cmd(WRCFGB);

    const SegmentConfig shared_cfg = build_config();
    for (uint8_t curr_segment = 0U; curr_segment < NUM_SEGMENTS; curr_segment++)
    {
        // Data used to configure the last segment on the daisy chain needs to be sent first
        const uint8_t    tx_cfg_idx = NUM_SEGMENTS - curr_segment - 1;
        CFGAR_msg *const seg_a      = &tx_msg_a.segment_configs[tx_cfg_idx];
        CFGBR_msg *const seg_b      = &tx_msg_b.segment_configs[tx_cfg_idx];
        CFGAR *const     seg_a_cfg  = &seg_a->config;
        CFGBR *const     seg_b_cfg  = &seg_b->config;

        // write defaults
        *seg_a_cfg = shared_cfg.a;
        *seg_b_cfg = shared_cfg.b;

        // Write DCC bits for balancing
        if (balance_config != NULL)
        {
            uint32_t dcc_bits = 0U;
            // Get dcc bits to write for the current segment (which cells to balance)
            for (uint8_t cell = 0; cell < CELLS_PER_SEGMENT; cell++)
            {
                dcc_bits |= (uint32_t)(balance_config[curr_segment][cell] << cell);
            }
            seg_b_cfg->dcc_0     = 0;
            seg_a_cfg->dcc_1_8   = dcc_bits & 0xFF;
            seg_a_cfg->dcc_9_12  = dcc_bits >> 8 & 0xF;
            seg_b_cfg->dcc_13_16 = dcc_bits >> 12 & 0xF;
            seg_b_cfg->dcc_17    = 0;
            seg_b_cfg->dcc_18    = 0;
        }
        else
        {
            seg_b_cfg->dcc_0     = 0;
            seg_a_cfg->dcc_1_8   = 0;
            seg_a_cfg->dcc_9_12  = 0;
            seg_b_cfg->dcc_13_16 = 0;
            seg_b_cfg->dcc_17    = 0;
            seg_b_cfg->dcc_18    = 0;
        }

        // Calculate and pack the PEC15 bytes into data to write to the configuration register
        seg_a->pec = io_ltc6813_build_data_pec((uint8_t *)seg_a_cfg, sizeof(CFGAR));
        seg_b->pec = io_ltc6813_build_data_pec((uint8_t *)seg_b_cfg, sizeof(CFGBR));
    }
    // Write to configuration registers
    RETURN_IF_ERR(hw_spi_transmit(&ltc6813_spi, (uint8_t *)&tx_msg_a, sizeof(tx_msg_a)))
    RETURN_IF_ERR(hw_spi_transmit(&ltc6813_spi, (uint8_t *)&tx_msg_b, sizeof(tx_msg_b)))
    return EXIT_CODE_OK;
}

ExitCode io_ltc6813_readConfigurationRegisters(void)
{
#define RDCFGA (0x0002)
#define RDCFGB (0x0026)
    ltc6813_tx tx_msg_a = io_ltc6813_build_tx_cmd(RDCFGA);
    CFGAR_msg  rx_buf_a[NUM_SEGMENTS];
    RETURN_IF_ERR(hw_spi_transmitThenReceive(
        &ltc6813_spi, (uint8_t *)&tx_msg_a, sizeof(tx_msg_a), (uint8_t *)rx_buf_a, sizeof(rx_buf_a)));

    for (uint8_t curr_segment = 0U; curr_segment < NUM_SEGMENTS; curr_segment++)
    {
        assert(io_ltc6813_check_pec(
            (uint8_t *)&rx_buf_a[curr_segment], sizeof(rx_buf_a[curr_segment].config), &rx_buf_a[curr_segment].pec));
    }

    ltc6813_tx tx_msg_b = io_ltc6813_build_tx_cmd(RDCFGA);
    CFGBR_msg  rx_buf_b[NUM_SEGMENTS];
    RETURN_IF_ERR(hw_spi_transmitThenReceive(
        &ltc6813_spi, (uint8_t *)&tx_msg_b, sizeof(tx_msg_b), (uint8_t *)rx_buf_b, sizeof(rx_buf_b)));

    for (uint8_t curr_segment = 0U; curr_segment < NUM_SEGMENTS; curr_segment++)
    {
        RETURN_IF_ERR(
            io_ltc6813_check_pec(
                (uint8_t *)&rx_buf_b[curr_segment].config, sizeof(rx_buf_b[curr_segment].config),
                &rx_buf_b[curr_segment].pec)
                ? EXIT_CODE_OK
                : EXIT_CODE_ERROR);
    }

    return EXIT_CODE_OK;
}
