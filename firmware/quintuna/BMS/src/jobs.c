#include "jobs.h"

#include "app_canTx.h"
#include "app_canRx.h"
#include "io_canTx.h"
#include "io_canRx.h"

#include "io_bootHandler.h"
#include "io_canQueue.h"
#include "io_jsoncan.h"
#include "io_canMsg.h"
#include "io_ltc6813.h"
#include "io_time.h"

#include <string.h>

#include <cmsis_os2.h>
#include <hw_utils.h>

// void *voltages(float value)
void (*cellVoltageSetters[NUM_SEGMENTS][CELLS_PER_SEGMENT])(float) = {
    { app_canTx_BMS_Seg0_Cell0_Voltage_set, app_canTx_BMS_Seg0_Cell1_Voltage_set, app_canTx_BMS_Seg0_Cell2_Voltage_set,
      app_canTx_BMS_Seg0_Cell3_Voltage_set, app_canTx_BMS_Seg0_Cell4_Voltage_set, app_canTx_BMS_Seg0_Cell5_Voltage_set,
      app_canTx_BMS_Seg0_Cell6_Voltage_set, app_canTx_BMS_Seg0_Cell7_Voltage_set, app_canTx_BMS_Seg0_Cell8_Voltage_set,
      app_canTx_BMS_Seg0_Cell9_Voltage_set, app_canTx_BMS_Seg0_Cell10_Voltage_set,
      app_canTx_BMS_Seg0_Cell11_Voltage_set, app_canTx_BMS_Seg0_Cell12_Voltage_set,
      app_canTx_BMS_Seg0_Cell13_Voltage_set }
};

