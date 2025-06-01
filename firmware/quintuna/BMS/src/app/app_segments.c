#include "app_segments.h"
#include "app_canTx.h"
#include "app_thermistor.h"

#include "app_utils.h"
#include "io_ltc6813.h"
#include "io_time.h"

#include <hw_utils.h>
#include <math.h>
#include <stdint.h>
#include <string.h>

// TODO: Take a closer look at these
#define ADCV_CONVERSION_TIME_MS (10)
#define ADAX_CONVERSION_TIME_MS (10)
#define ADOL_CONVERSION_TIME_MS (10)
#define ADSTAT_CONVERSION_TIME_MS (10)
#define DIAGN_CONVERSION_TIME_MS (10)

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

static float ltc_thermistor_lut_buffer[201] = {
    29239.8f, 28571.6f, 27920.7f, 27286.6f, 26668.8f, 26066.9f, 25480.3f, 24908.8f, 24351.8f, 23808.9f, 23279.8f,
    22764.0f, 22261.2f, 21771.0f, 21293.0f, 20827.0f, 20372.5f, 19929.4f, 19497.2f, 19075.6f, 18664.4f, 18263.3f,
    17872.0f, 17490.3f, 17117.8f, 16754.3f, 16399.7f, 16053.6f, 15715.8f, 15386.1f, 15064.3f, 14750.1f, 14443.4f,
    14144.0f, 13851.7f, 13566.3f, 13287.5f, 13015.3f, 12749.5f, 12489.9f, 12236.3f, 11988.6f, 11746.6f, 11510.2f,
    11279.2f, 11053.5f, 10833.0f, 10617.5f, 10407.0f, 10201.2f, 10000.0f, 9803.4f,  9611.2f,  9423.3f,  9239.7f,
    9060.1f,  8884.5f,  8712.9f,  8545.0f,  8380.8f,  8220.3f,  8063.3f,  7909.7f,  7759.4f,  7612.5f,  7468.7f,
    7328.1f,  7190.5f,  7055.8f,  6924.1f,  6795.2f,  6669.0f,  6545.5f,  6424.7f,  6306.4f,  6190.7f,  6077.3f,
    5966.4f,  5857.8f,  5751.4f,  5647.3f,  5545.4f,  5445.5f,  5347.8f,  5252.1f,  5158.3f,  5066.5f,  4976.5f,
    4888.4f,  4802.1f,  4717.5f,  4634.7f,  4553.5f,  4474.0f,  4396.1f,  4319.8f,  4244.9f,  4171.6f,  4099.8f,
    4029.3f,  3960.3f,  3892.6f,  3826.3f,  3761.3f,  3697.5f,  3635.0f,  3573.8f,  3513.7f,  3454.8f,  3397.0f,
    3340.4f,  3284.8f,  3230.3f,  3176.9f,  3124.5f,  3073.1f,  3022.7f,  2973.2f,  2924.7f,  2877.0f,  2830.3f,
    2784.5f,  2739.5f,  2695.4f,  2652.1f,  2609.6f,  2567.9f,  2526.9f,  2486.8f,  2447.4f,  2408.6f,  2370.6f,
    2333.4f,  2296.7f,  2260.8f,  2225.5f,  2190.9f,  2156.8f,  2123.4f,  2090.6f,  2058.4f,  2026.8f,  1995.7f,
    1965.2f,  1935.2f,  1905.8f,  1876.9f,  1848.5f,  1820.6f,  1793.2f,  1766.2f,  1739.8f,  1713.8f,  1688.2f,
    1663.1f,  1638.5f,  1614.2f,  1590.4f,  1567.0f,  1544.0f,  1521.4f,  1499.2f,  1477.3f,  1455.9f,  1434.8f,
    1414.0f,  1393.6f,  1373.6f,  1353.9f,  1334.5f,  1315.4f,  1296.7f,  1278.3f,  1260.2f,  1242.3f,  1224.8f,
    1207.6f,  1190.6f,  1174.0f,  1157.6f,  1141.4f,  1125.6f,  1110.0f,  1094.6f,  1079.5f,  1064.7f,  1050.1f,
    1035.7f,  1021.5f,  1007.6f,  993.9f,   980.4f,   967.2f,   954.1f,   941.3f,   928.6f,   916.2f,   903.9f,
    891.9f,   880.0f,   868.4f
};

const ThermistorLUT ltc_thermistor_lut = {
    .starting_temp = 0,
    .resolution    = 0.5f,
    .size          = 201,
    .resistances   = ltc_thermistor_lut_buffer,
};

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
static SegmentConfig read_segment_config[NUM_SEGMENTS];
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
        io_ltc6813_readConfigurationRegisters(read_segment_config, config_success_buf);
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

    return seg_config_same;
}

