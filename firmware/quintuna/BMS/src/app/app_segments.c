#include "app_segments.h"
#include "app_canTx.h"
#include "app_thermistors.h"

#include "app_utils.h"
#include "io_ltc6813.h"
#include "io_time.h"

#include <hw_utils.h>
#include <stdint.h>
#include <string.h>

// TODO extended balancing

#define VUV (0x619U) // 2.5V Under-voltage threshold = (VUV + 1) * 16 * 100uV
#define VOV (0xA41U) // 4.2V Over-voltage threshold = VOV * 16 * 100uV

#define CONVERT_100UV_TO_VOLTAGE(v_100uv) ((float)v_100uv * 1E-4f)
#define CONVERT_VOLTAGE_TO_100UV(v) ((uint16_t)(v * 1E4f))

typedef enum
{
    THERMISTOR_MUX_0_7,
    THERMISTOR_MUX_8_13,
    THERMISTOR_MUX_COUNT,
} ThermistorMux;

static ADCSpeed adc_speed = ADCSpeed_7kHz;

static uint16_t segment_vref[NUM_SEGMENTS];

static uint16_t voltage_regs[NUM_SEGMENTS][CELLS_PER_SEGMENT];
static ExitCode volt_success_buf[NUM_SEGMENTS][VOLTAGE_REGISTER_GROUPS];

static uint16_t aux_regs[NUM_SEGMENTS][AUX_REGS_PER_SEGMENT];
// TODO find a way to merge these two
static ExitCode aux_reg_success_buf[NUM_SEGMENTS][AUX_REG_GROUPS];

static StatusRegGroups statuses[NUM_SEGMENTS];
static ExitCode        status_success_buf[NUM_SEGMENTS];

static SegmentConfig segment_config[NUM_SEGMENTS];
static SegmentConfig want_segment_config[NUM_SEGMENTS];
static ExitCode      config_success_buf[NUM_SEGMENTS];

static void writeThermistorMux(ThermistorMux mux)
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

void app_segments_adcSpeed(const ADCSpeed speed)
{
    adc_speed = speed;
    for (uint8_t seg = 0; seg < NUM_SEGMENTS; seg++)
    {
        segment_config[seg].reg_a.adcopt = 0x1u & (speed >> 2);
    }
}

void app_segments_writeDefaultConfig()
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

/**
 * @returns whether there are no errors and config matches
 * Hence if false there are either errors or the config does not match
 */
