#include "app_segments.h"
#include "app_canTx.h"
#include "app_thermistors.h"
#include "app_segments_setters.h"

#include "app_utils.h"
#include "io_log.h"
#include "io_ltc6813.h"
#include "io_time.h"

#include <hw_utils.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// TODO: Take a closer look at these guys
#define CONVERSION_TIME_MS (10)
#define OWC_CONVERSION_TIME_MS (201)

#define VUV (0x619U) // 2.5V Under-voltage threshold = (VUV + 1) * 16 * 100uV
#define VOV (0xA41U) // 4.2V Over-voltage threshold = VOV * 16 * 100uV

#define VREF_AUX_REG 5
#define NUM_CONFIG_SYNC_TRIES 20

#define CONVERT_100UV_TO_VOLTAGE(v_100uv) ((float)v_100uv * 1E-4f)
#define CONVERT_VOLTAGE_TO_100UV(v) ((uint16_t)(v * 1E4f))

typedef enum
{
    THERMISTOR_MUX_0_7,
    THERMISTOR_MUX_8_13,
    THERMISTOR_MUX_COUNT,
} ThermistorMux;

typedef struct
{
    uint8_t segment;
    int cell;
    float   temp;
} CellTemperature;
typedef struct
{
    CellTemperature min_thermistor_temp;
    CellTemperature max_thermistor_temp;
} TemperatureStats;

static TemperatureStats temperature_stats;

// Keeping separate buffers for every command is pretty wasteful but we have lots of RAM so oh well

static SegmentConfig segment_config[NUM_SEGMENTS];
static SegmentConfig read_segment_config[NUM_SEGMENTS];
static ExitCode      config_success_buf[NUM_SEGMENTS];

static uint16_t cell_voltage_regs[NUM_SEGMENTS][CELLS_PER_SEGMENT];
static ExitCode cell_voltage_success[NUM_SEGMENTS][CELLS_PER_SEGMENT];
static float    cell_voltages[NUM_SEGMENTS][CELLS_PER_SEGMENT];

static uint16_t aux_regs[THERMISTOR_MUX_COUNT][NUM_SEGMENTS][AUX_REGS_PER_SEGMENT];
static ExitCode aux_regs_success[THERMISTOR_MUX_COUNT][NUM_SEGMENTS][AUX_REGS_PER_SEGMENT];
static float    segment_vref[NUM_SEGMENTS];
static float    cell_temps[NUM_SEGMENTS][THERMISTORS_PER_SEGMENT];

static StatusRegGroups status_regs[NUM_SEGMENTS];
static ExitCode        status_regs_success[NUM_SEGMENTS];

static uint16_t adc_accuracy_regs[NUM_SEGMENTS][CELLS_PER_SEGMENT];
static ExitCode adc_accuracy_success[NUM_SEGMENTS][CELLS_PER_SEGMENT];

static uint16_t voltage_self_test_regs[NUM_SEGMENTS][CELLS_PER_SEGMENT];
static ExitCode voltage_self_test_success[NUM_SEGMENTS][CELLS_PER_SEGMENT];

static uint16_t aux_self_test_regs[NUM_SEGMENTS][AUX_REGS_PER_SEGMENT];
static ExitCode aux_self_test_reg_success[NUM_SEGMENTS][AUX_REGS_PER_SEGMENT];

static StatusRegGroups status_self_test_regs[NUM_SEGMENTS];
static ExitCode        status_self_test_success[NUM_SEGMENTS];

