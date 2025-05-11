#include "jobs.h"

#include "app_canTx.h"
#include "app_canRx.h"
#include "io_canTx.h"
#include "io_canRx.h"

#include "app_segments.h"

#include "io_bootHandler.h"
#include "io_canQueue.h"
#include "io_jsoncan.h"
#include "io_canMsg.h"
#include "io_ltc6813.h"
#include "io_time.h"

#include <string.h>

#include <hw_utils.h>

void jobs_runLtc(void)
{
    app_segments_readVoltages();
    app_overlap_adc_test();
    app_voltage_selftest();
    app_aux_selftest();

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