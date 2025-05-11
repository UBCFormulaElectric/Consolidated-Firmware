#include "app_segments.h"
#include "app_canTx.h"

#include "io_ltc6813.h"
#include "io_time.h"

static const ADCSpeed s = ADCSpeed_7kHz;

static float vref = 0.0f;

// This buffer is only used for storing valid cell voltage data
static float cell_voltages[NUM_SEGMENTS][CELLS_PER_SEGMENT];
// dump whatever you want in here fr
static uint16_t voltage_regs[NUM_SEGMENTS][CELLS_PER_SEGMENT];

// This buffer is only used for storing valid thermistor data
static float cell_temps[NUM_SEGMENTS][THERMISTORS_PER_SEGMENT];
// dump whatever you want in here fr
static uint16_t aux_regs[NUM_SEGMENTS][AUX_REGS_PER_SEGMENT];

static ExitCode volt_success_buf[NUM_SEGMENTS][VOLTAGE_REGISTER_GROUPS];
// i think there is some casting nonsense you can pull here
static ExitCode temp_success_buf[NUM_SEGMENTS][AUX_REGISTER_GROUPS * 2];

static void (*const cellVoltageSetters[NUM_SEGMENTS][CELLS_PER_SEGMENT])(float) = {
    { app_canTx_BMS_Seg0_Cell0_Voltage_set, app_canTx_BMS_Seg0_Cell1_Voltage_set, app_canTx_BMS_Seg0_Cell2_Voltage_set,
      app_canTx_BMS_Seg0_Cell3_Voltage_set, app_canTx_BMS_Seg0_Cell4_Voltage_set, app_canTx_BMS_Seg0_Cell5_Voltage_set,
      app_canTx_BMS_Seg0_Cell6_Voltage_set, app_canTx_BMS_Seg0_Cell7_Voltage_set, app_canTx_BMS_Seg0_Cell8_Voltage_set,
      app_canTx_BMS_Seg0_Cell9_Voltage_set, app_canTx_BMS_Seg0_Cell10_Voltage_set,
      app_canTx_BMS_Seg0_Cell11_Voltage_set, app_canTx_BMS_Seg0_Cell12_Voltage_set,
      app_canTx_BMS_Seg0_Cell13_Voltage_set }
};

static void (*const thermistorSetters[NUM_SEGMENTS][THERMISTORS_PER_SEGMENT])(float) = { 0 }; // TODO populate

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
    assert(false);
    // ASSERT_EXIT_OK(io_ltc6813_startThermistorsAdcConversion(s));
    // io_time_delay(10); // TODO tweak timings
    // io_ltc6813_readTemperatures(cell_temps, &vref, temp_success_buf);
    //
    // // for debug mode
    // for (uint8_t segment = 0; segment < NUM_SEGMENTS; segment++)
    //     for (uint8_t therm = 0; therm < THERMISTORS_PER_SEGMENT; therm++)
    //         thermistorSetters[segment][therm](cell_temps[segment][therm]);
}

void app_segments_overlapADCTest()
{
    ASSERT_EXIT_OK(io_ltc6813_overlapADCTest(s));
    io_time_delay(10);                                        // TODO tweak timings
    io_ltc6813_readVoltages(cell_voltages, volt_success_buf); // TODO make this more efficient

    // for debug mode
    for (uint8_t segment = 0; segment < NUM_SEGMENTS; segment++)
    {
        // 6, 7 and 12, 13
        const float adc_1_2_diff = fabsf(cell_voltages[segment][6] - cell_voltages[segment][7]),
                    adc_3_4_diff = fabsf(cell_voltages[segment][12] - cell_voltages[segment][13]);
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
    assert(false);
    // ASSERT_EXIT_OK(io_ltc6813_sendSelfTestAux(s));
    // io_time_delay(10); // TODO tweak timings
    // io_ltc6813_readAuxRegisters(aux_regs, temp_success_buf);
    // for (uint8_t segment = 0; segment < NUM_SEGMENTS; segment++)
    // {
    //     bool self_test_ok = true;
    //     for (uint8_t aux_reg = 0; aux_reg < AUX_REGS_PER_SEGMENT; aux_reg++)
    //     {
    //         const bool aux_ok = voltage_regs[segment][aux_reg] == io_ltc6813_selfTestExpectedValue(s);
    //         self_test_ok &= aux_ok;
    //     }
    //     segmentAuxRegSelfTestSetters[segment](self_test_ok);
    // }
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