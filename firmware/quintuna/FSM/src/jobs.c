#include "jobs.h"
// app
#include "app_commitInfo.h"
#include "app_canTx.h"
#include "app_canRx.h"
// io
#include "io_time.h"
#include "io_canTx.h"
#include "io_canQueue.h"
#include "io_jsoncan.h"

static void canTransmit(const JsonCanMsg *msg)
{
    const CanMsg m = io_jsoncan_copyToCanMsg(msg);
    io_canQueue_pushTx(&m);
}

void jobs_init(void)
{
    // can
    io_canTx_init(canTransmit);
    io_canTx_enableMode_Can2(CAN2_MODE_DEFAULT, true);
    io_canQueue_init();
    app_canTx_init();
    app_canRx_init();

    // broadcast commit info
    app_canTx_FSM_Hash_set(GIT_COMMIT_HASH);
    app_canTx_FSM_Clean_set(GIT_COMMIT_CLEAN);
}

void jobs_run1Hz_tick(void)
{
    // debug mode
    const bool debug_mode_enabled = app_canRx_Debug_EnableDebugMode_get();
    io_canTx_enableMode_Can2(CAN2_MODE_DEBUG, debug_mode_enabled);
    // cantx
    io_canTx_enqueue1HzMsgs();
}

void jobs_run100Hz_tick(void)
{
    // cantx
    io_canTx_enqueue100HzMsgs();
}

void jobs_run1kHz_tick(void)
{
    // cantx
    const uint32_t task_start_ms = io_time_getCurrentMs();
    io_canTx_enqueueOtherPeriodicMsgs(task_start_ms);

    // Watchdog check-in must be the last function called before putting the
    // task to sleep. Prevent check in if the elapsed period is greater or
    // equal to the period ms
    // if (io_time_getCurrentMs() - task_start_ms <= 1)
    // {
    // }
}
