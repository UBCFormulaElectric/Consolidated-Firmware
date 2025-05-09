#include "jobs.h"

#include "io_bootHandler.h"
#include "io_canTx.h"

#include "io_canQueue.h"
#include "io_jsoncan.h"
#include "io_canMsg.h"
#include <io_canRx.h>
#include "io_ltc6813.h"

void jobs_runLtc(void)
{
    static float cell_voltages[NUM_SEGMENTS][CELLS_PER_SEGMENT];
    static float cell_temps[NUM_SEGMENTS][THERMISTORS_PER_SEGMENT];

    static bool voltage_read_success[NUM_SEGMENTS][VOLTAGE_REGISTER_GROUPS] = { false };
    static bool temp_read_success[NUM_SEGMENTS][AUX_REGISTER_GROUPS]        = { false };

    static bool balance_config[NUM_SEGMENTS][CELLS_PER_SEGMENT] = { false };
    io_ltc6813_writeConfigurationRegisters(balance_config); // no balancing

    io_ltc6813_startCellsAdcConversion(ADCSpeed_3kHz);
    io_ltc6813_readVoltages(cell_voltages, voltage_read_success);

    float vref = 0.0f;
    io_ltc6813_startThermistorsAdcConversion(ADCSpeed_3kHz);
    io_ltc6813_readTemperatures(cell_temps, &vref, temp_read_success);
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