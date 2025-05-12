#include "app_segments.h"
#include "app_canTx.h"
#include "app_thermistors.h"

#include "io_ltc6813.h"
#include "io_time.h"

#include <string.h>

static const ADCSpeed s = ADCSpeed_7kHz;

static float segment_vref[NUM_SEGMENTS] = { 0.0f };

// This buffer is only used for storing valid cell voltage data
// TODO convert at last moment, most always keep as uint16_t
static float cell_voltages[NUM_SEGMENTS][CELLS_PER_SEGMENT];
// dump whatever you want in here fr
static uint16_t voltage_regs[NUM_SEGMENTS][CELLS_PER_SEGMENT];
static ExitCode volt_success_buf[NUM_SEGMENTS][VOLTAGE_REGISTER_GROUPS];

// This buffer is only used for storing valid thermistor data
// TODO convert at last moment, most always keep as uint16_t
static float cell_temps[NUM_SEGMENTS][THERMISTORS_PER_SEGMENT];
// dump whatever you want in here fr
static uint16_t aux_regs[NUM_SEGMENTS][AUX_REGS_PER_SEGMENT];
static ExitCode temp_success_buf[NUM_SEGMENTS][AUX_REG_GROUPS * 2];
static ExitCode aux_reg_success_buf[NUM_SEGMENTS][AUX_REG_GROUPS];

static void (*const cellVoltageSetters[NUM_SEGMENTS][CELLS_PER_SEGMENT])(float) = {
    { app_canTx_BMS_Seg0_Cell0_Voltage_set, app_canTx_BMS_Seg0_Cell1_Voltage_set, app_canTx_BMS_Seg0_Cell2_Voltage_set,
      app_canTx_BMS_Seg0_Cell3_Voltage_set, app_canTx_BMS_Seg0_Cell4_Voltage_set, app_canTx_BMS_Seg0_Cell5_Voltage_set,
      app_canTx_BMS_Seg0_Cell6_Voltage_set, app_canTx_BMS_Seg0_Cell7_Voltage_set, app_canTx_BMS_Seg0_Cell8_Voltage_set,
      app_canTx_BMS_Seg0_Cell9_Voltage_set, app_canTx_BMS_Seg0_Cell10_Voltage_set,
      app_canTx_BMS_Seg0_Cell11_Voltage_set, app_canTx_BMS_Seg0_Cell12_Voltage_set,
      app_canTx_BMS_Seg0_Cell13_Voltage_set }
};

static void (*const thermistorSetters[NUM_SEGMENTS][THERMISTORS_PER_SEGMENT])(float) = { { 0 } }; // TODO populate

static void (*const cellOWCSetters[NUM_SEGMENTS][CELLS_PER_SEGMENT])(bool) = {
    { app_canTx_BMS_Cell0_OWC_set, app_canTx_BMS_Cell1_OWC_set, app_canTx_BMS_Cell2_OWC_set,
      app_canTx_BMS_Cell3_OWC_set, app_canTx_BMS_Cell4_OWC_set, app_canTx_BMS_Cell5_OWC_set,
      app_canTx_BMS_Cell6_OWC_set, app_canTx_BMS_Cell7_OWC_set, app_canTx_BMS_Cell8_OWC_set,
      app_canTx_BMS_Cell9_OWC_set, app_canTx_BMS_Cell10_OWC_set, app_canTx_BMS_Cell11_OWC_set,
      app_canTx_BMS_Cell12_OWC_set, app_canTx_BMS_Cell13_OWC_set }
    // { app_canTx_BMS_Seg1_Cell0_Voltage_set, app_canTx_BMS_Seg1_Cell1_Voltage_set,
    // app_canTx_BMS_Seg1_Cell2_Voltage_set,
    //   app_canTx_BMS_Seg1_Cell3_Voltage_set, app_canTx_BMS_Seg1_Cell4_Voltage_set,
    //   app_canTx_BMS_Seg1_Cell5_Voltage_set, app_canTx_BMS_Seg1_Cell6_Voltage_set,
    //   app_canTx_BMS_Seg1_Cell7_Voltage_set, app_canTx_BMS_Seg1_Cell8_Voltage_set,
    //   app_canTx_BMS_Seg1_Cell9_Voltage_set, app_canTx_BMS_Seg1_Cell10_Voltage_set,
    //   app_canTx_BMS_Seg1_Cell11_Voltage_set, app_canTx_BMS_Seg1_Cell12_Voltage_set,
    //   app_canTx_BMS_Seg1_Cell13_Voltage_set }
};

