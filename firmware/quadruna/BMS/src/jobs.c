#include "jobs.h"

#include "app_canTx.h"
#include "app_canRx.h"
#include "app_accumulator.h"
#include "app_soc.h"
#include "app_stateMachine.h"
#include "states/app_initState.h"
#include "app_commitInfo.h"
#include "app_heartbeatMonitors.h"
#include "app_stackWaterMarks.h"
#include "app_tractiveSystem.h"

#include "io_canMsg.h"
#include "io_canQueue.h"
#include "io_jsoncan.h"
#include "io_bootHandler.h"
#include "io_ltc6813.h"

static void jsoncan_transmit(const JsonCanMsg *tx_msg)
{
    const CanMsg msg = io_jsoncan_copyToCanMsg(tx_msg);
    io_canQueue_pushTx(&msg);
}

void jobs_init(void)
{
    io_canQueue_init();
    io_canTx_init(jsoncan_transmit);
    io_canTx_enableMode(CAN_MODE_DEFAULT, true);

    app_canTx_init();
    app_canRx_init();
    app_accumulator_init();
    app_tractiveSystem_init();
    // Re-enable if auxiliary thermistors installed
    // app_thermistors_init();
    app_soc_init();

    app_stateMachine_init(app_initState_get());
    app_heartbeatMonitor_init(&hb_monitor);

    // broadcast commit info
    app_canTx_BMS_Hash_set(GIT_COMMIT_HASH);
    app_canTx_BMS_Clean_set(GIT_COMMIT_CLEAN);
    app_canTx_BMS_Heartbeat_set(true);
}
void jobs_run1Hz_tick(void)
{
    app_stackWaterMark_check();
}
void jobs_run100Hz_tick(void) {}
void jobs_run1kHz_tick(void) {}

void jobs_runCanRx_tick(void)
{
    const CanMsg rx_msg         = io_canQueue_popRx();
    JsonCanMsg   jsoncan_rx_msg = io_jsoncan_copyFromCanMsg(&rx_msg);
    io_canRx_updateRxTableWithMessage(&jsoncan_rx_msg);
}

void jobs_runCanRx_callBack(const CanMsg *rx_msg)
{
    if (io_canRx_filterMessageId(rx_msg->std_id))
    {
        io_canQueue_pushRx(rx_msg);
    }
    // check and process CAN msg for bootloader start msg
    io_bootHandler_processBootRequest(rx_msg);
}

void jobs_runLtc(void)
{
    static float cell_Voltages[NUM_SEGMENTS][CELLS_PER_SEGMENT];
    static float cell_Temps[NUM_SEGMENTS][THERMISTORS_PER_SEGMENT];

    static bool voltage_read_success[NUM_SEGMENTS][VOLTAGE_REGISTER_GROUPS] = { false };
    static bool temp_read_success[NUM_SEGMENTS][AUX_REGISTER_GROUPS]        = { false };

    static bool balance_config[NUM_SEGMENTS][CELLS_PER_SEGMENT] = { false };
    io_ltc6813_writeConfigurationRegisters(balance_config); // no balancing

    // io_ltc6813_startCellsAdcConversion(ADCSpeed_3kHz);
    // io_ltc6813_readVoltages(cell_Voltages, voltage_read_success);

    float vref = 0.0f;
    io_ltc6813_startThermistorsAdcConversion(ADCSpeed_3kHz);
    io_ltc6813_readTemperatures(cell_Temps, &vref, temp_read_success);


    osDelay(10);
}
