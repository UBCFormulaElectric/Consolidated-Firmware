#include "app_segments.h"
#include "app_segments_internal.h"
#include "app_thermistor.h"
#include "app_thermistors.h"
#include "utils.h"
#include "io_ltc6813.h"
#include <app_canTx.h>
#include <string.h>
#include <float.h>
#include <stdlib.h>

#define CONVERT_100UV_TO_VOLTAGE(v_100uv) ((float)v_100uv * 1E-4f)
#define CONVERT_VOLTAGE_TO_100UV(v) ((uint16_t)(v * 1E4f))

#define VREF_AUX_REG 5

float     pack_voltage;
CellParam max_cell_voltage;
CellParam min_cell_voltage;
CellParam max_cell_temp;
CellParam min_cell_temp;

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

                const ExitCode exit = aux_regs_success[mux][segment][aux_gpio];
                if (IS_EXIT_ERR(exit))
                {
                    cell_temps_success[segment][thermistor] = exit;
                    continue;
                }

                const float voltage = CONVERT_100UV_TO_VOLTAGE(aux_regs[mux][segment][aux_gpio]);
                const bool  success = IS_EXIT_OK(aux_regs_success[mux][segment][aux_gpio]);
                if (aux_gpio == VREF_AUX_REG)
                {
                    if (success)
                    {
                        segment_vref[segment] = voltage;
                        vref_setters[segment](voltage);
                        segment_vref_ok_setters[segment](fabsf(voltage - 3.0f) < 0.014f);
                    }
                    else
                    {
                        segment_vref[segment] = -1.0f;
                        vref_setters[segment](-1.0f);
                        segment_vref_ok_setters[segment](false);
                    }
                    continue;
                }

                if (!success)
                {
                    cell_temps_success[segment][thermistor] = false;
                    thermistor_setters[segment][thermistor](-1.0f);
                    continue;
                }

                // TODO: Use measured VREF instead of constant
                const float current    = (3.0f - voltage) / 10e3f;
                const float resistance = voltage / current;
                const float temp       = app_thermistor_resistanceToTemp(resistance, &ltc_thermistor_lut);

                cell_temps[segment][thermistor]         = temp;
                cell_temps_success[segment][thermistor] = EXIT_CODE_OK;
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

    bool overall_ok = true;

    for (uint8_t segment = 0; segment < NUM_SEGMENTS; segment++)
    {
        const bool owc_valid_first   = IS_EXIT_OK(owc_pucv_success[segment][0]);
        const bool owc_passing_first = owc_valid_first && owc_pucv_regs[segment][0] != 0;

        overall_ok &= owc_passing_first;
        owc_results_success[segment][0] = owc_valid_first;
        owc_results[segment][0]         = owc_passing_first;

        cell_owc_setters[segment][0](owc_passing_first);

        for (uint8_t cell = 0; cell < CELLS_PER_SEGMENT - 1; cell++)
        {
            const bool owc_valid =
                IS_EXIT_OK(owc_pucv_success[segment][cell]) && IS_EXIT_OK(owc_pdcv_success[segment][cell]);
            const bool owc_passing = owc_valid && owc_pdcv_regs[segment][cell] - owc_pucv_regs[segment][cell] <=
                                                      CONVERT_VOLTAGE_TO_100UV(0.4f);

            overall_ok &= owc_passing;
            owc_results_success[segment][cell + 1] = owc_valid;
            owc_results[segment][cell + 1]         = owc_passing;

            cell_owc_setters[segment][cell + 1](owc_passing);
        }

        // Treat our last cell as described for Cell 18 in the datasheet. Datasheet doesn't really make it clear that
        // this what you should do when using <18 cells, but this is the only way I could get it to work (and I think
        // makes sense since there's no higher cell to pull it up to?).
        const bool owc_valid_last   = IS_EXIT_OK(owc_pdcv_success[segment][CELLS_PER_SEGMENT - 1]);
        const bool owc_passing_last = owc_valid_last && owc_pdcv_regs[segment][CELLS_PER_SEGMENT - 1] != 0;

        overall_ok &= owc_passing_last;
        owc_results_success[segment][CELL_TAPS_PER_SEGMENT - 1] = owc_valid_last;
        owc_results[segment][CELL_TAPS_PER_SEGMENT - 1]         = owc_passing_last;

        cell_owc_setters[segment][CELL_TAPS_PER_SEGMENT - 1](owc_passing_last);
        comm_ok_setters[segment](ALL_COMM_OK(segment));
    }

    app_canTx_BMS_OpenWireCheckStatus_set(overall_ok);
}

