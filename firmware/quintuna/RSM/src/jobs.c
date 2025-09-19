#include "jobs.h"
// app
#include "app_canTx.h"
#include "app_canRx.h"
#include "app_commitInfo.h"
#include "app_jsoncan.h"
#include "app_coolant.h"
#include "app_brake.h"
#include "app_suspension.h"
#include "app_imu.h"
#include "app_heartbeatMonitors.h"
#include "app_jsoncan.h"

// io
#include "io_time.h"
#include "io_canTx.h"
#include "io_canQueue.h"
#include "io_coolants.h"
#include "io_potentiometer.h"
#include "io_rPump.h"
#include "io_imu.h"
#include "io_brakeLight.h"

TimerChannel               timerGPIO;
static const Potentiometer rsm_pot = {
    .i2c_handle = &r_pump_i2c,
};

CanTxQueue can_tx_queue;

static void jsoncan_transmit(const JsonCanMsg *msg)
{
    const CanMsg m = app_jsoncan_copyToCanMsg(msg);
    io_canQueue_pushTx(&can_tx_queue, &m);
}

void jobs_init(void)
{
    io_canTx_init(jsoncan_transmit);
    io_canTx_enableMode_can2(CAN2_MODE_DEFAULT, true);
    io_canQueue_initRx();
    io_canQueue_initTx(&can_tx_queue);

    app_canTx_init();
    app_canRx_init();

    app_canTx_RSM_Hash_set(GIT_COMMIT_HASH);
    app_canTx_RSM_Clean_set(GIT_COMMIT_CLEAN);
    app_canTx_RSM_Heartbeat_set(true);

    // LOG_IF_ERR(io_imu_init());
    // LOG_IF_ERR(io_rPump_isPumpReady());
    io_coolant_init();

    LOG_IF_ERR(io_rPump_isPumpReady());
}

void jobs_run1Hz_tick(void)
{
    io_canTx_enqueue1HzMsgs();
    // float flow_rate = io_coolant_getFlowRate();
    // LOG_INFO("Flow rate: %.d L/min", (int)io_coolant_getFlowRate() * 1000);
}
bool gpio_state = false;

void jobs_run100Hz_tick(void)
{
    app_coolant_broadcast();
    app_suspension_broadcast();
    // app_imu_broadcast();
    app_brake_broadcast();
    app_heartbeatMonitor_checkIn(&hb_monitor);
    app_heartbeatMonitor_broadcastFaults(&hb_monitor);

    io_brakeLight_set(app_canRx_FSM_BrakeActuated_get());
    io_canTx_enqueue100HzMsgs();

    io_potentiometer_writePercentage(&rsm_pot, (POTENTIOMETER_WIPER)WIPER0, 50);
    uint8_t data;
    io_potentiometer_readPercentage(&rsm_pot, (POTENTIOMETER_WIPER)WIPER0, &data);
}

void jobs_run1kHz_tick(void)
{
    const uint32_t ms = io_time_getCurrentMs();
    io_canTx_enqueueOtherPeriodicMsgs(ms);
}