void (*cellOWCSetters[NUM_SEGMENTS][CELLS_PER_SEGMENT])(float) = {
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

void (*segmentVoltageSelfTestSetters[NUM_SEGMENTS])(bool) = { app_canTx_BMS_Self_Test_Voltages_set };
void (*segmentAuxRegSelfTestSetters[NUM_SEGMENTS])(bool)  = { app_canTx_BMS_Self_Test_Aux_set };

void (*segmentOverlapADC12TestSetters[NUM_SEGMENTS])(bool) = {
    app_canTx_BMS_Overlap_ADC_1_2_Test_set,
};
void (*segmentOverlapADC23TestSetters[NUM_SEGMENTS])(bool) = {
    app_canTx_BMS_Overlap_ADC_2_3_Test_set,
};

void jobs_runLtc(void)
{
    const ADCSpeed s = ADCSpeed_7kHz;

    static float    cell_voltages[NUM_SEGMENTS][CELLS_PER_SEGMENT];
    static ExitCode volt_success_buf[NUM_SEGMENTS][VOLTAGE_REGISTER_GROUPS],
        aux_success_buf[NUM_SEGMENTS][AUX_REGISTER_GROUPS];
    ASSERT_EXIT_OK(io_ltc6813_startCellsAdcConversion(s));
    osDelay(10);
    io_ltc6813_readVoltages(cell_voltages, volt_success_buf);
    for (uint8_t segment = 0; segment < NUM_SEGMENTS; segment++)
        for (uint8_t cell = 0; cell < CELLS_PER_SEGMENT; cell++)
            cellVoltageSetters[segment][cell](cell_voltages[segment][cell]);

    // static float vref = 0.0f;
    // static float cell_temps[NUM_SEGMENTS][THERMISTORS_PER_SEGMENT];
    // ASSERT_EXIT_OK(io_ltc6813_startThermistorsAdcConversion(ADCSpeed_3kHz));
    // io_ltc6813_readTemperatures(cell_temps, &vref, aux_success_buf);

    // OPEN WIRE CHECK
    // data collection
    // static float owc_pucv[NUM_SEGMENTS][CELLS_PER_SEGMENT];
    // ASSERT_EXIT_OK(io_ltc6813CellVoltages_owcPull(PULL_UP, ADCSpeed_26Hz, false));
    // ASSERT_EXIT_OK(io_ltc6813CellVoltages_owcPull(PULL_UP, ADCSpeed_26Hz));
    // ASSERT_EXIT_OK(io_ltc6813_startCellsAdcConversion(ADCSpeed_26Hz));
    // osDelay(10);
    // io_ltc6813_readVoltages(owc_pucv, volt_success_buf);

    // perform the check
    //     for (uint8_t segment = 0; segment < NUM_SEGMENTS; segment++)
    //     {
    //         cellOWCSetters[segment][0](owc_pucv[segment][0]); // == 0
    //         for (uint8_t cell = 1; cell < CELLS_PER_SEGMENT; cell++)
    //             // the datasheet says this, i would imagine a pull up would be > than a pull down, hence usually
    //             // positive??
    //             cellOWCSetters[segment][cell](owc_pucv[segment][cell] - cell_voltages[segment][cell]); // < -0.4.f
    // #if CELLS_PER_SEGMENT >= 18                                                                        // for
    //         completeness cellOWCSetters[segment][17](cell_voltages[segment][cell] == 0.0f);
    // #endif
    //     }
    // ASSERT_EXIT_OK(io_ltc6813CellVoltages_owcPull(PULL_DOWN, ADCSpeed_26Hz));
    // ASSERT_EXIT_OK(io_ltc6813CellVoltages_owcPull(PULL_DOWN, ADCSpeed_26Hz));
    // osDelay(10);

    ASSERT_EXIT_OK(io_ltc6813_overlapADCTest(s));
    osDelay(10);
    io_ltc6813_readVoltages(cell_voltages, volt_success_buf); // TODO make this more efficient
    for (uint8_t segment = 0; segment < NUM_SEGMENTS; segment++)
    {
        // 6, 7 and 12, 13
        const float adc_1_2_diff = fabsf(cell_voltages[segment][6] - cell_voltages[segment][7]),
                    adc_3_4_diff = fabsf(cell_voltages[segment][12] - cell_voltages[segment][13]);
        segmentOverlapADC12TestSetters[segment](adc_1_2_diff < 0.001f);
        segmentOverlapADC23TestSetters[segment](adc_3_4_diff < 0.001f);
    }

    static uint16_t voltage_regs[NUM_SEGMENTS][CELLS_PER_SEGMENT];
    ASSERT_EXIT_OK(io_ltc6813_sendSelfTestVoltages(s));
    osDelay(10);
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

    static uint16_t aux_regs[NUM_SEGMENTS][AUX_REGS_PER_SEGMENT];
    io_ltc6813_sendSelfTestAux(s);
    osDelay(10);
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

    // io_ltc6813_sendSelfTestStat(s);

    // io_ltc6813_diagnoseMUX();
    // static LTCStatus statuses[NUM_SEGMENTS];
    // static ExitCode  success[NUM_SEGMENTS];
    // io_ltc6813_getStatus(statuses, success);
}

static void jsoncan_transmit_func(const JsonCanMsg *tx_msg)
{
    const CanMsg msg = io_jsoncan_copyToCanMsg(tx_msg);
    io_canQueue_pushTx(&msg);
}

void jobs_init()
{
    io_canQueue_init();
    io_canTx_init(jsoncan_transmit_func);
    io_canTx_enableMode(CAN_MODE_DEFAULT, true);

    static bool balance_config[NUM_SEGMENTS][CELLS_PER_SEGMENT] = { false };
    io_ltc6813_writeConfigurationRegisters(balance_config);
    io_ltc6813_writeConfigurationRegisters(balance_config);
    ASSERT_EXIT_OK(io_ltc6813_writeConfigurationRegisters(balance_config)); // no balancing
}

void jobs_run1Hz_tick(void)
{
    io_canTx_enqueue1HzMsgs();
}

void jobs_run100Hz_tick(void)
{
    const bool debug_mode_enabled = app_canRx_Debug_EnableDebugMode_get();
    io_canTx_enableMode(CAN_MODE_DEBUG, debug_mode_enabled);
    io_canTx_enqueue100HzMsgs();
}

void jobs_run1kHz_tick(void)
{
    io_canTx_enqueueOtherPeriodicMsgs(io_time_getCurrentMs());
}

void jobs_runCanRx_tick(void)
{
    const CanMsg rx_msg       = io_canQueue_popRx();
    JsonCanMsg   json_can_msg = io_jsoncan_copyFromCanMsg(&rx_msg);
    io_canRx_updateRxTableWithMessage(&json_can_msg);
}

void jobs_canRxCallback(const CanMsg *rx_msg)
{
    if (io_canRx_filterMessageId(rx_msg->std_id))
    {
        io_canQueue_pushRx(rx_msg);
    }

    io_bootHandler_processBootRequest(rx_msg);
}