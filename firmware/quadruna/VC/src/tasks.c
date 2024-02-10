#include "tasks.h"
#include "main.h"
#include "cmsis_os.h"
#include "string.h"

#include "app_globals.h"
#include "app_heartbeatMonitor.h"
#include "states/app_initState.h"
#include "app_canTx.h"
#include "app_canRx.h"
#include "app_canAlerts.h"
#include "app_commitInfo.h"

#include "io_jsoncan.h"
#include "io_log.h"
#include "io_canRx.h"

#include "hw_bootup.h"
#include "hw_utils.h"
#include "hw_hardFaultHandler.h"
#include "hw_watchdog.h"
#include "hw_watchdogConfig.h"
#include "hw_gpio.h"
#include "hw_stackWaterMark.h"
#include "hw_stackWaterMarkConfig.h"
#include "hw_uart.h"

extern ADC_HandleTypeDef   hadc1;
extern ADC_HandleTypeDef   hadc3;
extern FDCAN_HandleTypeDef hfdcan1;
extern UART_HandleTypeDef  huart7;
// extern IWDG_HandleTypeDef  hiwdg1;

void canRxQueueOverflowCallBack(uint32_t overflow_count)
{
    app_canTx_VC_RxOverflowCount_set(overflow_count);
    app_canAlerts_VC_Warning_RxOverflow_set(true);
}

void canTxQueueOverflowCallBack(uint32_t overflow_count)
{
    app_canTx_VC_TxOverflowCount_set(overflow_count);
    app_canAlerts_VC_Warning_TxOverflow_set(true);
}

static const CanConfig can_config = {
    .rx_msg_filter        = io_canRx_filterMessageId,
    .tx_overflow_callback = canTxQueueOverflowCallBack,
    .rx_overflow_callback = canRxQueueOverflowCallBack,
};

#define DEBUG_BUF_SIZE 12
static UART    debug_uart = { .handle = &huart7 };
static uint8_t data[DEBUG_BUF_SIZE];

void tasks_preInit(void) {}

void tasks_init(void)
{
    __HAL_DBGMCU_FREEZE_IWDG1();

    // Configure and initialize SEGGER SystemView.
    SEGGER_SYSVIEW_Conf();
    LOG_INFO("VC reset!");

    // efuses:
    // HAL_ADC_Start_DMA(
    //     hw_tasks_config->hadc1, (uint32_t *)hw_adc_getRawValuesBuffer(),
    //     hw_tasks_config->hadc1->Init.NbrOfConversion);

    hw_hardFaultHandler_init();
    hw_can_init(&hfdcan1);

    // TODO: Re-enable watchdog (disabled because it can get annoying when bringing up a board).
    hw_watchdog_init(hw_watchdogConfig_refresh, hw_watchdogConfig_timeoutCallback);

    io_canTx_init(io_jsoncan_pushTxMsgToQueue);
    io_canTx_enableMode(CAN_MODE_DEFAULT, true);
    io_can_init(&can_config);

    app_canTx_init();
    app_canRx_init();
    app_stateMachine_init(app_initState_get());

    app_canTx_VC_Hash_set(GIT_COMMIT_HASH);
    app_canTx_VC_Clean_set(GIT_COMMIT_CLEAN);

    // Receive data over the debug UART line in interrupt mode.
    hw_uart_receiveIt(&debug_uart, data, sizeof(data));
}

void tasks_run1Hz(void)
{
    static const TickType_t period_ms = 1000U;
    WatchdogHandle         *watchdog  = hw_watchdog_allocateWatchdog();
    hw_watchdog_initWatchdog(watchdog, RTOS_TASK_1HZ, period_ms);

    static uint32_t start_ticks = 0;
    start_ticks                 = osKernelGetTickCount();

    for (;;)
    {
        hw_stackWaterMarkConfig_check();
        app_stateMachine_tick1Hz();

        // TODO: setup can debug
        // const bool debug_mode_enabled = app_canRx_Debug_EnableDebugMode_get();
        // io_canTx_enableMode(CAN_MODE_DEBUG, debug_mode_enabled);
        io_canTx_enqueue1HzMsgs();

        // Watchdog check-in must be the last function called before putting the
        // task to sleep.
        hw_watchdog_checkIn(watchdog);

        start_ticks += period_ms;
        osDelayUntil(start_ticks);
    }
}

void tasks_run100Hz(void)
{
    static const TickType_t period_ms = 10;
    WatchdogHandle         *watchdog  = hw_watchdog_allocateWatchdog();
    hw_watchdog_initWatchdog(watchdog, RTOS_TASK_100HZ, period_ms);

    static uint32_t start_ticks = 0;
    start_ticks                 = osKernelGetTickCount();

    for (;;)
    {
        const uint32_t start_time_ms = osKernelGetTickCount();

        app_stateMachine_tick100Hz();
        io_canTx_enqueue100HzMsgs();

        // Watchdog check-in must be the last function called before putting the
        // task to sleep.
        hw_watchdog_checkIn(watchdog);

        start_ticks += period_ms;
        osDelayUntil(start_ticks);
    }
}

void tasks_run1kHz(void)
{
    static const TickType_t period_ms = 1U;
    WatchdogHandle         *watchdog  = hw_watchdog_allocateWatchdog();
    hw_watchdog_initWatchdog(watchdog, RTOS_TASK_1KHZ, period_ms);

    static uint32_t start_ticks = 0;
    start_ticks                 = osKernelGetTickCount();

    for (;;)
    {
        const uint32_t start_time_ms = osKernelGetTickCount();

        hw_watchdog_checkForTimeouts();

        const uint32_t task_start_ms = TICK_TO_MS(osKernelGetTickCount());
        io_canTx_enqueueOtherPeriodicMsgs(task_start_ms);

        // Watchdog check-in must be the last function called before putting the
        // task to sleep. Prevent check in if the elapsed period is greater or
        // equal to the period ms
        if ((TICK_TO_MS(osKernelGetTickCount()) - task_start_ms) <= period_ms)
        {
            hw_watchdog_checkIn(watchdog);
        }

        start_ticks += period_ms;
        osDelayUntil(start_ticks);
    }
}

void tasks_runCanTx(void)
{
    for (;;)
    {
        io_can_transmitMsgFromQueue();
    }
}

void tasks_runCanRx(void)
{
    for (;;)
    {
        CanMsg rx_msg;
        io_can_popRxMsgFromQueue(&rx_msg);

        JsonCanMsg jsoncan_rx_msg;
        io_jsoncan_copyFromCanMsg(&rx_msg, &jsoncan_rx_msg);
        io_canRx_updateRxTableWithMessage(&jsoncan_rx_msg);
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart == debug_uart.handle)
    {
        // This interrupt is fired when DEBUG_BUF_SIZE bytes are received via UART.
        // NOTE: If you send more or less data in a UART transaction, seems like the 
        // peripheral can get confused...

        // Just re-transmit whatever data was received, as a demo.
        hw_uart_transmitPoll(&debug_uart, data, sizeof(data), osWaitForever);

        // Start receiving data in interrupt mode again so this interrupt will get fired if 
        // more data is recieved.
        hw_uart_receiveIt(&debug_uart, data, sizeof(data));
    }
}