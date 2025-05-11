#include "app_segments.h"
#include "app_canTx.h"

#include "io_ltc6813.h"
#include "io_time.h"

static const ADCSpeed s = ADCSpeed_7kHz;

static float    cell_voltages[NUM_SEGMENTS][CELLS_PER_SEGMENT];
static uint16_t voltage_regs[NUM_SEGMENTS][CELLS_PER_SEGMENT];

static ExitCode volt_success_buf[NUM_SEGMENTS][VOLTAGE_REGISTER_GROUPS];
static ExitCode aux_success_buf[NUM_SEGMENTS][AUX_REGISTER_GROUPS];

static const void (*cellVoltageSetters[NUM_SEGMENTS][CELLS_PER_SEGMENT])(float) = {
    { app_canTx_BMS_Seg0_Cell0_Voltage_set, app_canTx_BMS_Seg0_Cell1_Voltage_set, app_canTx_BMS_Seg0_Cell2_Voltage_set,
      app_canTx_BMS_Seg0_Cell3_Voltage_set, app_canTx_BMS_Seg0_Cell4_Voltage_set, app_canTx_BMS_Seg0_Cell5_Voltage_set,
      app_canTx_BMS_Seg0_Cell6_Voltage_set, app_canTx_BMS_Seg0_Cell7_Voltage_set, app_canTx_BMS_Seg0_Cell8_Voltage_set,
      app_canTx_BMS_Seg0_Cell9_Voltage_set, app_canTx_BMS_Seg0_Cell10_Voltage_set,
      app_canTx_BMS_Seg0_Cell11_Voltage_set, app_canTx_BMS_Seg0_Cell12_Voltage_set,
      app_canTx_BMS_Seg0_Cell13_Voltage_set }
};

static const void (*cellOWCSetters[NUM_SEGMENTS][CELLS_PER_SEGMENT])(float) = {
    // { app_canTx_BMS_Cell0_OWC_set, app_canTx_BMS_Cell1_OWC_set, app_canTx_BMS_Cell2_OWC_set,
    //   app_canTx_BMS_Cell3_OWC_set, app_canTx_BMS_Cell4_OWC_set, app_canTx_BMS_Cell5_OWC_set,
    //   app_canTx_BMS_Cell6_OWC_set, app_canTx_BMS_Cell7_OWC_set, app_canTx_BMS_Cell8_OWC_set,
    //   app_canTx_BMS_Cell9_OWC_set, app_canTx_BMS_Cell10_OWC_set, app_canTx_BMS_Cell11_OWC_set,
    //   app_canTx_BMS_Cell12_OWC_set, app_canTx_BMS_Cell13_OWC_set }
    { app_canTx_BMS_Seg1_Cell0_Voltage_set, app_canTx_BMS_Seg1_Cell1_Voltage_set, app_canTx_BMS_Seg1_Cell2_Voltage_set,
      app_canTx_BMS_Seg1_Cell3_Voltage_set, app_canTx_BMS_Seg1_Cell4_Voltage_set, app_canTx_BMS_Seg1_Cell5_Voltage_set,
      app_canTx_BMS_Seg1_Cell6_Voltage_set, app_canTx_BMS_Seg1_Cell7_Voltage_set, app_canTx_BMS_Seg1_Cell8_Voltage_set,
      app_canTx_BMS_Seg1_Cell9_Voltage_set, app_canTx_BMS_Seg1_Cell10_Voltage_set,
      app_canTx_BMS_Seg1_Cell11_Voltage_set, app_canTx_BMS_Seg1_Cell12_Voltage_set,
      app_canTx_BMS_Seg1_Cell13_Voltage_set }
};

// test setters
static const void (*segmentVoltageSelfTestSetters[NUM_SEGMENTS])(bool)  = { app_canTx_BMS_Self_Test_Voltages_set };
static const void (*segmentAuxRegSelfTestSetters[NUM_SEGMENTS])(bool)   = { app_canTx_BMS_Self_Test_Aux_set };
static const void (*segmentOverlapADC12TestSetters[NUM_SEGMENTS])(bool) = { app_canTx_BMS_Overlap_ADC_1_2_Test_set };
static const void (*segmentOverlapADC23TestSetters[NUM_SEGMENTS])(bool) = { app_canTx_BMS_Overlap_ADC_2_3_Test_set };

void app_segments_readVoltages()
{
    ASSERT_EXIT_OK(io_ltc6813_startCellsAdcConversion(s));
    io_time_delay(10);
    io_ltc6813_readVoltages(cell_voltages, volt_success_buf);

    // for debug mode
    for (uint8_t segment = 0; segment < NUM_SEGMENTS; segment++)
        for (uint8_t cell = 0; cell < CELLS_PER_SEGMENT; cell++)
            cellVoltageSetters[segment][cell](cell_voltages[segment][cell]);
}

void app_overlap_adc_test()
{
    ASSERT_EXIT_OK(io_ltc6813_overlapADCTest(s));
    io_time_delay(10);
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

void app_voltage_selftest()
{
    ASSERT_EXIT_OK(io_ltc6813_sendSelfTestVoltages(s));
    io_time_delay(10);
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

void app_aux_selftest()
{
    static uint16_t aux_regs[NUM_SEGMENTS][AUX_REGS_PER_SEGMENT];
    io_ltc6813_sendSelfTestAux(s);
    io_time_delay(10);
    io_ltc6813_readAuxRegisters(aux_regs, aux_success_buf);
    for (uint8_t segment = 0; segment < NUM_SEGMENTS; segment++)
    {
        bool self_test_ok = true;
        for (uint8_t aux_reg = 0; aux_reg < AUX_REGS_PER_SEGMENT; aux_reg++)
        {
            const bool aux_ok = voltage_regs[segment][aux_reg] == io_ltc6813_selfTestExpectedValue(s);
            self_test_ok &= aux_ok;
        }
        segmentAuxRegSelfTestSetters[segment](self_test_ok);
    }
}