static uint16_t owc_pucv_regs[NUM_SEGMENTS][CELLS_PER_SEGMENT];
static ExitCode owc_pucv_success[NUM_SEGMENTS][CELLS_PER_SEGMENT];
static uint16_t owc_pdcv_regs[NUM_SEGMENTS][CELLS_PER_SEGMENT];
static ExitCode owc_pdcv_success[NUM_SEGMENTS][CELLS_PER_SEGMENT];
static bool     owc_results[NUM_SEGMENTS][CELL_TAPS_PER_SEGMENT];
static bool     owc_results_success[NUM_SEGMENTS][CELL_TAPS_PER_SEGMENT];

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

        bool all_ok = true;
        for (uint8_t seg = 0; seg < NUM_SEGMENTS; seg++)
        {
            if (IS_EXIT_ERR(config_success_buf[seg]))
            {
                all_ok = false;
            }
        }

        if (all_ok)
        {
            break;
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
    for (int tries = 0; tries < NUM_CONFIG_SYNC_TRIES; tries++)
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

    return EXIT_CODE_TIMEOUT;
}

ExitCode app_segments_runVoltageConversion(void)
{
    RETURN_IF_ERR(io_ltc6813_startCellsAdcConversion());
    io_time_delay(CONVERSION_TIME_MS);
    io_ltc6813_readVoltageRegisters(cell_voltage_regs, cell_voltage_success);
    return EXIT_CODE_OK;
}

ExitCode app_segments_runAuxConversion(void)
{
    for (ThermistorMux mux = 0U; mux < THERMISTOR_MUX_COUNT; mux++)
    {
        writeThermistorMux(mux);
        io_ltc6813_writeConfigurationRegisters(segment_config);

        RETURN_IF_ERR(io_ltc6813_startThermistorsAdcConversion());
        io_time_delay(CONVERSION_TIME_MS);
        io_ltc6813_readAuxRegisters(aux_regs[mux], aux_regs_success[mux]);
    }

    return EXIT_CODE_OK;
}

ExitCode app_segments_runStatusConversion(void)
{
    RETURN_IF_ERR(io_ltc6813_diagnoseMUX());
    io_time_delay(1); // Empirically you need this small delay after sending DIAGN
    RETURN_IF_ERR(io_ltc6813_startInternalADCConversions());
    io_time_delay(CONVERSION_TIME_MS);
    io_ltc6813_getStatus(status_regs, status_regs_success);
    return EXIT_CODE_OK;
}

ExitCode app_segments_runAdcAccuracyTest(void)
{
    RETURN_IF_ERR(io_ltc6813_overlapADCTest());
    io_time_delay(CONVERSION_TIME_MS);
    io_ltc6813_readVoltageRegisters(adc_accuracy_regs, adc_accuracy_success);
    return EXIT_CODE_OK;
}

ExitCode app_segments_runVoltageSelfTest(void)
{
    RETURN_IF_ERR(io_ltc6813_sendSelfTestVoltages());
    io_time_delay(CONVERSION_TIME_MS);
    io_ltc6813_readVoltageRegisters(voltage_self_test_regs, voltage_self_test_success);
    return EXIT_CODE_OK;
}

ExitCode app_segments_runAuxSelfTest(void)
{
    RETURN_IF_ERR(io_ltc6813_sendSelfTestAux());
    io_time_delay(CONVERSION_TIME_MS);
    io_ltc6813_readAuxRegisters(aux_self_test_regs, aux_self_test_reg_success);
    return EXIT_CODE_OK;
}

ExitCode app_segments_runStatusSelfTest(void)
{
    RETURN_IF_ERR(io_ltc6813_sendSelfTestStat());
    io_time_delay(CONVERSION_TIME_MS);
    io_ltc6813_getStatus(status_self_test_regs, status_self_test_success);
    return EXIT_CODE_OK;
}

ExitCode app_segments_runOpenWireCheck(void)
{
    RETURN_IF_ERR(io_ltc6813_owcPull(PULL_UP));
    io_time_delay(OWC_CONVERSION_TIME_MS);
    RETURN_IF_ERR(io_ltc6813_owcPull(PULL_UP));
    io_time_delay(OWC_CONVERSION_TIME_MS);
    io_ltc6813_readVoltageRegisters(owc_pucv_regs, owc_pucv_success);

    RETURN_IF_ERR(io_ltc6813_owcPull(PULL_DOWN));
    io_time_delay(OWC_CONVERSION_TIME_MS);
    RETURN_IF_ERR(io_ltc6813_owcPull(PULL_DOWN));
    io_time_delay(OWC_CONVERSION_TIME_MS);
    io_ltc6813_readVoltageRegisters(owc_pdcv_regs, owc_pdcv_success);

    return EXIT_CODE_OK;
}