// test setters
static void (*const segmentVoltageSelfTestSetters[NUM_SEGMENTS])(bool)  = { app_canTx_BMS_Self_Test_Voltages_set };
static void (*const segmentAuxRegSelfTestSetters[NUM_SEGMENTS])(bool)   = { app_canTx_BMS_Self_Test_Aux_set };
static void (*const segmentVRefTrueSetters[NUM_SEGMENTS])(bool)         = { app_canTx_BMS_VREF_Test_set };
static void (*const segmentOverlapADC12TestSetters[NUM_SEGMENTS])(bool) = { app_canTx_BMS_Overlap_ADC_1_2_Test_set };
static void (*const segmentOverlapADC23TestSetters[NUM_SEGMENTS])(bool) = { app_canTx_BMS_Overlap_ADC_2_3_Test_set };

void app_segments_broadcastVoltages()
{
    ASSERT_EXIT_OK(io_ltc6813_startCellsAdcConversion(s));
    io_time_delay(10); // TODO tweak timings
    io_ltc6813_readVoltages(cell_voltages, volt_success_buf);

    // for debug mode
    for (uint8_t segment = 0; segment < NUM_SEGMENTS; segment++)
        for (uint8_t cell = 0; cell < CELLS_PER_SEGMENT; cell++)
            cellVoltageSetters[segment][cell](cell_voltages[segment][cell]);
}

void app_segments_broadcastTempsVRef()
{
    ASSERT_EXIT_OK(io_ltc6813_startThermistorsAdcConversion(s));
    io_time_delay(10); // TODO tweak timings
    memset(cell_temps, 0, NUM_SEGMENTS * THERMISTORS_PER_SEGMENT * sizeof(float));
    memset(segment_vref, 0, NUM_SEGMENTS * sizeof(float));

    for (uint8_t mux = 0; mux < 2; mux++)
    {
        // TODO flip a mux based on i == 0 and i == 1
        io_ltc6813_readAuxRegisters(aux_regs, aux_reg_success_buf);
        for (uint8_t segment = 0U; segment < NUM_SEGMENTS; segment++)
        {
            for (uint8_t aux_gpio = 0U; aux_gpio < AUX_REGS_PER_SEGMENT; aux_gpio++)
            {
                temp_success_buf[segment][aux_gpio / 3 + AUX_REG_GROUPS * mux] =
                    aux_reg_success_buf[segment][aux_gpio / 3];
                if (IS_EXIT_ERR(aux_reg_success_buf[segment][aux_gpio / 3]))
                {
                    continue;
                }
                if (aux_gpio == 5)
                {
                    segment_vref[segment] = (float)aux_regs[segment][aux_gpio] * 1E-4F;
                    continue;
                }
                const uint8_t adj = aux_gpio >= 6 ? 1 : 0;
                cell_temps[segment][aux_gpio - adj + THERMISTORS_PER_SEGMENT / 2 * mux] =
                    app_thermistor_resistanceToTemp(aux_regs[segment][aux_gpio], &ltc_thermistor_lut);
            }
        }
    }

    // for debug mode
    // for (uint8_t segment = 0; segment < NUM_SEGMENTS; segment++)
    //     for (uint8_t therm = 0; therm < THERMISTORS_PER_SEGMENT; therm++)
    //         thermistorSetters[segment][therm](cell_temps[segment][therm]);
}

void app_segments_ADCAccuracyTest()
{
    ASSERT_EXIT_OK(io_ltc6813_overlapADCTest(s));
    io_time_delay(10);                                        // TODO tweak timings
    io_ltc6813_readVoltages(cell_voltages, volt_success_buf); // TODO make this more efficient

    // for debug mode
    for (uint8_t segment = 0; segment < NUM_SEGMENTS; segment++)
    {
        // 6, 7 and 12, 13
        const float vref_diff    = fabsf(segment_vref[segment] - 3.0f),
                    adc_1_2_diff = fabsf(cell_voltages[segment][6] - cell_voltages[segment][7]),
                    adc_3_4_diff = fabsf(cell_voltages[segment][12] - cell_voltages[segment][13]);
        segmentVRefTrueSetters[segment](vref_diff < 0.014f);
        segmentOverlapADC12TestSetters[segment](adc_1_2_diff < 0.001f);
        segmentOverlapADC23TestSetters[segment](adc_3_4_diff < 0.001f);
    }
}