void app_segments_broadcastVoltageStats(void)
{
    float     tmp_pack_voltage     = 0.0f;
    CellParam tmp_min_cell_voltage = { .value = FLT_MAX, .segment = 0, .cell = 0 };
    CellParam tmp_max_cell_voltage = { .value = 0.0f, .segment = 0, .cell = 0 };

    memset(voltage_comm_ok, true, sizeof(voltage_comm_ok));

    bool any_valid = false;
    for (uint8_t segment = 0U; segment < NUM_SEGMENTS; segment++)
    {
        float segment_voltage = 0.0f;
        for (uint8_t cell = 0U; cell < CELLS_PER_SEGMENT; cell++)
        {
            const bool success = IS_EXIT_OK(cell_voltage_success[segment][cell]);
            voltage_comm_ok[segment] &= success;

            const CellParam cell_voltage = { .value = cell_voltages[segment][cell], .segment = segment, .cell = cell };

            if (success)
            {
                any_valid = true;
            }

            // Sum the voltages
            tmp_pack_voltage += cell_voltage.value;
            segment_voltage += cell_voltage.value;

            // Get the maximum cell voltage
            if (success && cell_voltage.value > tmp_max_cell_voltage.value)
            {
                tmp_max_cell_voltage = cell_voltage;
            }

            // Get the minimum cell voltage
            if (success && cell_voltage.value < tmp_min_cell_voltage.value)
            {
                tmp_min_cell_voltage = cell_voltage;
            }
        }

        comm_ok_setters[segment](ALL_COMM_OK(segment));
        avg_segment_voltage_setters[segment](segment_voltage / CELLS_PER_SEGMENT);
    }

    if (any_valid)
    {
        pack_voltage     = tmp_pack_voltage;
        min_cell_voltage = tmp_min_cell_voltage;
        max_cell_voltage = tmp_max_cell_voltage;

        app_canTx_BMS_MaxCellVoltageSegment_set(max_cell_voltage.segment);
        app_canTx_BMS_MaxCellVoltageIdx_set(max_cell_voltage.cell);
        app_canTx_BMS_MaxCellVoltage_set(max_cell_voltage.value);
        app_canTx_BMS_MinCellVoltageSegment_set(min_cell_voltage.segment);
        app_canTx_BMS_MinCellVoltageIdx_set(min_cell_voltage.cell);
        app_canTx_BMS_MinCellVoltage_set(min_cell_voltage.value);
        app_canTx_BMS_PackVoltage_set(pack_voltage);
        app_canTx_BMS_AvgCellVoltage_set(pack_voltage / (NUM_SEGMENTS * CELLS_PER_SEGMENT));
        app_canTx_BMS_ImbalanceMagnitude_set(fabsf(max_cell_voltage.value - min_cell_voltage.value));
    }
}

void app_segments_broadcastTempStats(void)
{
    float     temp_sum          = 0.0f;
    CellParam tmp_min_cell_temp = { .value = FLT_MAX, .segment = 0, .cell = 0 };
    CellParam tmp_max_cell_temp = { .value = 0.0f, .segment = 0, .cell = 0 };

    memset(temp_comm_ok, true, sizeof(temp_comm_ok));

    bool any_valid = false;
    for (uint8_t segment = 0U; segment < NUM_SEGMENTS; segment++)
    {
        float segment_temp = 0.0f;
        for (uint8_t cell = 0U; cell < CELLS_PER_SEGMENT; cell++)
        {
            const bool success = IS_EXIT_OK(cell_temps_success[segment][cell]);
            temp_comm_ok[segment] &= success;

            const CellParam cell_temp = { .value = cell_temps[segment][cell], .segment = segment, .cell = cell };

            if (success)
            {
                any_valid = true;
            }

            // Sum the temps
            temp_sum += cell_temp.value;
            segment_temp += cell_temp.value;

            // Get the maximum cell temp
            if (success && cell_temp.value > tmp_max_cell_temp.value)
            {
                tmp_max_cell_temp = cell_temp;
            }

            // Get the minimum cell temp
            const bool inshallah = segment == 8 && cell == 12;
            if (success && cell_temp.value < tmp_min_cell_temp.value && !inshallah)
            {
                tmp_min_cell_temp = cell_temp;
            }
        }

        comm_ok_setters[segment](ALL_COMM_OK(segment));
        avg_segment_temp_setters[segment](segment_temp / CELLS_PER_SEGMENT);
    }

    if (any_valid)
    {
        min_cell_temp = tmp_min_cell_temp;
        max_cell_temp = tmp_max_cell_temp;

        app_canTx_BMS_MaxCellTempSegment_set(max_cell_temp.segment);
        app_canTx_BMS_MaxCellTempIdx_set(max_cell_temp.cell);
        app_canTx_BMS_MaxCellTemp_set(max_cell_temp.value);
        app_canTx_BMS_MinCellTempSegment_set(min_cell_temp.segment);
        app_canTx_BMS_MinCellTempIdx_set(min_cell_temp.cell);
        app_canTx_BMS_MinCellTemp_set(min_cell_temp.value);
        app_canTx_BMS_AvgCellTemp_set(temp_sum / (NUM_SEGMENTS * CELLS_PER_SEGMENT));
    }
}