ExitCode app_segments_configSync(void)
{
    static void (*const comm_ok_setters[NUM_SEGMENTS])(bool) = { app_canTx_BMS_Seg0_CommOk_set };

    for (;;)
    {
        const bool is_config_equal = isConfigEqual();

        // first check if it's a comms thing
        bool comms_ok = true;
        for (uint8_t seg = 0; seg < NUM_SEGMENTS; seg++)
        {
            const bool seg_comms_ok = IS_EXIT_OK(config_success_buf[seg]);
            comm_ok_setters[seg](seg_comms_ok);
            comms_ok &= seg_comms_ok;
        }

        if (is_config_equal)
        {
            return EXIT_CODE_OK;
        }

        if (comms_ok)
        {
            // comms is fine, but configs are not matching
            LOG_IF_ERR(io_ltc6813_writeConfigurationRegisters(segment_config));
        }
        else
        {
            // comms are bad, reset voltages and temps in the CAN table
            // TODO add a function to reset all values??
            for (uint8_t seg_idx = 0; seg_idx < NUM_SEGMENTS; seg_idx++)
            {
                for (uint8_t cell = 0; cell < CELLS_PER_SEGMENT; cell++)
                {
                    cell_voltage_setters[seg_idx][cell](-0.1f);
                }

                for (uint8_t thermistor = 0; thermistor < CELLS_PER_SEGMENT; thermistor++)
                {
                    thermistor_setters[seg_idx][thermistor](-1);
                }
            }
        }
    }

    return EXIT_CODE_OK;
}

