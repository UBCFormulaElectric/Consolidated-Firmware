#include "tasks.h"
#include "cmsis_os.h"
#include "shared.pb.h"

// app
#include "app_mainState.h"

// io
#include "io_log.h"
#include "io_jsoncan.h"
#include "io_canMsgQueues.h"
#include "io_chimeraConfig.h"

// hw
#include "hw_utils.h"
#include "hw_bootup.h"
#include "hw_hardFaultHandler.h"
#include "hw_cans.h"
#include "hw_adcs.h"
#include "hw_uarts.h"
#include "hw_gpios.h"
#include "hw_watchdogs.h"

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
    __HAL_DBGMCU_FREEZE_IWDG();
    hw_hardFaultHandler_init();

    hw::adc::adc1.init();

    io::chimera::init(
        &hw::uart::chimera_uart, GpioNetName_crit_net_name_tag, AdcNetName_crit_net_name_tag, &hw::gpio::n_chimera_pin);

    // can
    hw::can::can1.init();
    io_canTx_init(
        [](const JsonCanMsg *msg)
        {
            hw::can::CanMsg tx_msg{};
            io::jsoncan::copyToCanMsg(msg, &tx_msg);
            io::can1queue.pushTxMsgToQueue(&tx_msg);
        }); // TODO this needs to be more sophisticated for multiple busses
    io_canTx_enableMode(CAN_MODE_DEFAULT, true);
    io::can1queue.init();
    app_canTx_init();
    app_canRx_init();
    // broadcast commit info
    app_canTx_CRIT_Hash_set(GIT_COMMIT_HASH);
    app_canTx_CRIT_Clean_set(GIT_COMMIT_CLEAN);

    app::StateMachine::init(&app::critstates::main_state);
}

void tasks_runCanTx(void)
{
    io::chimera::sleepTaskIfEnabled();

    // Setup tasks.
    for (;;)
    {
        hw::can::CanMsg tx_msg = io::can1queue.popTxMsgFromQueue();
        if (const bool transmit_status = hw::can::can1.transmit(&tx_msg); !transmit_status)
        {
         // idk do something
        }
    }
}

void tasks_runCanRx()
{
    io::chimera::sleepTaskIfEnabled();

    // Setup tasks.
    for (;;)
    {
        hw::can::CanMsg rx_msg = io::can1queue.popRxMsgFromQueue();

        JsonCanMsg jsoncan_rx_msg;
        io::jsoncan::copyFromCanMsg(&rx_msg, &jsoncan_rx_msg);
        io_canRx_updateRxTableWithMessage(&jsoncan_rx_msg);
    }
}

void tasks_run1Hz()
{
    io::chimera::sleepTaskIfEnabled();

    // Setup tasks.
    static const TickType_t period_ms = 1000U;

    hw::watchdog::WatchdogInstance run1HzWatchdog{ RTOS_TASK_1HZ, period_ms };
    hw::watchdog::monitor::registerWatchdogInstance(&run1HzWatchdog);

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
        run1HzWatchdog.checkIn();

        start_ticks += period_ms;
        osDelayUntil(start_ticks);
    }
}

void tasks_run100Hz()
{
    io::chimera::sleepTaskIfEnabled();

    // Setup tasks.
    static const TickType_t        period_ms = 10;
    hw::watchdog::WatchdogInstance run100HzWatchdog{ RTOS_TASK_100HZ, period_ms };
    hw::watchdog::monitor::registerWatchdogInstance(&run100HzWatchdog);

    static uint32_t start_ticks = 0;
    start_ticks                 = osKernelGetTickCount();

    for (;;)
    {
        app::StateMachine::tick100Hz();
        io_canTx_enqueue100HzMsgs();

        // Watchdog check-in must be the last function called before putting the
        // task to sleep.
        run100HzWatchdog.checkIn();

        start_ticks += period_ms;
        osDelayUntil(start_ticks);
    }
}

void tasks_run1kHz()
{
    io::chimera::sleepTaskIfEnabled();

    // Setup tasks.
    static const TickType_t period_ms = 1;

    hw::watchdog::WatchdogInstance run1kHzWatchdog{ RTOS_TASK_1KHZ, period_ms };
    hw::watchdog::monitor::registerWatchdogInstance(&run1kHzWatchdog);

    static uint32_t start_ticks = 0;
    start_ticks                 = osKernelGetTickCount();

    /* Infinite loop */
    for (;;)
    {
        // Check in for timeouts for all RTOS tasks
        hw::watchdog::monitor::checkForTimeouts();

        const uint32_t task_start_ms = TICK_TO_MS(osKernelGetTickCount());
        io_canTx_enqueueOtherPeriodicMsgs(task_start_ms);

        // Watchdog check-in must be the last function called before putting the
        // task to sleep. Prevent check in if the elapsed period is greater or
        // equal to the period ms
        if ((TICK_TO_MS(osKernelGetTickCount()) - task_start_ms) <= period_ms)
        {
            run1kHzWatchdog.checkIn();
        }

        start_ticks += period_ms;
        osDelayUntil(start_ticks);
    }
}
