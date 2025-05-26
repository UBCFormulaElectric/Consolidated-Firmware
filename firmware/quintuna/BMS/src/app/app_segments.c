#include "app_segments.h"
#include "app_canTx.h"
#include "app_thermistors.h"

#include "app_utils.h"
#include "io_ltc6813.h"
#include "io_time.h"

#include <hw_utils.h>
#include <math.h>
#include <stdint.h>
#include <string.h>

// See Table 5. t_6C gives total conversion time in us.
#define ADCV_CONVERSION_TIME_MS (3)   // Actually 2.343 ms but round up
#define ADAX_CONVERSION_TIME_MS (4)   // Actually 3.862 ms but round up
#define ADOL_CONVERSION_TIME_MS (2)   // Actually 1.024 but round up
#define ADSTAT_CONVERSION_TIME_MS (2) // Actually 1.556 ms but round up

#define VUV (0x619U) // 2.5V Under-voltage threshold = (VUV + 1) * 16 * 100uV
#define VOV (0xA41U) // 4.2V Over-voltage threshold = VOV * 16 * 100uV

#define VREF_AUX_REG 5

#define CONVERT_100UV_TO_VOLTAGE(v_100uv) ((float)v_100uv * 1E-4f)
#define CONVERT_VOLTAGE_TO_100UV(v) ((uint16_t)(v * 1E4f))

typedef enum
{
    THERMISTOR_MUX_0_7,
    THERMISTOR_MUX_8_13,
    THERMISTOR_MUX_COUNT,
} ThermistorMux;

static void (*const cell_voltage_setters[NUM_SEGMENTS][CELLS_PER_SEGMENT])(float) = {
    { app_canTx_BMS_Seg0_Cell0_Voltage_set, app_canTx_BMS_Seg0_Cell1_Voltage_set, app_canTx_BMS_Seg0_Cell2_Voltage_set,
      app_canTx_BMS_Seg0_Cell3_Voltage_set, app_canTx_BMS_Seg0_Cell4_Voltage_set, app_canTx_BMS_Seg0_Cell5_Voltage_set,
      app_canTx_BMS_Seg0_Cell6_Voltage_set, app_canTx_BMS_Seg0_Cell7_Voltage_set, app_canTx_BMS_Seg0_Cell8_Voltage_set,
      app_canTx_BMS_Seg0_Cell9_Voltage_set, app_canTx_BMS_Seg0_Cell10_Voltage_set,
      app_canTx_BMS_Seg0_Cell11_Voltage_set, app_canTx_BMS_Seg0_Cell12_Voltage_set,
      app_canTx_BMS_Seg0_Cell13_Voltage_set }
};
static void (*const thermistor_setters[NUM_SEGMENTS][CELLS_PER_SEGMENT])(float) = {
    { app_canTx_BMS_Seg0_Cell0_Temp_set, app_canTx_BMS_Seg0_Cell1_Temp_set, app_canTx_BMS_Seg0_Cell2_Temp_set,
      app_canTx_BMS_Seg0_Cell3_Temp_set, app_canTx_BMS_Seg0_Cell4_Temp_set, app_canTx_BMS_Seg0_Cell5_Temp_set,
      app_canTx_BMS_Seg0_Cell6_Temp_set, app_canTx_BMS_Seg0_Cell7_Temp_set, app_canTx_BMS_Seg0_Cell8_Temp_set,
      app_canTx_BMS_Seg0_Cell9_Temp_set, app_canTx_BMS_Seg0_Cell10_Temp_set, app_canTx_BMS_Seg0_Cell11_Temp_set,
      app_canTx_BMS_Seg0_Cell12_Temp_set, app_canTx_BMS_Seg0_Cell13_Temp_set }
};

static uint16_t voltage_regs[NUM_SEGMENTS][CELLS_PER_SEGMENT];
static ExitCode volt_success_buf[NUM_SEGMENTS][CELLS_PER_SEGMENT];
static float    cell_voltages[NUM_SEGMENTS][CELLS_PER_SEGMENT];

static uint16_t aux_regs[NUM_SEGMENTS][AUX_REGS_PER_SEGMENT];
static ExitCode aux_reg_success_buf[NUM_SEGMENTS][AUX_REGS_PER_SEGMENT];
static float    segment_vref[NUM_SEGMENTS];
static float    cell_temps[NUM_SEGMENTS][THERMISTORS_PER_SEGMENT];

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

