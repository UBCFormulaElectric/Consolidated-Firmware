#include "jobs.h"

#include "io_bootHandler.h"
#include "io_canTx.h"

#include "io_canQueue.h"
#include "io_jsoncan.h"
#include "io_canMsg.h"
#include "io_canRx.h"
#include "io_ltc6813.h"

#include <string.h>

void jobs_runLtc(void)
{
    const ADCSpeed s = ADCSpeed_3kHz;

    static float    cell_voltages[NUM_SEGMENTS][CELLS_PER_SEGMENT];
    static ExitCode volt_success_buf[NUM_SEGMENTS][VOLTAGE_REGISTER_GROUPS],
        aux_success_buf[NUM_SEGMENTS][AUX_REGISTER_GROUPS];
    ASSERT_EXIT_OK(io_ltc6813_startCellsAdcConversion(s));
    io_ltc6813_readVoltages(cell_voltages, volt_success_buf);

    // static float vref = 0.0f;
    // static float cell_temps[NUM_SEGMENTS][THERMISTORS_PER_SEGMENT];
    // ASSERT_EXIT_OK(io_ltc6813_startThermistorsAdcConversion(ADCSpeed_3kHz));
    // io_ltc6813_readTemperatures(cell_temps, &vref, aux_success_buf);

    // OPEN WIRE CHECK
    io_ltc6813CellVoltages_owcPull(PULL_UP);
    ASSERT_EXIT_OK(io_ltc6813_startCellsAdcConversion(s));
    static float owc_pucv[NUM_SEGMENTS][CELLS_PER_SEGMENT];
    io_ltc6813_readVoltages(owc_pucv, volt_success_buf);
    // perform the check
    // ReSharper disable once CppReplaceMemsetWithZeroInitialization
    static bool open_wire_cells[NUM_SEGMENTS][CELLS_PER_SEGMENT];
    memset(open_wire_cells, 0, sizeof(open_wire_cells));
    for (uint8_t segment = 0; segment < NUM_SEGMENTS; segment++)
        for (uint8_t cell = 0; cell < CELLS_PER_SEGMENT; cell++)
            // the datasheet says this, i would imagine a pull up would be > than a pull down, hence usually positive??
            open_wire_cells[segment][cell] = owc_pucv[segment][cell] - cell_voltages[segment][cell] < -0.4f;
    io_ltc6813CellVoltages_owcPull(PULL_DOWN); // reset to default

    static uint16_t voltage_regs[NUM_SEGMENTS][CELLS_PER_SEGMENT];
    io_ltc6813_sendSelfTestVoltages(s);
    io_ltc6813_readVoltageRegisters(voltage_regs, volt_success_buf);

    static uint16_t aux_regs[NUM_SEGMENTS][AUX_REGS_PER_SEGMENT];
    io_ltc6813_sendSelfTestAux(s);
    io_ltc6813_readAuxRegisters(aux_regs, aux_success_buf);

    io_ltc6813_sendSelfTestStat(s);
    io_ltc6813_diagnoseMUX();
    io_ltc6813_overlapADCTest(s);

    static LTCStatus statuses[NUM_SEGMENTS];
    static ExitCode  success[NUM_SEGMENTS];
    io_ltc6813_getStatus(statuses, success);
}

static void jsoncan_transmit_func(const JsonCanMsg *tx_msg)
{
    const CanMsg msg = io_jsoncan_copyToCanMsg(tx_msg);
    io_canQueue_pushTx(&msg);
}

void jobs_init()
{
    io_canTx_init(jsoncan_transmit_func);
    io_canQueue_init();

    io_canTx_init(jsoncan_transmit_func);
    io_canTx_enableMode(CAN_MODE_DEFAULT, true);

    static bool balance_config[NUM_SEGMENTS][CELLS_PER_SEGMENT] = { false };
    io_ltc6813_writeConfigurationRegisters(balance_config);
    io_ltc6813_writeConfigurationRegisters(balance_config);
    // ASSERT_EXIT_OK(io_ltc6813_writeConfigurationRegisters(balance_config)); // no balancing
}

void jobs_run1Hz_tick(void)
{
    io_canTx_enqueue1HzMsgs();
}

void jobs_run100Hz_tick(void)
{
    io_canTx_enqueue100HzMsgs();
}

void jobs_run1kHz_tick(void) {}

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