void app_segments_broadcastCellVoltages(void)
{
    for (uint8_t segment = 0; segment < NUM_SEGMENTS; segment++)
    {
        for (uint8_t cell = 0; cell < CELLS_PER_SEGMENT; cell++)
        {
            if (IS_EXIT_ERR(cell_voltage_success[segment][cell]))
            {
                // we claim that this is insufficient to raise module comm errors
                continue;
            }

            // see page 68, 0xffff is invalid (either not populated or faulted)
            if (cell_voltage_regs[segment][cell] == 0xffff)
            {
                // -0.1V over CAN means invalid
                cell_voltage_setters[segment][cell](-0.1f);
                cell_voltage_success[segment][cell] = EXIT_CODE_ERROR;
                continue;
            }

            const float voltage          = CONVERT_100UV_TO_VOLTAGE(cell_voltage_regs[segment][cell]);
            cell_voltages[segment][cell] = voltage;
            cell_voltage_setters[segment][cell](voltage);
        }
    }
}

void app_segments_updateTemperatureStats(float cell_temp, uint8_t segment, int cell_number)
{
    if (cell_temp > temperature_stats.max_thermistor_temp.temp)
    {
        temperature_stats.max_thermistor_temp.temp = cell_temp;
        temperature_stats.max_thermistor_temp.segment = segment;
        temperature_stats.max_thermistor_temp.cell = cell_number;
        app_canTx_BMS_MaxTempSegment_set(segment);
        app_canTx_BMS_MaxTempIdx_set((uint8_t)cell_number);
    }
    if (cell_temp < temperature_stats.min_thermistor_temp.temp)
    {
        temperature_stats.min_thermistor_temp.temp = cell_temp;
        temperature_stats.min_thermistor_temp.segment = segment;
        temperature_stats.min_thermistor_temp.cell = cell_number;
        app_canTx_BMS_MinTempSegment_set(segment);
        app_canTx_BMS_MinTempIdx_set((uint8_t)cell_number);
    }
}

void app_segments_broadcastTempsVRef(void)
{
    memset(segment_vref, 0, NUM_SEGMENTS * sizeof(uint16_t));
    memset(cell_temps, 0, NUM_SEGMENTS * THERMISTORS_PER_SEGMENT * sizeof(float));

    for (ThermistorMux mux = 0U; mux < THERMISTOR_MUX_COUNT; mux++)
    {
        for (uint8_t segment = 0U; segment < NUM_SEGMENTS; segment++)
        {
            for (uint8_t aux_gpio = 0U; aux_gpio < AUX_REGS_PER_SEGMENT; aux_gpio++)
            {
                const uint8_t vref_reg_adjustment =
                    aux_gpio > VREF_AUX_REG ? 1
                                            : 0; // Annoyingly the VREF reg is right in the middle of GPIO voltage regs
                const uint8_t mux_offset =
                    mux == THERMISTOR_MUX_8_13 ? 8 : 0; // 8 GPIOs used for least significant MUX setting
                const int thermistor = aux_gpio - vref_reg_adjustment + mux_offset;

                if (thermistor >= THERMISTORS_PER_SEGMENT)
                {
                    continue;
                }

                if (IS_EXIT_ERR(aux_regs_success[mux][segment][aux_gpio]))
                {
                    continue;
                }

                const float voltage = CONVERT_100UV_TO_VOLTAGE(aux_regs[mux][segment][aux_gpio]);
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
                app_segments_updateTemperatureStats(temp, segment, thermistor);
                thermistor_setters[segment][thermistor](temp);
            }
        }
    }
}

