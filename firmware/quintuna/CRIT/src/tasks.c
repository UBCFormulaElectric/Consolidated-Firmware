#include "tasks.h"
#include "cmsis_os.h"
#include "shared.pb.h"

// io
#include "io_log.h"
#include "io_jsoncan.h"

// hw
#include "hw_utils.h"
#include "hw_hardFaultHandler.h"

// jsoncan stuff
#include "app_commitInfo.h"
#include "app_canRx.h"
#include "app_canTx.h"
#include "io_canTx.h"
#include "io_canRx.h"

void tasks_preInit() {}

static void canTransmit(const JsonCanMsg *msg)
{
    UNUSED(msg);
}

void tasks_init()
{
    // Configure and initialize SEGGER SystemView.
    // NOTE: Needs to be done after clock config!
    SEGGER_SYSVIEW_Conf();
    LOG_INFO("CRIT reset!");

    // Re-enable watchdog.
    __HAL_DBGMCU_FREEZE_IWDG();
    hw_hardFaultHandler_init();

    // can
    io_canTx_init(canTransmit); // TODO this needs to be more sophisticated for multiple busses
    io_canTx_enableMode_Can(CAN_MODE_DEFAULT, true);
    app_canTx_init();
    app_canRx_init();
    // broadcast commit info
    app_canTx_CRIT_Hash_set(GIT_COMMIT_HASH);
    app_canTx_CRIT_Clean_set(GIT_COMMIT_CLEAN);
}

void tasks_runCanTx()
{
    // Setup tasks.
    for (;;)
    {
    }
}

void tasks_runCanRx()
{
    // Setup tasks.
    for (;;)
    {
        JsonCanMsg jsoncan_rx_msg;
        io_canRx_updateRxTableWithMessage(&jsoncan_rx_msg);
    }
}

void tasks_run1Hz()
{
    // Setup tasks.
    static const TickType_t period_ms = 1000U;

    static uint32_t start_ticks = 0;
    start_ticks                 = osKernelGetTickCount();

    for (;;)
    {
        // hw_stackWaterMarkConfig_check();

        const bool debug_mode_enabled = app_canRx_Debug_EnableDebugMode_get();
        io_canTx_enableMode_Can(CAN_MODE_DEBUG, debug_mode_enabled);
        io_canTx_enqueue1HzMsgs();

        start_ticks += period_ms;
        osDelayUntil(start_ticks);
    }
}

void tasks_run100Hz()
{
    // Setup tasks.
    static const TickType_t period_ms = 10;

    static uint32_t start_ticks = 0;
    start_ticks                 = osKernelGetTickCount();

    for (;;)
    {
        io_canTx_enqueue100HzMsgs();
        start_ticks += period_ms;
        osDelayUntil(start_ticks);
    }
}

void tasks_run1kHz()
{
    // Setup tasks.
    static const TickType_t period_ms = 1;

    static uint32_t start_ticks = 0;
    start_ticks                 = osKernelGetTickCount();

    /* Infinite loop */
    for (;;)
    {
        // Check in for timeouts for all RTOS tasks
        const uint32_t task_start_ms = TICK_TO_MS(osKernelGetTickCount());
        io_canTx_enqueueOtherPeriodicMsgs(task_start_ms);

        // Watchdog check-in must be the last function called before putting the
        // task to sleep. Prevent check in if the elapsed period is greater or
        // equal to the period ms
        if ((TICK_TO_MS(osKernelGetTickCount()) - task_start_ms) <= period_ms)
        {
        }

        start_ticks += period_ms;
        osDelayUntil(start_ticks);
    }
}
