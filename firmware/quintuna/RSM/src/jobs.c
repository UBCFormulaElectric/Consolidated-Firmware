#include "jobs.h"
// app
#include "app_canTx.h"
#include "app_canRx.h"
#include "app_commitInfo.h"
// io
#include "io_time.h"
#include "io_canTx.h"
#include "io_canQueue.h"
#include "io_jsoncan.h"
#include "io_coolants.h"
#include "io_log.h"
#include "io_rPump.h"
#include "io_imu.h"
// testing
#include "io_leds.h"
#include "app_timer.h"

#include <stdio.h>

TimerChannel timerGPIO;

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

    ASSERT_EXIT_OK(io_rPump_isPumpReady());
    ASSERT_EXIT_OK(io_imu_init());

    app_timer_init(&timerGPIO, 100);
    app_timer_restart(&timerGPIO);
}

void jobs_run1Hz_tick(void)
{
    io_canTx_enqueue1HzMsgs();
}
bool gpio_state = false;

void jobs_run100Hz_tick(void)
{
    io_canTx_enqueue100HzMsgs();

    TimerState state = app_timer_updateAndGetState(&timerGPIO);

    // Toggle LED state based on elapsed time
    if (state == TIMER_STATE_EXPIRED)
    {
        io_led_enable(&led, !gpio_state);
        app_timer_restart(&timerGPIO);
        gpio_state = !gpio_state;
    }
}

void jobs_run1kHz_tick(void)
{
    const uint32_t ms = io_time_getCurrentMs();
    io_canTx_enqueueOtherPeriodicMsgs(ms);
}
