#include "app/segments.h"
#include "app/segments/segments_internal.h"
#include "app/utils.h"

#define NUM_CONFIG_SYNC_TRIES 20
#define VUV (0x619U) // 2.5V Under-voltage threshold = (VUV + 1) * 16 * 100uV
#define VOV (0xA41U) // 4.2V Over-voltage threshold = VOV * 16 * 100uV

static SegmentConfig segment_config[NUM_SEGMENTS];

void app_segments_setDefaultConfig(void)
{
    for (uint8_t seg = 0; seg < NUM_SEGMENTS; seg++)
    {
        SegmentConfig *const out       = &segment_config[seg];
        CFGAR *const         seg_a_cfg = &out->reg_a;
        CFGBR *const         seg_b_cfg = &out->reg_b;

        // enables GPIOs for thermistors (reading values)
        seg_a_cfg->gpio_1_5 = 0x1F;
        seg_b_cfg->gpio_6_9 = 0xF;
        seg_a_cfg->refon    = 1;
        seg_a_cfg->dten     = 0;

        // upper and lower bound
        seg_a_cfg->vuv_0_7  = VUV & 0xFF;
        seg_a_cfg->vuv_8_11 = VUV >> 8 & 0xF;
        seg_a_cfg->vov_0_3  = VOV & 0xF;
        seg_a_cfg->vov_4_11 = VOV >> 4 & 0xFF;
    }
}

void app_segments_setBalanceConfig(const bool balance_config[NUM_SEGMENTS][CELLS_PER_SEGMENT])
{
    for (uint8_t seg = 0; seg < NUM_SEGMENTS; seg++)
    {
        SegmentConfig *const out       = &segment_config[seg];
        CFGAR *const         seg_a_cfg = &out->reg_a;
        CFGBR *const         seg_b_cfg = &out->reg_b;
        uint32_t             dcc_bits  = 0U;

        // Get dcc bits to write for the current segment (which cells to balance)
        for (uint8_t cell = 0; cell < CELLS_PER_SEGMENT; cell++)
        {
            dcc_bits |= (uint32_t)(balance_config[seg][cell] << cell);
        }

        // TODO double check this is correct
        // note that we want balance_config to be auto scalable (for futureproofing)
        seg_b_cfg->dcc_0     = 0;
        seg_a_cfg->dcc_1_8   = dcc_bits & 0xFF;
        seg_a_cfg->dcc_9_12  = dcc_bits >> 8 & 0xF;
        seg_b_cfg->dcc_13_16 = dcc_bits >> 12 & 0xF;
        seg_b_cfg->dcc_18    = 0;
        seg_b_cfg->dcc_17    = 0;
    }
}

void app_segments_setThermistorMuxConfig(const ThermistorMux mux)
{
    for (uint8_t seg = 0; seg < NUM_SEGMENTS; seg++)
    {
        SegmentConfig *const out       = &segment_config[seg];
        CFGBR *const         seg_b_cfg = &out->reg_b;

        if (mux == THERMISTOR_MUX_0_7)
        {
            seg_b_cfg->gpio_6_9 = 0x7;
        }
        else
        {
            seg_b_cfg->gpio_6_9 = 0xF;
        }
    }
}

/**
 * @returns whether there are no errors and config matches
 * Hence if false there are either errors or the config does not match
 */
static ExitCode isConfigEqual(void)
{
    SegmentConfig read_segment_config[NUM_SEGMENTS];
    ExitCode      config_success_buf[NUM_SEGMENTS];
    io_ltc6813_readConfigurationRegisters(read_segment_config, config_success_buf);

    for (uint8_t seg = 0; seg < NUM_SEGMENTS; seg++)
    {
        RETURN_IF_ERR(config_success_buf[seg]);
    }

    bool seg_config_same = true;
    for (uint8_t seg = 0; seg_config_same && seg < NUM_SEGMENTS; seg++)
    {
        const SegmentConfig *const a = &segment_config[seg];
        const SegmentConfig *const b = &read_segment_config[seg];

        // NOTE: the reason we don't use memcmp is because we only really care about these configs
        // base configs
        seg_config_same &= a->reg_a.adcopt == b->reg_a.adcopt;
        seg_config_same &= a->reg_a.refon == b->reg_a.refon;

        // undervoltage
        seg_config_same &= a->reg_a.vuv_0_7 == b->reg_a.vuv_0_7;
        seg_config_same &= a->reg_a.vuv_8_11 == b->reg_a.vuv_8_11;

        // overvoltage config
        seg_config_same &= a->reg_a.vov_0_3 == b->reg_a.vov_0_3;
        seg_config_same &= a->reg_a.vov_4_11 == b->reg_a.vov_4_11;

        // balancing leds
        seg_config_same &= a->reg_b.dcc_0 == b->reg_b.dcc_0;
        seg_config_same &= a->reg_a.dcc_1_8 == b->reg_a.dcc_1_8;
        seg_config_same &= a->reg_a.dcc_9_12 == b->reg_a.dcc_9_12;
        seg_config_same &= a->reg_b.dcc_13_16 == b->reg_b.dcc_13_16;
        seg_config_same &= a->reg_b.dcc_17 == b->reg_b.dcc_17;
        seg_config_same &= a->reg_b.dcc_18 == b->reg_b.dcc_18;
    }

    return seg_config_same ? EXIT_CODE_OK : EXIT_CODE_ERROR;
}

ExitCode app_segments_configSync(void)
{
    for (int tries = 0; tries < NUM_CONFIG_SYNC_TRIES; tries++)
    {
        const ExitCode is_config_equal = isConfigEqual();
        if (IS_EXIT_OK(is_config_equal))
        {
            return EXIT_CODE_OK;
        }

        LOG_IF_ERR(is_config_equal);
        LOG_IF_ERR(io_ltc6813_writeConfigurationRegisters(segment_config));
    }

    return EXIT_CODE_TIMEOUT;
}

ExitCode app_segments_writeConfig(void)
{
    return io_ltc6813_writeConfigurationRegisters(segment_config);
}
