#include "jobs.h"

// App
#include "app_commitInfo.h"
#include "app_utils.h"
#include "app_canTx.h"
#include "app_canRx.h"
#include "app_heartbeatMonitors.h"
#include "screens/app_screens.h"
#include "app_leds.h"
#include "app_switches.h"
#include "app_jsoncan.h"

// IO
#include "io_bootHandler.h"
#include "io_canMsg.h"
#include "io_canTx.h"
#include "io_time.h"
#include "io_canQueue.h"
#include "io_shift_register.h"

// HW
#include "hw_gpios.h"

CanTxQueue can_tx_queue;

static void canTransmit(const JsonCanMsg *msg)
{
    const CanMsg tx_msg = app_jsoncan_copyToCanMsg(msg);
    io_canQueue_pushTx(&can_tx_queue, &tx_msg);
}

void jobs_init(void)
{
    // can
    io_canTx_init(canTransmit);
    io_canTx_enableMode_can2(CAN2_MODE_DEFAULT, true);
    io_canQueue_initRx();
    io_canQueue_initTx(&can_tx_queue);

    app_canTx_init();
    app_canRx_init();

    app_heartbeatMonitor_init(&hb_monitor);

    app_canTx_CRIT_Hash_set(GIT_COMMIT_HASH);
    app_canTx_CRIT_Clean_set(GIT_COMMIT_CLEAN);
    app_canTx_CRIT_Heartbeat_set(true);

    io_shift_register_led_init();
    app_screens_init();
}

void jobs_run1Hz_tick(void)
{
    const bool debug_mode_enabled = app_canRx_Debug_EnableDebugMode_get();
    io_canTx_enableMode_can2(CAN2_MODE_DEBUG, debug_mode_enabled);
    io_canTx_enqueue1HzMsgs();
}

void jobs_run100Hz_tick(void)
{
    app_heartbeatMonitor_checkIn(&hb_monitor);
    app_heartbeatMonitor_broadcastFaults(&hb_monitor);

    app_leds_update();
    app_screens_update();
    app_switches_broadcast();

    io_canTx_enqueue100HzMsgs();
}

void jobs_run1kHz_tick(void)
{
    // Check in for timeouts for all RTOS tasks
    const uint32_t task_start_ms = io_time_getCurrentMs();
    io_canTx_enqueueOtherPeriodicMsgs(task_start_ms);

    // Watchdog check-in must be the last function called before putting the
    // task to sleep. Prevent check in if the elapsed period is greater or
    // equal to the period ms
    if (io_time_getCurrentMs() - task_start_ms <= 1)
    {
    }
}