ExitCode app_segments_broadcastCellVoltages(void)
{
    RETURN_IF_ERR(io_ltc6813_startCellsAdcConversion());
    io_time_delay(6);
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
    static void (*const vref_setters[NUM_SEGMENTS])(float)           = { app_canTx_BMS_Seg0_Vref_set };
    static void (*const segment_vref_ok_setters[NUM_SEGMENTS])(bool) = { app_canTx_BMS_Seg0_VrefOk_set };

    memset(segment_vref, 0, NUM_SEGMENTS * sizeof(uint16_t));
    memset(cell_temps, 0, NUM_SEGMENTS * THERMISTORS_PER_SEGMENT * sizeof(float));

    for (ThermistorMux mux = 0U; mux < THERMISTOR_MUX_COUNT; mux++)
    {
        writeThermistorMux(mux);
        io_ltc6813_writeConfigurationRegisters(segment_config);

        RETURN_IF_ERR(io_ltc6813_startThermistorsAdcConversion());
        io_time_delay(7);
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
                    segment_vref_ok_setters[segment](fabsf(voltage - 3.0f) < 0.014f);
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
    static void (*const mux_test_setters[NUM_SEGMENTS])(bool)          = { app_canTx_BMS_Seg0_MuxOk_set };
    static void (*const analog_supply_ok_setters[NUM_SEGMENTS])(bool)  = { app_canTx_BMS_Seg0_AnalogSupplyOk_set };
    static void (*const analog_supply_setters[NUM_SEGMENTS])(float)    = { app_canTx_BMS_Seg0_AnalogSupply_set };
    static void (*const digital_supply_ok_setters[NUM_SEGMENTS])(bool) = { app_canTx_BMS_Seg0_DigitalSupplyOk_set };
    static void (*const digital_supply_setters[NUM_SEGMENTS])(float)   = { app_canTx_BMS_Seg0_DigitalSupply_set };
    static void (*const rev_code_setters[NUM_SEGMENTS])(uint32_t)      = { app_canTx_BMS_Seg0_RevisionCode_set };
    static void (*const thermal_ok_setters[NUM_SEGMENTS])(bool)        = { app_canTx_BMS_Seg0_ThermalOk_set };
    static void (*const temp_setters[NUM_SEGMENTS])(uint32_t)          = { app_canTx_BMS_Seg0_DieTemp_set };

    RETURN_IF_ERR(io_ltc6813_diagnoseMUX());
    io_time_delay(1); // Empirically you need this small delay after sending DIAGN
    RETURN_IF_ERR(io_ltc6813_startInternalADCConversions());
    io_time_delay(DIAGN_CONVERSION_TIME_MS);
    io_ltc6813_getStatus(statuses, status_success_buf);

    for (uint8_t segment = 0; segment < NUM_SEGMENTS; segment++)
    {
        if (IS_EXIT_ERR(status_success_buf[segment]))
        {
            mux_test_setters[segment](false);
            rev_code_setters[segment](0);
            analog_supply_ok_setters[segment](false);
            analog_supply_setters[segment](0.0f);
            digital_supply_ok_setters[segment](false);
            digital_supply_setters[segment](0.0f);
            rev_code_setters[segment](0);
            thermal_ok_setters[segment](false);
            temp_setters[segment](0);
            continue;
        }

        rev_code_setters[segment](statuses[segment].stat_b.REV);

        const float temperature = CONVERT_100UV_TO_VOLTAGE(statuses[segment].stat_a.ITMP) / 7.6e-3f - 276.0f;
        temp_setters[segment]((uint32_t)temperature);

        const float analog_power_supply = CONVERT_100UV_TO_VOLTAGE(statuses[segment].stat_a.VA);
        analog_supply_ok_setters[segment](4.5f <= analog_power_supply && analog_power_supply <= 5.5f);
        analog_supply_setters[segment](analog_power_supply);

        const float digital_power_supply = CONVERT_100UV_TO_VOLTAGE(statuses[segment].stat_b.VD);
        digital_supply_ok_setters[segment](2.7f <= digital_power_supply && digital_power_supply <= 3.6f);
        digital_supply_setters[segment](digital_power_supply);

        thermal_ok_setters[segment](!statuses[segment].stat_b.THSD);
        mux_test_setters[segment](!(bool)statuses[segment].stat_b.MUXFAIL);
    }

    return EXIT_CODE_OK;
}

ExitCode app_segments_ADCAccuracyTest(void)
{
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
        const float adc_1_2_diff = fabsf(cell_voltages[segment][6] - cell_voltages[segment][7]);
        const float adc_3_4_diff = fabsf(cell_voltages[segment][12] - cell_voltages[segment][13]);

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
    static void (*const segment_cell_self_test_ok_setters[NUM_SEGMENTS])(
        bool) = { app_canTx_BMS_Seg0_CellSelfTestOk_set };

    RETURN_IF_ERR(io_ltc6813_sendSelfTestVoltages());
    io_time_delay(ADCV_CONVERSION_TIME_MS);
    io_ltc6813_readVoltageRegisters(voltage_regs, volt_success_buf);

    for (uint8_t segment = 0; segment < NUM_SEGMENTS; segment++)
    {
        bool self_test_ok = true;
        for (uint8_t cell = 0; cell < CELLS_PER_SEGMENT; cell++)
        {
            const bool cell_ok =
                IS_EXIT_OK(volt_success_buf[segment][cell]) && voltage_regs[segment][cell] == SELF_TEST_EXPECTED_VALUE;
            self_test_ok &= cell_ok;
        }

        segment_cell_self_test_ok_setters[segment](self_test_ok);
    }

    return EXIT_CODE_OK;
}

ExitCode app_segments_auxSelftest(void)
{
    static void (*const segment_aux_self_test_ok_setters[NUM_SEGMENTS])(
        bool) = { app_canTx_BMS_Seg0_AuxSelfTestOk_set };

    RETURN_IF_ERR(io_ltc6813_sendSelfTestAux());
    io_time_delay(ADAX_CONVERSION_TIME_MS);
    io_ltc6813_readAuxRegisters(aux_regs, aux_reg_success_buf);

    for (uint8_t segment = 0; segment < NUM_SEGMENTS; segment++)
    {
        bool self_test_ok = true;
        for (uint8_t aux_reg = 0; self_test_ok && aux_reg < AUX_REGS_PER_SEGMENT; aux_reg++)
        {
            const bool aux_ok = IS_EXIT_OK(aux_reg_success_buf[segment][aux_reg]) &&
                                aux_regs[segment][aux_reg] == SELF_TEST_EXPECTED_VALUE;
            self_test_ok &= aux_ok;
        }

        segment_aux_self_test_ok_setters[segment](self_test_ok);
    }

    return EXIT_CODE_OK;
}

ExitCode app_segments_statusSelftest(void)
{
    static void (*const segment_status_self_test_ok_setters[NUM_SEGMENTS])(
        bool) = { app_canTx_BMS_Seg0_StatusSelfTestOk_set };

    RETURN_IF_ERR(io_ltc6813_sendSelfTestStat());
    io_time_delay(ADSTAT_CONVERSION_TIME_MS);
    io_ltc6813_getStatus(statuses, status_success_buf);

    for (uint8_t segment = 0; segment < NUM_SEGMENTS; segment++)
    {
        const uint16_t *statuses_buffer = (uint16_t *)&statuses[segment];
        bool            self_test_pass  = true;

        // NOTE: only the first four words we need to check
        for (uint8_t word = 0; word < 4; word++)
        {
            self_test_pass &= statuses_buffer[word] == SELF_TEST_EXPECTED_VALUE;
        }

        segment_status_self_test_ok_setters[segment](self_test_pass);
    }

    return EXIT_CODE_OK;
}

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