static bool is_config_equal()
{
    for (uint8_t try = 0; try < 3; try++)
    {
        io_ltc6813_readConfigurationRegisters(want_segment_config, config_success_buf);
        for (uint8_t seg = 0; seg < NUM_SEGMENTS; seg++)
        {
            if (IS_EXIT_ERR(config_success_buf[seg]))
            {
                return false;
            }
        }
    }

    // check that segment_config is same
    bool seg_config_same = true;
    for (uint8_t seg = 0; seg_config_same && seg < NUM_SEGMENTS; seg++)
    {
        // seg_config_same &= memcmp(&segment_config[seg], &want_segment_config[seg], sizeof(SegmentConfig)) == 0;
        const SegmentConfig *const a = &segment_config[seg];
        const SegmentConfig *const b = &want_segment_config[seg];
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

    return seg_config_same;
}

ExitCode app_segments_configSync()
{
    static void (*const commsOkSetter[NUM_SEGMENTS])(bool) = { app_canTx_BMS_Seg0_COMM_OK_set };
    for (uint8_t tries = 0; !is_config_equal(); tries++)
    {
        // first check if it's a comms thing
        bool comms_ok = true;
        for (uint8_t seg = 0; seg < NUM_SEGMENTS; seg++)
        {
            const bool seg_comms_ok = IS_EXIT_OK(config_success_buf[seg]);
            commsOkSetter[seg](seg_comms_ok);
            comms_ok &= seg_comms_ok;
        }
        if (!comms_ok)
        {
            // TODO debounced AMS fault
            // TODO add a function to reset all values??
            BMS_Segment0_Tests_Signals *const a = (BMS_Segment0_Tests_Signals *)app_canTx_BMS_Segment0_Tests_getData();
            memset(a, 0, sizeof(BMS_Segment0_Tests_Signals));
            // memset all voltage setters to 0
            BMS_Segment0_VoltageStats_0_Signals *const b =
                (BMS_Segment0_VoltageStats_0_Signals *)app_canTx_BMS_Segment0_VoltageStats_0_getData();
            BMS_Segment0_VoltageStats_1_Signals *const c =
                (BMS_Segment0_VoltageStats_1_Signals *)app_canTx_BMS_Segment0_VoltageStats_1_getData();
            memset(b, 0, sizeof(BMS_Segment0_VoltageStats_0_Signals));
            memset(c, 0, sizeof(BMS_Segment0_VoltageStats_1_Signals));
        }

        // comms is fine, but configs are not matching
        (void)io_ltc6813_writeConfigurationRegisters(segment_config);
    }
    return EXIT_CODE_OK;
}

// test setters
ExitCode app_segments_broadcastCellVoltages()
{
    static void (*const cellVoltageSetters[NUM_SEGMENTS][CELLS_PER_SEGMENT])(
        float) = { { app_canTx_BMS_Seg0_Cell0_Voltage_set, app_canTx_BMS_Seg0_Cell1_Voltage_set,
                     app_canTx_BMS_Seg0_Cell2_Voltage_set, app_canTx_BMS_Seg0_Cell3_Voltage_set,
                     app_canTx_BMS_Seg0_Cell4_Voltage_set, app_canTx_BMS_Seg0_Cell5_Voltage_set,
                     app_canTx_BMS_Seg0_Cell6_Voltage_set, app_canTx_BMS_Seg0_Cell7_Voltage_set,
                     app_canTx_BMS_Seg0_Cell8_Voltage_set, app_canTx_BMS_Seg0_Cell9_Voltage_set,
                     app_canTx_BMS_Seg0_Cell10_Voltage_set, app_canTx_BMS_Seg0_Cell11_Voltage_set,
                     app_canTx_BMS_Seg0_Cell12_Voltage_set, app_canTx_BMS_Seg0_Cell13_Voltage_set } };

    // information gathering
    RETURN_IF_ERR(io_ltc6813_startCellsAdcConversion(adc_speed));
    io_time_delay(10); // TODO tweak timings
    io_ltc6813_readVoltageRegisters(voltage_regs, volt_success_buf);

    // processing
    for (uint8_t segment = 0; segment < NUM_SEGMENTS; segment++)
    {
        for (uint8_t cell = 0; cell < CELLS_PER_SEGMENT; cell++)
        {
            if (IS_EXIT_ERR(volt_success_buf[segment][cell / 3]))
            {
                // we claim that this is insufficient to raise module comm errors
                continue;
            }
            // see page 68, 0xffff is invalid (either not populated or faulted)
            if (voltage_regs[segment][cell] == 0xffff)
            {
                cellVoltageSetters[segment][cell](0xffff);
                volt_success_buf[segment][cell / 3] = EXIT_CODE_ERROR;
                continue;
            }
            cellVoltageSetters[segment][cell](CONVERT_100UV_TO_VOLTAGE(voltage_regs[segment][cell]));
        }
    }
    return EXIT_CODE_OK;
}

ExitCode app_segments_broadcastTempsVRef()
{
    static void (*const thermistor_setters[NUM_SEGMENTS][CELLS_PER_SEGMENT])(float) = {
        { app_canTx_BMS_Seg0_Cell0_Temp_set, app_canTx_BMS_Seg0_Cell1_Temp_set, app_canTx_BMS_Seg0_Cell2_Temp_set,
          app_canTx_BMS_Seg0_Cell3_Temp_set, app_canTx_BMS_Seg0_Cell4_Temp_set, app_canTx_BMS_Seg0_Cell5_Temp_set,
          app_canTx_BMS_Seg0_Cell6_Temp_set, app_canTx_BMS_Seg0_Cell7_Temp_set, app_canTx_BMS_Seg0_Cell8_Temp_set,
          app_canTx_BMS_Seg0_Cell9_Temp_set, app_canTx_BMS_Seg0_Cell10_Temp_set, app_canTx_BMS_Seg0_Cell11_Temp_set,
          app_canTx_BMS_Seg0_Cell12_Temp_set, app_canTx_BMS_Seg0_Cell13_Temp_set }
    };

    static void (*const vrefSetters[NUM_SEGMENTS])(float) = { app_canTx_BMS_Seg0_VREF_set };
    // TODO convert at last moment, most always keep as uint16_t
    static float cell_temps[NUM_SEGMENTS][THERMISTORS_PER_SEGMENT];
    memset(segment_vref, 0, NUM_SEGMENTS * sizeof(uint16_t));
    memset(cell_temps, 0, NUM_SEGMENTS * THERMISTORS_PER_SEGMENT * sizeof(float));

    for (ThermistorMux mux = 0U; mux < THERMISTOR_MUX_COUNT; mux++)
    {
        writeThermistorMux(mux);
        io_ltc6813_writeConfigurationRegisters(segment_config);

        // information gathering
        // TODO flip a mux based on i == 0 and i == 1
        RETURN_IF_ERR(io_ltc6813_startThermistorsAdcConversion(adc_speed));
        io_time_delay(10); // TODO tweak timings
        io_ltc6813_readAuxRegisters(aux_regs, aux_reg_success_buf);

        // processing
        for (uint8_t segment = 0U; segment < NUM_SEGMENTS; segment++)
        {
            for (uint8_t aux_gpio = 0U; aux_gpio < (mux == 0 ? 9 : 7); aux_gpio++)
            {
                if (IS_EXIT_ERR(aux_reg_success_buf[segment][aux_gpio / 3]))
                {
                    continue;
                }

                const float voltage = CONVERT_100UV_TO_VOLTAGE(aux_regs[segment][aux_gpio]);
                if (aux_gpio == 5)
                {
                    segment_vref[segment] = aux_regs[segment][aux_gpio];
                    vrefSetters[segment](voltage);
                    continue;
                }

                const float current    = (3.0f - voltage) / 10e3f;
                const float resistance = voltage / current;
                const float temp       = app_thermistor_resistanceToTemp(resistance, &ltc_thermistor_lut);

                const uint8_t adj         = aux_gpio >= 6 ? 1 : 0;
                const int     cell        = aux_gpio - adj + (8) * (int)mux;
                cell_temps[segment][cell] = temp;

                if (thermistor_setters[segment][cell] != NULL)
                {
                    thermistor_setters[segment][cell](temp);
                }
            }
        }
    }

    return EXIT_CODE_OK;
}

ExitCode app_segments_broadcastStatus()
{
    static void (*const muxTestSetters[NUM_SEGMENTS])(bool)     = { app_canTx_BMS_Seg0_MUX_OK_set };
    static void (*const vregOKSetters[NUM_SEGMENTS])(bool)      = { app_canTx_BMS_Seg0_VREG_OK_set };
    static void (*const vregSetters[NUM_SEGMENTS])(float)       = { app_canTx_BMS_Seg0_VREG_set };
    static void (*const vregdSetters[NUM_SEGMENTS])(bool)       = { app_canTx_BMS_Seg0_VREGD_OK_set };
    static void (*const revCodeSetters[NUM_SEGMENTS])(uint32_t) = { app_canTx_BMS_Seg0_REV_CODE_set };
    static void (*const thermalOKsetters[NUM_SEGMENTS])(bool)   = { app_canTx_BMS_Seg0_THERMAL_OK_set };
    // static void (*const tempSetters[NUM_SEGMENTS])(float)           = {  };

    RETURN_IF_ERR(io_ltc6813_startInternalADCConversions(adc_speed));
    io_time_delay(15);
    io_ltc6813_getStatus(statuses, status_success_buf);
    for (uint8_t segment = 0; segment < NUM_SEGMENTS; segment++)
    {
        if (IS_EXIT_ERR(status_success_buf[segment]))
        {
            revCodeSetters[segment](0);
            vregOKSetters[segment](false);
            vregdSetters[segment](false);
            thermalOKsetters[segment](false);
            muxTestSetters[segment](false);
            continue;
        }

        revCodeSetters[segment](statuses[segment].REV);

        // tempSetters[segment]((float)statuses[segment].ITMP / 76.0f - 276.0f);

        vregOKSetters[segment](
            CONVERT_VOLTAGE_TO_100UV(4.5f) <= statuses[segment].VA &&
            statuses[segment].VA <= CONVERT_VOLTAGE_TO_100UV(5.5f));
        vregSetters[segment](CONVERT_100UV_TO_VOLTAGE(statuses[segment].VA));
        vregdSetters[segment](
            CONVERT_VOLTAGE_TO_100UV(2.7f) <= statuses[segment].VD &&
            statuses[segment].VD <= CONVERT_VOLTAGE_TO_100UV(3.6f));

        thermalOKsetters[segment](!statuses[segment].THSD);
        muxTestSetters[segment](!(bool)statuses[segment].MUXFAIL);
    }
    return EXIT_CODE_OK;
}

ExitCode app_segments_ADCAccuracyTest()
{
    static void (*const segmentVRefTrueSetters[NUM_SEGMENTS])(bool)         = { app_canTx_BMS_Seg0_VREF_OK_set };
    static void (*const segmentOverlapADC12TestSetters[NUM_SEGMENTS])(bool) = { app_canTx_BMS_Seg0_ADC_1_2_Equal_set };
    static void (*const segmentOverlapADC23TestSetters[NUM_SEGMENTS])(bool) = { app_canTx_BMS_Seg0_ADC_2_3_Equal_set };

    RETURN_IF_ERR(io_ltc6813_overlapADCTest(adc_speed));
    io_time_delay(10);                                               // TODO tweak timings
    io_ltc6813_readVoltageRegisters(voltage_regs, volt_success_buf); // TODO make this more efficient

    // for debug mode
    for (uint8_t segment = 0; segment < NUM_SEGMENTS; segment++)
    {
        // 6, 7 and 12, 13
        const uint16_t vref_diff    = (uint16_t)abs(segment_vref[segment] - CONVERT_VOLTAGE_TO_100UV(3.0f)),
                       adc_1_2_diff = (uint16_t)abs(voltage_regs[segment][6] - voltage_regs[segment][7]),
                       adc_3_4_diff = (uint16_t)abs(voltage_regs[segment][12] - voltage_regs[segment][13]);
        segmentVRefTrueSetters[segment](vref_diff < CONVERT_VOLTAGE_TO_100UV(0.014f));
        const bool adc_1_2_fail =
            IS_EXIT_ERR(volt_success_buf[segment][0]) || IS_EXIT_ERR(volt_success_buf[segment][1]);
        segmentOverlapADC12TestSetters[segment](!adc_1_2_fail && adc_1_2_diff < CONVERT_VOLTAGE_TO_100UV(0.001f));
        const bool adc_2_3_fail =
            IS_EXIT_ERR(volt_success_buf[segment][1]) || IS_EXIT_ERR(volt_success_buf[segment][2]);
        segmentOverlapADC23TestSetters[segment](!adc_2_3_fail && adc_3_4_diff < CONVERT_VOLTAGE_TO_100UV(0.001f));
    }
    return EXIT_CODE_OK;
}

void app_segments_voltageSelftest()
{
    static void (*const segmentVoltageSelfTestSetters[NUM_SEGMENTS])(
        bool) = { app_canTx_BMS_Seg0_VOLT_REGISTER_OK_set };

    // ASSERT_EXIT_OK(io_ltc6813_clearCellRegisters());
    ASSERT_EXIT_OK(io_ltc6813_sendSelfTestVoltages(adc_speed));
    io_time_delay(10); // TODO tweak timings
    io_ltc6813_readVoltageRegisters(voltage_regs, volt_success_buf);
    for (uint8_t segment = 0; segment < NUM_SEGMENTS; segment++)
    {
        bool self_test_ok = true;
        for (uint8_t cell = 0; cell < CELLS_PER_SEGMENT; cell++)
        {
            const bool cell_ok = voltage_regs[segment][cell] == io_ltc6813_selfTestExpectedValue(adc_speed);
            self_test_ok &= cell_ok;
        }
        segmentVoltageSelfTestSetters[segment](self_test_ok);
    }
}

void app_segments_auxSelftest()
{
    static void (*const segmentAuxRegSelfTestSetters[NUM_SEGMENTS])(bool) = { app_canTx_BMS_Seg0_AUX_REGISTER_OK_set };

    // ASSERT_EXIT_OK(io_ltc6813_clearAuxRegisters());
    ASSERT_EXIT_OK(io_ltc6813_sendSelfTestAux(adc_speed));
    io_time_delay(10); // TODO tweak timings
    io_ltc6813_readAuxRegisters(aux_regs, aux_reg_success_buf);
    for (uint8_t segment = 0; segment < NUM_SEGMENTS; segment++)
    {
        bool self_test_ok = true;
        for (uint8_t aux_reg = 0; self_test_ok && aux_reg < AUX_REGS_PER_SEGMENT; aux_reg++)
        {
            const bool aux_ok = aux_regs[segment][aux_reg] == io_ltc6813_selfTestExpectedValue(adc_speed);
            self_test_ok &= aux_ok;
        }
        segmentAuxRegSelfTestSetters[segment](self_test_ok);
    }
}

void app_segments_statusSelftest()
{
    static void (*const segmentStatTestSelfTestSetters[NUM_SEGMENTS])(
        bool) = { app_canTx_BMS_Seg0_STAT_REGISTER_OK_set };

    ASSERT_EXIT_OK(io_ltc6813_sendSelfTestStat(adc_speed));
    io_time_delay(10); // TODO tweak timings
    io_ltc6813_getStatus(statuses, status_success_buf);

    for (uint8_t segment = 0; segment < NUM_SEGMENTS; segment++)
    {
        const uint16_t *statuses_buffer = (uint16_t *)&statuses[segment];
        bool            self_test_pass  = true;

        static_assert(sizeof(StatusRegGroups) == sizeof(uint16_t) * 6);
        for (uint8_t word = 0; word < 4; word++) // NOTE: only the first four words we need to check
        {
            self_test_pass &= statuses_buffer[word] == io_ltc6813_selfTestExpectedValue(adc_speed);
        }
        segmentStatTestSelfTestSetters[segment](self_test_pass);
    }
}

ExitCode app_segments_openWireCheck()
{
    static void (*const cellOWCSetters[NUM_SEGMENTS][CELLS_PER_SEGMENT])(bool) = {
        { app_canTx_BMS_Seg0_Cell0_OWC_OK_set, app_canTx_BMS_Seg0_Cell1_OWC_OK_set, app_canTx_BMS_Seg0_Cell2_OWC_OK_set,
          app_canTx_BMS_Seg0_Cell3_OWC_OK_set, app_canTx_BMS_Seg0_Cell4_OWC_OK_set, app_canTx_BMS_Seg0_Cell5_OWC_OK_set,
          app_canTx_BMS_Seg0_Cell6_OWC_OK_set, app_canTx_BMS_Seg0_Cell7_OWC_OK_set, app_canTx_BMS_Seg0_Cell8_OWC_OK_set,
          app_canTx_BMS_Seg0_Cell9_OWC_OK_set, app_canTx_BMS_Seg0_Cell10_OWC_OK_set,
          app_canTx_BMS_Seg0_Cell11_OWC_OK_set, app_canTx_BMS_Seg0_Cell12_OWC_OK_set,
          app_canTx_BMS_Seg0_Cell13_OWC_OK_set }
    };

    // data collection
    static uint16_t owc_pucv[NUM_SEGMENTS][CELLS_PER_SEGMENT];
    ASSERT_EXIT_OK(io_ltc6813CellVoltages_owcPull(PULL_UP, ADCSpeed_26Hz, false));
    // io_time_delay(201);
    ASSERT_EXIT_OK(io_ltc6813CellVoltages_owcPull(PULL_UP, ADCSpeed_26Hz, false));
    // io_time_delay(201);
    ASSERT_EXIT_OK(io_ltc6813_startCellsAdcConversion(ADCSpeed_26Hz));
    io_time_delay(201);
    io_ltc6813_readVoltageRegisters(owc_pucv, volt_success_buf);

    // reset back to normal
    static uint16_t owc_pdcv[NUM_SEGMENTS][CELLS_PER_SEGMENT];
    ASSERT_EXIT_OK(io_ltc6813CellVoltages_owcPull(PULL_DOWN, ADCSpeed_26Hz, false));
    // io_time_delay(201);
    ASSERT_EXIT_OK(io_ltc6813CellVoltages_owcPull(PULL_DOWN, ADCSpeed_26Hz, false));
    // io_time_delay(201);
    ASSERT_EXIT_OK(io_ltc6813_startCellsAdcConversion(ADCSpeed_26Hz));
    io_time_delay(201);
    io_ltc6813_readVoltageRegisters(owc_pdcv, volt_success_buf);

    // perform the check
    for (uint8_t segment = 0; segment < NUM_SEGMENTS; segment++)
    {
        cellOWCSetters[segment][0](owc_pucv[segment][0] != 0);
        for (uint8_t cell = 1; cell < CELLS_PER_SEGMENT; cell++)
        {
            const bool owc_passing =
                owc_pdcv[segment][cell] - owc_pucv[segment][cell] <= CONVERT_VOLTAGE_TO_100UV(0.4f);
            cellOWCSetters[segment][cell](owc_passing);
        }
#if CELLS_PER_SEGMENT >= 18
        completeness cellOWCSetters[segment][17](owc_pdcv[segment][cell] == 0.0f);
#endif
    }
    return EXIT_CODE_OK;
}
