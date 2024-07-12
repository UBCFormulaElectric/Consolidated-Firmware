#include "tasks.h"
#include "cmsis_os.h"

// app
#include "app_mainState.h"

// io
#include "io_log.h"
#include "io_jsoncan.h"
#include "io_canMsgQueues.h"

// hw
#include "hw_utils.h"
#include "hw_bootup.h"
#include "hw_hardFaultHandler.h"
#include "hw_cans.h"
#include "hw_adcs.h"

// jsoncan stuff
extern "C"
{
#include "app_commitInfo.h"
#include "app_canRx.h"
#include "app_canTx.h"
#include "io_canTx.h"
#include "io_canRx.h"
}

void tasks_preInit(void)
{
    hw::bootup::enableInterruptsForApp();
}

void tasks_init(void)
{
    // Configure and initialize SEGGER SystemView.
    // NOTE: Needs to be done after clock config!
    SEGGER_SYSVIEW_Conf();
    LOG_INFO("CRIT reset!");

    // Re-enable watchdog.
    // __HAL_DBGMCU_FREEZE_IWDG();
    // hw_watchdog_init(hw_watchdogConfig_refresh, hw_watchdogConfig_timeoutCallback);
    hw_hardFaultHandler_init();
    hw::can::can1.init();
    hw::adc::adc1.init();

    // io_chimera_init(&debug_uart, GpioNetName_crit_net_name_tag, AdcNetName_crit_net_name_tag, &n_chimera_pin);
    io_canTx_init(
        [](const JsonCanMsg *msg)
        {
            hw::can::CanMsg tx_msg{};
            io::jsoncan::copyToCanMsg(msg, &tx_msg);
            io::can1queue.pushTxMsgToQueue(&tx_msg);
        });
    io_canTx_enableMode(CAN_MODE_DEFAULT, true);
    io::can1queue.init();

    app_canTx_init();
    app_canRx_init();

    app::StateMachine::init(&app::critstates::main_state);

    // broadcast commit info
    app_canTx_CRIT_Hash_set(GIT_COMMIT_HASH);
    app_canTx_CRIT_Clean_set(GIT_COMMIT_CLEAN);
}

void tasks_runCanTx(void)
{
    // io_chimera_sleepTaskIfEnabled();

    // Setup tasks.
    for (;;)
    {
        hw::can::CanMsg tx_msg = io::can1queue.popTxMsgFromQueue();
        hw::can::can1.transmit(&tx_msg);
    }
}

void tasks_runCanRx(void)
{
    // io_chimera_sleepTaskIfEnabled();

    // Setup tasks.
    for (;;)
    {
        hw::can::CanMsg rx_msg = io::can1queue.popRxMsgFromQueue();

        JsonCanMsg jsoncan_rx_msg;
        io::jsoncan::copyFromCanMsg(&rx_msg, &jsoncan_rx_msg);
        io_canRx_updateRxTableWithMessage(&jsoncan_rx_msg);
    }
}

void tasks_run1Hz(void)
{
    // io_chimera_sleepTaskIfEnabled();

    // Setup tasks.
    static const TickType_t period_ms = 1000U;
    // WatchdogHandle         *watchdog  = hw_watchdog_allocateWatchdog();
    // hw_watchdog_initWatchdog(watchdog, RTOS_TASK_1HZ, period_ms);

    static uint32_t start_ticks = 0;
    start_ticks                 = osKernelGetTickCount();

    for (;;)
    {
        // hw_stackWaterMarkConfig_check();
        app::StateMachine::tick1Hz();

        const bool debug_mode_enabled = app_canRx_Debug_EnableDebugMode_get();
        io_canTx_enableMode(CAN_MODE_DEBUG, debug_mode_enabled);
        io_canTx_enqueue1HzMsgs();

        // Watchdog check-in must be the last function called before putting the
        // task to sleep.
        // hw_watchdog_checkIn(watchdog);

        start_ticks += period_ms;
        osDelayUntil(start_ticks);
    }
}

void tasks_run100Hz(void)
{
    // io_chimera_sleepTaskIfEnabled();

    // Setup tasks.
    static const TickType_t period_ms = 10;
    // WatchdogHandle         *watchdog  = hw_watchdog_allocateWatchdog();
    // hw_watchdog_initWatchdog(watchdog, RTOS_TASK_100HZ, period_ms);

    static uint32_t start_ticks = 0;
    start_ticks                 = osKernelGetTickCount();

    for (;;)
    {
        app::StateMachine::tick100Hz();
        io_canTx_enqueue100HzMsgs();

        // Watchdog check-in must be the last function called before putting the
        // task to sleep.
        // hw_watchdog_checkIn(watchdog);

        start_ticks += period_ms;
        osDelayUntil(start_ticks);
    }
}

void tasks_run1kHz(void)
{
    // io_chimera_sleepTaskIfEnabled();

    // Setup tasks.
    static const TickType_t period_ms = 1;
    // WatchdogHandle         *watchdog  = hw_watchdog_allocateWatchdog();
    // hw_watchdog_initWatchdog(watchdog, RTOS_TASK_1KHZ, period_ms);

    static uint32_t start_ticks = 0;
    start_ticks                 = osKernelGetTickCount();

    /* Infinite loop */
    for (;;)
    {
        // Check in for timeouts for all RTOS tasks
        // hw_watchdog_checkForTimeouts();

        const uint32_t task_start_ms = TICK_TO_MS(osKernelGetTickCount());
        io_canTx_enqueueOtherPeriodicMsgs(task_start_ms);

        // Watchdog check-in must be the last function called before putting the
        // task to sleep. Prevent check in if the elapsed period is greater or
        // equal to the period ms
        if ((TICK_TO_MS(osKernelGetTickCount()) - task_start_ms) <= period_ms)
        {
            // hw_watchdog_checkIn(watchdog);
        }

        start_ticks += period_ms;
        osDelayUntil(start_ticks);
    }
}

// void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
// {
//     if (huart == debug_uart.handle)
//     {
//         io_chimera_msgRxCallback();
//     }
// }