void app_segments_voltageSelftest()
{
    ASSERT_EXIT_OK(io_ltc6813_sendSelfTestVoltages(s));
    io_time_delay(10); // TODO tweak timings
    io_ltc6813_readVoltageRegisters(voltage_regs, volt_success_buf);
    for (uint8_t segment = 0; segment < NUM_SEGMENTS; segment++)
    {
        bool self_test_ok = true;
        for (uint8_t cell = 0; cell < CELLS_PER_SEGMENT; cell++)
        {
            const bool cell_ok = voltage_regs[segment][cell] == io_ltc6813_selfTestExpectedValue(s);
            self_test_ok &= cell_ok;
        }
        segmentVoltageSelfTestSetters[segment](self_test_ok);
    }
}

void app_segments_auxSelftest()
{
    ASSERT_EXIT_OK(io_ltc6813_sendSelfTestAux(s));
    io_time_delay(10); // TODO tweak timings
    io_ltc6813_readAuxRegisters(aux_regs, aux_reg_success_buf);
    for (uint8_t segment = 0; segment < NUM_SEGMENTS; segment++)
    {
        bool self_test_ok = true;
        for (uint8_t aux_reg = 0; self_test_ok && aux_reg < AUX_REGS_PER_SEGMENT; aux_reg++)
        {
            const bool aux_ok = aux_regs[segment][aux_reg] == io_ltc6813_selfTestExpectedValue(s);
            self_test_ok &= aux_ok;
        }
        segmentAuxRegSelfTestSetters[segment](self_test_ok);
    }
}

void app_segments_statusSelftest()
{
    ASSERT_EXIT_OK(io_ltc6813_sendSelfTestStat(s));
    io_time_delay(10); // TODO tweak timings
    static LTCStatus statuses[NUM_SEGMENTS];
    static ExitCode  success[NUM_SEGMENTS];
    io_ltc6813_getStatus(statuses, success);
}

void app_segments_openWireCheck()
{
    // data collection
    static float owc_pucv[NUM_SEGMENTS][CELLS_PER_SEGMENT];
    ASSERT_EXIT_OK(io_ltc6813CellVoltages_owcPull(PULL_UP, ADCSpeed_26Hz, false));
    // io_time_delay(201);
    ASSERT_EXIT_OK(io_ltc6813CellVoltages_owcPull(PULL_UP, ADCSpeed_26Hz, false));
    // io_time_delay(201);
    ASSERT_EXIT_OK(io_ltc6813_startCellsAdcConversion(ADCSpeed_26Hz));
    io_time_delay(201);
    io_ltc6813_readVoltages(owc_pucv, volt_success_buf);

    // reset back to normal
    static float owc_pdcv[NUM_SEGMENTS][CELLS_PER_SEGMENT];
    ASSERT_EXIT_OK(io_ltc6813CellVoltages_owcPull(PULL_DOWN, ADCSpeed_26Hz, false));
    // io_time_delay(201);
    ASSERT_EXIT_OK(io_ltc6813CellVoltages_owcPull(PULL_DOWN, ADCSpeed_26Hz, false));
    // io_time_delay(201);
    ASSERT_EXIT_OK(io_ltc6813_startCellsAdcConversion(ADCSpeed_26Hz));
    io_time_delay(201);
    io_ltc6813_readVoltages(owc_pdcv, volt_success_buf);

    // perform the check
    for (uint8_t segment = 0; segment < NUM_SEGMENTS; segment++)
    {
        cellOWCSetters[segment][0](owc_pucv[segment][0] == 0);
        for (uint8_t cell = 1; cell < CELLS_PER_SEGMENT; cell++)
            cellOWCSetters[segment][cell](owc_pucv[segment][cell] - owc_pdcv[segment][cell] < -0.4f);
#if CELLS_PER_SEGMENT >= 18
        completeness cellOWCSetters[segment][17](owc_pdcv[segment][cell] == 0.0f);
#endif
    }
}