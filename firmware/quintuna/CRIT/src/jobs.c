#include "jobs.h"

// App
#include "app_commitInfo.h"
#include "app_utils.h"
#include "app_canTx.h"
#include "app_canRx.h"
#include "app_heartbeatMonitors.h"

// IO
#include "io_canTx.h"
#include "io_time.h"
#include "io_canQueue.h"
#include "io_jsoncan.h"

static void canTransmit(const JsonCanMsg *msg)
{
    UNUSED(msg);
    CanMsg tx_msg = io_jsoncan_copyToCanMsg(msg);
    io_canQueue_pushTx(&tx_msg);
}

void jobs_init(void)
{
    // can
    io_canTx_init(canTransmit); // TODO this needs to be more sophisticated for multiple busses
    io_canQueue_init();
    io_canTx_enableMode_can2(CAN2_MODE_DEFAULT, true);
    app_canTx_init();
    app_canRx_init();

    app_heartbeatMonitor_init(&hb_monitor);

    // broadcast commit info
    app_canTx_CRIT_Hash_set(GIT_COMMIT_HASH);
    app_canTx_CRIT_Clean_set(GIT_COMMIT_CLEAN);
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
