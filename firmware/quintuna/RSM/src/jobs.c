#include "jobs.h"
// app
#include "app_canTx.h"
#include "app_canRx.h"
#include "app_commitInfo.h"
#include "app_stateMachine.h"
#include "app_mainState.h"
// io
#include "io_time.h"
#include "io_canTx.h"
#include "io_canQueue.h"
#include "io_jsoncan.h"
#include "io_coolants.h"
#include "io_log.h"
#include "io_rPump.h"
#include "io_imu.h"
#include "io_potentiometer.h"
// testing
#include "io_leds.h"
#include "app_timer.h"

#include <stdint.h>
#include <stdio.h>
#include "hw_pwmInputFreqOnly.h"
#include "hw_pwms.h"

TimerChannel timerGPIO;
static const Potentiometer rsm_pot = {
    .i2c_handle = &r_pump_i2c,
};


static void jsoncan_transmit(const JsonCanMsg *msg)
{
    const CanMsg m = io_jsoncan_copyToCanMsg(msg);
    io_canQueue_pushTx(&m);
}

void jobs_init(void)
{
    app_canTx_init();
    app_canRx_init();

    app_canTx_RSM_Hash_set(GIT_COMMIT_HASH);
    app_canTx_RSM_Clean_set(GIT_COMMIT_CLEAN);
    app_canTx_RSM_Heartbeat_set(true);

    io_canTx_init(jsoncan_transmit);
    io_canTx_enableMode(CAN_MODE_DEFAULT, true);
    io_canQueue_init();
    io_coolant_init();

    //ASSERT_EXIT_OK(io_rPump_isPumpReady());
    //ASSERT_EXIT_OK(io_imu_init());

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
    io_canTx_enqueue100HzMsgs();


    io_potentiometer_writePercentage(&rsm_pot, (POTENTIOMETER_WIPER) WIPER0, 50);
    uint8_t data;
    io_potentiometer_readPercentage(&rsm_pot, (POTENTIOMETER_WIPER) WIPER0, &data);
}

void jobs_run1kHz_tick(void)
{
    const uint32_t ms = io_time_getCurrentMs();
    io_canTx_enqueueOtherPeriodicMsgs(ms);
}