void app_segments_writeDefaultConfig(void)
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
static bool isConfigEqual(void)
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

    bool seg_config_same = true;
    for (uint8_t seg = 0; seg_config_same && seg < NUM_SEGMENTS; seg++)
    {
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

ExitCode app_segments_configSync(void)
{
    static void (*const comm_ok_setters[NUM_SEGMENTS])(bool) = { app_canTx_BMS_Seg0_CommOk_set };

    for (uint8_t tries = 0; !isConfigEqual(); tries++)
    {
        UNUSED(tries);

        // first check if it's a comms thing
        bool comms_ok = true;
        for (uint8_t seg = 0; seg < NUM_SEGMENTS; seg++)
        {
            const bool seg_comms_ok = IS_EXIT_OK(config_success_buf[seg]);
            comm_ok_setters[seg](seg_comms_ok);
            comms_ok &= seg_comms_ok;
        }

        if (!comms_ok)
        {
            // TODO add a function to reset all values??
            for (uint8_t seg_idx = 0; seg_idx < NUM_SEGMENTS; seg_idx++)
            {
                for (uint8_t cell = 0; cell < CELLS_PER_SEGMENT; cell++)
                {
                    cell_voltage_setters[seg_idx][cell](-0.1f);
                }

                for (uint8_t thermistor = 0; thermistor < CELLS_PER_SEGMENT; thermistor++)
                {
                    thermistor_setters[seg_idx][thermistor](-0.1f);
                }
            }
        }

        // comms is fine, but configs are not matching
        LOG_IF_ERR(io_ltc6813_writeConfigurationRegisters(segment_config));
    }

    return EXIT_CODE_OK;
}

ExitCode app_segments_broadcastCellVoltages(void)
{
    RETURN_IF_ERR(io_ltc6813_startCellsAdcConversion());
    io_time_delay(ADCV_CONVERSION_TIME_MS);
    io_ltc6813_readVoltageRegisters(voltage_regs, volt_success_buf);

    for (uint8_t segment = 0; segment < NUM_SEGMENTS; segment++)
    {
        for (uint8_t cell = 0; cell < CELLS_PER_SEGMENT; cell++)
        {
            if (IS_EXIT_ERR(volt_success_buf[segment][cell]))
            {
                // we claim that this is insufficient to raise module comm errors
                continue;
            }

            // see page 68, 0xffff is invalid (either not populated or faulted)
            if (voltage_regs[segment][cell] == 0xffff)
            {
                // -0.1V over CAN means invalid
                cell_voltage_setters[segment][cell](-0.1f);
                volt_success_buf[segment][cell] = EXIT_CODE_ERROR;
                continue;
            }

            const float voltage          = CONVERT_100UV_TO_VOLTAGE(voltage_regs[segment][cell]);
            cell_voltages[segment][cell] = voltage;
            cell_voltage_setters[segment][cell](voltage);
        }
    }

    return EXIT_CODE_OK;
}

ExitCode app_segments_broadcastTempsVRef(void)
{
    static void (*const vref_setters[NUM_SEGMENTS])(float) = { app_canTx_BMS_Seg0_Vref_set };

    memset(segment_vref, 0, NUM_SEGMENTS * sizeof(uint16_t));
    memset(cell_temps, 0, NUM_SEGMENTS * THERMISTORS_PER_SEGMENT * sizeof(float));

    for (ThermistorMux mux = 0U; mux < THERMISTOR_MUX_COUNT; mux++)
    {
        writeThermistorMux(mux);
        io_ltc6813_writeConfigurationRegisters(segment_config);

        RETURN_IF_ERR(io_ltc6813_startThermistorsAdcConversion());
        io_time_delay(ADAX_CONVERSION_TIME_MS);
        io_ltc6813_readAuxRegisters(aux_regs, aux_reg_success_buf);

        for (uint8_t segment = 0U; segment < NUM_SEGMENTS; segment++)
        {
            for (uint8_t aux_gpio = 0U; aux_gpio < AUX_REGS_PER_SEGMENT; aux_gpio++)
            {
                const uint8_t vref_reg_adjustment =
                    aux_gpio > VREF_AUX_REG ? 1
                                            : 0; // Annoyingly the VREF reg is right in the middle of GPIO voltage regs
                const uint8_t mux_offset = mux > 0 ? 8 : 0; // 8 GPIOs used for least significant MUX setting
                const int     thermistor = aux_gpio - vref_reg_adjustment + mux_offset;

                if (thermistor >= THERMISTORS_PER_SEGMENT)
                {
                    continue;
                }

                if (IS_EXIT_ERR(aux_reg_success_buf[segment][aux_gpio]))
                {
                    continue;
                }

                const float voltage = CONVERT_100UV_TO_VOLTAGE(aux_regs[segment][aux_gpio]);
                if (aux_gpio == VREF_AUX_REG)
                {
                    segment_vref[segment] = voltage;
                    vref_setters[segment](voltage);
                    continue;
                }

                // TODO: Use measured VREF instead of constant
                const float current    = (3.0f - voltage) / 10e3f;
                const float resistance = voltage / current;
                const float temp       = app_thermistor_resistanceToTemp(resistance, &ltc_thermistor_lut);

                cell_temps[segment][thermistor] = temp;
                thermistor_setters[segment][thermistor](temp);
            }
        }
    }

    return EXIT_CODE_OK;
}

ExitCode app_segments_broadcastStatus(void)
{
    static void (*const mux_test_setters[NUM_SEGMENTS])(bool)     = { app_canTx_BMS_Seg0_MuxOk_set };
    static void (*const vreg_ok_setters[NUM_SEGMENTS])(bool)      = { app_canTx_BMS_Seg0_AnalogSupplyOk_set };
    static void (*const vreg_setters[NUM_SEGMENTS])(float)        = { app_canTx_BMS_Seg0_AnalogSupply_set };
    static void (*const vregd_setters[NUM_SEGMENTS])(bool)        = { app_canTx_BMS_Seg0_DigitalSupplyOk_set };
    static void (*const rev_code_setters[NUM_SEGMENTS])(uint32_t) = { app_canTx_BMS_Seg0_RevisionCode_set };
    static void (*const thermal_ok_setters[NUM_SEGMENTS])(bool)   = { app_canTx_BMS_Seg0_ThermalOk_set };
    static void (*const temp_setters[NUM_SEGMENTS])(uint32_t)     = { app_canTx_BMS_Seg0_DieTemp_set };

    RETURN_IF_ERR(io_ltc6813_startInternalADCConversions());
    io_time_delay(ADSTAT_CONVERSION_TIME_MS);
    io_ltc6813_getStatus(statuses, status_success_buf);

    for (uint8_t segment = 0; segment < NUM_SEGMENTS; segment++)
    {
        if (IS_EXIT_ERR(status_success_buf[segment]))
        {
            rev_code_setters[segment](0);
            vreg_ok_setters[segment](false);
            vregd_setters[segment](false);
            thermal_ok_setters[segment](false);
            mux_test_setters[segment](false);
            continue;
        }

        rev_code_setters[segment](statuses[segment].stat_b.REV);

        const float temperature = CONVERT_100UV_TO_VOLTAGE(statuses[segment].stat_a.ITMP) / 7.6e-3f - 276.0f;
        temp_setters[segment]((uint32_t)temperature);

        const float analog_power_supply = CONVERT_100UV_TO_VOLTAGE(statuses[segment].stat_a.VA);
        vreg_ok_setters[segment](4.5f <= analog_power_supply && analog_power_supply <= 5.5f);
        vreg_setters[segment](analog_power_supply);

        const float digital_power_supply = CONVERT_100UV_TO_VOLTAGE(statuses[segment].stat_b.VD);
        vregd_setters[segment](2.7f <= digital_power_supply && digital_power_supply <= 3.6f);

        thermal_ok_setters[segment](!statuses[segment].stat_b.THSD);
        mux_test_setters[segment](!(bool)statuses[segment].stat_b.MUXFAIL);
    }

    return EXIT_CODE_OK;
}

ExitCode app_segments_ADCAccuracyTest(void)
{
    static void (*const segment_vref_ok_setters[NUM_SEGMENTS])(bool) = { app_canTx_BMS_Seg0_VrefOk_set };
    static void (*const segment_overlap_adc1_2_test_setters[NUM_SEGMENTS])(
        bool) = { app_canTx_BMS_Seg0_Adc12Equal_set };
    static void (*const segment_overalap_adc2_3_test_setters[NUM_SEGMENTS])(
        bool) = { app_canTx_BMS_Seg0_Adc23Equal_set };

    RETURN_IF_ERR(io_ltc6813_overlapADCTest());
    io_time_delay(ADOL_CONVERSION_TIME_MS);
    io_ltc6813_readVoltageRegisters(voltage_regs, volt_success_buf);

    for (uint8_t segment = 0; segment < NUM_SEGMENTS; segment++)
    {
        // See section "Overlap Cell Measurement (ADOL Command)".
        const float vref_diff    = fabsf(segment_vref[segment] - 3.0f);
        const float adc_1_2_diff = fabsf(cell_voltages[segment][6] - cell_voltages[segment][7]);
        const float adc_3_4_diff = fabsf(cell_voltages[segment][12] - cell_voltages[segment][13]);

        const bool vref_fail = IS_EXIT_ERR(aux_reg_success_buf[segment][VREF_AUX_REG]);
        segment_vref_ok_setters[segment](!vref_fail && vref_diff < 0.014f);

        const bool adc_1_2_fail =
            IS_EXIT_ERR(volt_success_buf[segment][6]) || IS_EXIT_ERR(volt_success_buf[segment][7]);
        segment_overlap_adc1_2_test_setters[segment](!adc_1_2_fail && adc_1_2_diff < 0.001f);

        const bool adc_2_3_fail =
            IS_EXIT_ERR(volt_success_buf[segment][12]) || IS_EXIT_ERR(volt_success_buf[segment][13]);
        segment_overalap_adc2_3_test_setters[segment](!adc_2_3_fail && adc_3_4_diff < 0.001f);
    }

    return EXIT_CODE_OK;
}

ExitCode app_segments_voltageSelftest(void)
{
    static void (*const segmentVoltageSelfTestSetters[NUM_SEGMENTS])(
        bool) = { app_canTx_BMS_Seg0_VOLT_REGISTER_OK_set };

    RETURN_IF_ERR(io_ltc6813_sendSelfTestVoltages());
    io_time_delay(10); // TODO tweak timings
    io_ltc6813_readVoltageRegisters(voltage_regs, volt_success_buf);

    for (uint8_t segment = 0; segment < NUM_SEGMENTS; segment++)
    {
        bool self_test_ok = true;
        for (uint8_t cell = 0; cell < CELLS_PER_SEGMENT; cell++)
        {
            const bool cell_ok =
                volt_success_buf[segment][cell] && voltage_regs[segment][cell] == SELF_TEST_EXPECTED_VALUE;
            self_test_ok &= cell_ok;
        }

        segmentVoltageSelfTestSetters[segment](self_test_ok);
    }

    return EXIT_CODE_OK;
}

ExitCode app_segments_auxSelftest(void)
{
    static void (*const segmentAuxRegSelfTestSetters[NUM_SEGMENTS])(bool) = { app_canTx_BMS_Seg0_AUX_REGISTER_OK_set };

    RETURN_IF_ERR(io_ltc6813_sendSelfTestAux());
    io_time_delay(10); // TODO tweak timings
    io_ltc6813_readAuxRegisters(aux_regs, aux_reg_success_buf);

    for (uint8_t segment = 0; segment < NUM_SEGMENTS; segment++)
    {
        bool self_test_ok = true;
        for (uint8_t aux_reg = 0; self_test_ok && aux_reg < AUX_REGS_PER_SEGMENT; aux_reg++)
        {
            const bool aux_ok =
                aux_reg_success_buf[segment][aux_reg] && aux_regs[segment][aux_reg] == SELF_TEST_EXPECTED_VALUE;
            self_test_ok &= aux_ok;
        }

        segmentAuxRegSelfTestSetters[segment](self_test_ok);
    }

    return EXIT_CODE_OK;
}

ExitCode app_segments_statusSelftest(void)
{
    static void (*const segmentStatTestSelfTestSetters[NUM_SEGMENTS])(
        bool) = { app_canTx_BMS_Seg0_STAT_REGISTER_OK_set };

    RETURN_IF_ERR(io_ltc6813_sendSelfTestStat());
    io_time_delay(10); // TODO tweak timings
    io_ltc6813_getStatus(statuses, status_success_buf);

    for (uint8_t segment = 0; segment < NUM_SEGMENTS; segment++)
    {
        const uint16_t *statuses_buffer = (uint16_t *)&statuses[segment];
        bool            self_test_pass  = true;

        for (uint8_t word = 0; word < 4; word++) // NOTE: only the first four words we need to check
        {
            self_test_pass &= statuses_buffer[word] == SELF_TEST_EXPECTED_VALUE;
        }
        segmentStatTestSelfTestSetters[segment](self_test_pass);
    }

    return EXIT_CODE_OK;
}

// TODO: This is sus
ExitCode app_segments_openWireCheck(void)
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
    ASSERT_EXIT_OK(io_ltc6813_owcPull(PULL_UP));
    // io_time_delay(201);
    ASSERT_EXIT_OK(io_ltc6813_owcPull(PULL_UP));
    // io_time_delay(201);
    // ASSERT_EXIT_OK(io_ltc6813_startCellsAdcConversion()); // TODO: I don't think you do this here?
    io_time_delay(201);
    io_ltc6813_readVoltageRegisters(owc_pucv, volt_success_buf);

    // reset back to normal
    static uint16_t owc_pdcv[NUM_SEGMENTS][CELLS_PER_SEGMENT];
    ASSERT_EXIT_OK(io_ltc6813_owcPull(PULL_DOWN));
    // io_time_delay(201);
    ASSERT_EXIT_OK(io_ltc6813_owcPull(PULL_DOWN));
    // io_time_delay(201);
    // ASSERT_EXIT_OK(io_ltc6813_startCellsAdcConversion()); // TODO: I don't think you do this here?
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