void app_segments_broadcastStatus(void)
{
    for (uint8_t segment = 0; segment < NUM_SEGMENTS; segment++)
    {
        if (IS_EXIT_ERR(status_regs_success[segment]))
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

        rev_code_setters[segment](status_regs[segment].stat_b.REV);

        const float temperature = CONVERT_100UV_TO_VOLTAGE(status_regs[segment].stat_a.ITMP) / 7.6e-3f - 276.0f;
        temp_setters[segment]((uint32_t)temperature);

        const float analog_power_supply = CONVERT_100UV_TO_VOLTAGE(status_regs[segment].stat_a.VA);
        analog_supply_ok_setters[segment](4.5f <= analog_power_supply && analog_power_supply <= 5.5f);
        analog_supply_setters[segment](analog_power_supply);

        const float digital_power_supply = CONVERT_100UV_TO_VOLTAGE(status_regs[segment].stat_b.VD);
        digital_supply_ok_setters[segment](2.7f <= digital_power_supply && digital_power_supply <= 3.6f);
        digital_supply_setters[segment](digital_power_supply);

        thermal_ok_setters[segment](!status_regs[segment].stat_b.THSD);
        mux_test_setters[segment](!(bool)status_regs[segment].stat_b.MUXFAIL);
    }
}

void app_segments_broadcastAdcAccuracyTest(void)
{
    for (uint8_t segment = 0; segment < NUM_SEGMENTS; segment++)
    {
        // See section "Overlap Cell Measurement (ADOL Command)".
        const uint16_t adc_1_2_diff = (uint16_t)abs(adc_accuracy_regs[segment][6] - adc_accuracy_regs[segment][7]);
        const uint16_t adc_3_4_diff = (uint16_t)abs(adc_accuracy_regs[segment][12] - adc_accuracy_regs[segment][13]);

        const bool adc_1_2_fail =
            IS_EXIT_ERR(adc_accuracy_success[segment][6]) || IS_EXIT_ERR(adc_accuracy_success[segment][7]);
        segment_overlap_adc1_2_test_setters[segment](!adc_1_2_fail && adc_1_2_diff < CONVERT_VOLTAGE_TO_100UV(0.001f));

        const bool adc_2_3_fail =
            IS_EXIT_ERR(adc_accuracy_success[segment][12]) || IS_EXIT_ERR(adc_accuracy_success[segment][13]);
        segment_overalap_adc2_3_test_setters[segment](!adc_2_3_fail && adc_3_4_diff < CONVERT_VOLTAGE_TO_100UV(0.001f));
    }
}

void app_segments_broadcastVoltageSelfTest(void)
{
    for (uint8_t segment = 0; segment < NUM_SEGMENTS; segment++)
    {
        bool self_test_ok = true;
        for (uint8_t cell = 0; cell < CELLS_PER_SEGMENT; cell++)
        {
            const bool cell_ok = IS_EXIT_OK(voltage_self_test_success[segment][cell]) &&
                                 voltage_self_test_regs[segment][cell] == SELF_TEST_EXPECTED_VALUE;
            self_test_ok &= cell_ok;
        }

        segment_cell_self_test_ok_setters[segment](self_test_ok);
    }
}

void app_segments_broadcastAuxSelfTest(void)
{
    for (uint8_t segment = 0; segment < NUM_SEGMENTS; segment++)
    {
        bool self_test_ok = true;
        for (uint8_t aux_reg = 0; self_test_ok && aux_reg < AUX_REGS_PER_SEGMENT; aux_reg++)
        {
            const bool aux_ok = IS_EXIT_OK(aux_self_test_reg_success[segment][aux_reg]) &&
                                aux_self_test_regs[segment][aux_reg] == SELF_TEST_EXPECTED_VALUE;
            self_test_ok &= aux_ok;
        }

        segment_aux_self_test_ok_setters[segment](self_test_ok);
    }
}

void app_segments_broadcastStatusSelfTest(void)
{
    for (uint8_t segment = 0; segment < NUM_SEGMENTS; segment++)
    {
        const uint16_t *statuses_buffer = (uint16_t *)&status_self_test_regs[segment];
        bool            self_test_pass  = true;

        // NOTE: only the first four words we need to check
        for (uint8_t word = 0; word < 4; word++)
        {
            self_test_pass &=
                IS_EXIT_OK(status_self_test_success[segment]) && statuses_buffer[word] == SELF_TEST_EXPECTED_VALUE;
        }

        segment_status_self_test_ok_setters[segment](self_test_pass);
    }
}

void app_segments_broadcastOpenWireCheck(void)
{
    // See "Open Wire Check (ADOW Command)" in datasheet for this works.
    // Known limitation: If >=2x adjacent cells are open wire, it only reports the lowest one.

    for (uint8_t segment = 0; segment < NUM_SEGMENTS; segment++)
    {
        const bool owc_valid_first      = IS_EXIT_OK(owc_pucv_success[segment][0]);
        const bool owc_passing_first    = owc_valid_first && owc_pucv_regs[segment][0] != 0;
        owc_results_success[segment][0] = owc_valid_first;
        owc_results[segment][0]         = owc_passing_first;
        cell_owc_setters[segment][0](owc_passing_first);

        for (uint8_t cell = 0; cell < CELLS_PER_SEGMENT - 1; cell++)
        {
            const bool owc_valid =
                IS_EXIT_OK(owc_pucv_success[segment][cell]) && IS_EXIT_OK(owc_pdcv_success[segment][cell]);
            const bool owc_passing = owc_valid && owc_pdcv_regs[segment][cell] - owc_pucv_regs[segment][cell] <=
                                                      CONVERT_VOLTAGE_TO_100UV(0.4f);

            owc_results_success[segment][cell + 1] = owc_valid;
            owc_results[segment][cell + 1]         = owc_passing;
            cell_owc_setters[segment][cell + 1](owc_passing);
        }

        // Treat our last cell as described for Cell 18 in the datasheet. Datasheet doesn't really make it clear that
        // this what you should do when using <18 cells, but this is the only way I could get it to work (and I think
        // makes sense since there's no higher cell to pull it up to?).
        const bool owc_valid_last   = IS_EXIT_OK(owc_pdcv_success[segment][CELLS_PER_SEGMENT - 1]);
        const bool owc_passing_last = owc_valid_last && owc_pdcv_regs[segment][CELLS_PER_SEGMENT - 1] != 0;
        owc_results_success[segment][CELL_TAPS_PER_SEGMENT - 1] = owc_valid_last;
        owc_results[segment][CELL_TAPS_PER_SEGMENT - 1]         = owc_passing_last;
        cell_owc_setters[segment][CELL_TAPS_PER_SEGMENT - 1](owc_passing_last);
    }
}

// TODO: Combine this and getMaxCellVoltage into one "update voltage stats" function or something
float app_segments_getPackVoltage()
{
    float pack_voltage = 0.0;
    for (uint8_t segment = 0U; segment < NUM_SEGMENTS; segment++)
    {
        for (uint8_t cell = 0U; cell < CELLS_PER_SEGMENT; cell++)
        {
            // Sum the voltages
            const float cell_voltage = cell_voltages[segment][cell];
            pack_voltage += cell_voltage;
        }
    }
    return pack_voltage;
}

// TODO: Combine getPackVoltage and this into one "update voltage stats" function or something
float app_segments_getMaxCellVoltage()
{
    // Find the min and max voltages
    float max_cell_voltage = 0.0f;
    for (uint8_t segment = 0U; segment < NUM_SEGMENTS; segment++)
    {
        for (uint8_t cell = 0U; cell < CELLS_PER_SEGMENT; cell++)
        {
            // Collect each cell voltage to find the max
            const float cell_voltage = cell_voltages[segment][cell];
            // Get the maximum cell voltage
            if (cell_voltage > max_cell_voltage)
            {
                max_cell_voltage = cell_voltage;
            }
        }
    }
    return max_cell_voltage;
}