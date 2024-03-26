#include "tasks.h"
#include "main.h"
#include "cmsis_os.h"

#include "app_globals.h"
#include "app_canTx.h"
#include "app_canRx.h"
#include "app_canAlerts.h"
#include "app_utils.h"
#include "app_units.h"
#include "app_stateMachine.h"
#include "app_heartbeatMonitor.h"
#include "states/app_initState.h"
#include "app_commitInfo.h"

#include "io_canTx.h"
#include "io_canRx.h"
#include "io_led.h"
#include "io_buzzer.h"
#include "io_sbgEllipse.h"
#include "io_can.h"
#include "io_jsoncan.h"
#include "io_time.h"
#include "io_log.h"

#include "hw_hardFaultHandler.h"
#include "hw_bootup.h"
#include "hw_can.h"
#include "hw_gpio.h"
#include "hw_utils.h"
#include "hw_stackWaterMarkConfig.h"
#include "hw_watchdog.h"
#include "hw_watchdogConfig.h"

#define HEARTBEAT_MONITOR_TIMEOUT_PERIOD_MS 300U

void canTxQueueOverflowCallBack(uint32_t overflow_count)
{
    app_canTx_DCM_TxOverflowCount_set(overflow_count);
    app_canAlerts_DCM_Warning_TxOverflow_set(true);
    BREAK_IF_DEBUGGER_CONNECTED();
}

void canRxQueueOverflowCallBack(uint32_t overflow_count)
{
    app_canTx_DCM_RxOverflowCount_set(overflow_count);
    app_canAlerts_DCM_Warning_RxOverflow_set(true);
    BREAK_IF_DEBUGGER_CONNECTED();
}

extern CAN_HandleTypeDef  hcan1;
extern UART_HandleTypeDef huart1;

static const CanConfig can_config = {
    .rx_msg_filter        = io_canRx_filterMessageId,
    .tx_overflow_callback = canTxQueueOverflowCallBack,
    .rx_overflow_callback = canRxQueueOverflowCallBack,
};
static CanHandle can = {
    .can                       = &hcan1,
    .can_msg_received_callback = io_can_msgReceivedCallback,
};

UART imu_uart = { .handle = &huart1 };

static const BinaryLed brake_light = { .gpio = {
                                           .port = BRAKE_LIGHT_EN_3V3_GPIO_Port,
                                           .pin  = BRAKE_LIGHT_EN_3V3_Pin,
                                       } };
static const Buzzer    buzzer      = { .gpio = {
                                           .port = BUZZER_EN_3V3_GPIO_Port,
                                           .pin  = BUZZER_EN_3V3_Pin,
                               } };

// config to forward can functions to shared heartbeat
// BMS rellies on DIM, FSM, and BMS
bool heartbeatMonitorChecklist[HEARTBEAT_BOARD_COUNT] = { [BMS_HEARTBEAT_BOARD] = true,
                                                          [DCM_HEARTBEAT_BOARD] = false,
                                                          [PDM_HEARTBEAT_BOARD] = false,
                                                          [FSM_HEARTBEAT_BOARD] = true,
                                                          [DIM_HEARTBEAT_BOARD] = true };

// heartbeatGetters - get heartbeat signals from other boards
bool (*heartbeatGetters[HEARTBEAT_BOARD_COUNT])() = { [BMS_HEARTBEAT_BOARD] = &app_canRx_BMS_Heartbeat_get,
                                                      [DCM_HEARTBEAT_BOARD] = NULL,
                                                      [PDM_HEARTBEAT_BOARD] = NULL,
                                                      [FSM_HEARTBEAT_BOARD] = &app_canRx_FSM_Heartbeat_get,
                                                      [DIM_HEARTBEAT_BOARD] = &app_canRx_DIM_Heartbeat_get };

// heartbeatUpdaters - update local CAN table with heartbeat status
void (*heartbeatUpdaters[HEARTBEAT_BOARD_COUNT])(bool) = { [BMS_HEARTBEAT_BOARD] = &app_canRx_BMS_Heartbeat_update,
                                                           [DCM_HEARTBEAT_BOARD] = NULL,
                                                           [PDM_HEARTBEAT_BOARD] = NULL,
                                                           [FSM_HEARTBEAT_BOARD] = &app_canRx_FSM_Heartbeat_update,
                                                           [DIM_HEARTBEAT_BOARD] = &app_canRx_DIM_Heartbeat_update };

// heartbeatFaultSetters - broadcast heartbeat faults over CAN
void (*heartbeatFaultSetters[HEARTBEAT_BOARD_COUNT])(bool) = {
    [BMS_HEARTBEAT_BOARD] = &app_canAlerts_DCM_Fault_MissingBMSHeartbeat_set,
    [DCM_HEARTBEAT_BOARD] = NULL,
    [PDM_HEARTBEAT_BOARD] = NULL,
    [FSM_HEARTBEAT_BOARD] = &app_canAlerts_DCM_Fault_MissingFSMHeartbeat_set,
    [DIM_HEARTBEAT_BOARD] = &app_canAlerts_DCM_Fault_MissingDIMHeartbeat_set
};

// heartbeatFaultGetters - gets fault statuses over CAN
bool (*heartbeatFaultGetters[HEARTBEAT_BOARD_COUNT])() = {
    [BMS_HEARTBEAT_BOARD] = &app_canAlerts_DCM_Fault_MissingBMSHeartbeat_get,
    [DCM_HEARTBEAT_BOARD] = NULL,
    [PDM_HEARTBEAT_BOARD] = NULL,
    [FSM_HEARTBEAT_BOARD] = &app_canAlerts_DCM_Fault_MissingFSMHeartbeat_get,
    [DIM_HEARTBEAT_BOARD] = &app_canAlerts_DCM_Fault_MissingDIMHeartbeat_get
};

static const GlobalsConfig globals_config = {
    .brake_light = &brake_light,
    .buzzer      = &buzzer,
};

void tasks_preInit(void)
{
    // After booting, re-enable interrupts and ensure the core is using the application's vector table.
    hw_bootup_enableInterruptsForApp();
}

void tasks_init(void)
{
    __HAL_DBGMCU_FREEZE_IWDG();

    // Configure and initialize SEGGER SystemView.
    SEGGER_SYSVIEW_Conf();
    LOG_INFO("DCM reset!");

    hw_hardFaultHandler_init();
    hw_can_init(&can);
    hw_watchdog_init(hw_watchdogConfig_refresh, hw_watchdogConfig_timeoutCallback);

    io_canTx_init(io_jsoncan_pushTxMsgToQueue);
    io_canTx_enableMode(CAN_MODE_DEFAULT, true);
    io_can_init(&can_config);

    if (!io_sbgEllipse_init(&imu_uart))
    {
        Error_Handler();
    }

    app_canTx_init();
    app_canRx_init();

    app_globals_init(&globals_config);
    app_heartbeatMonitor_init(
        heartbeatMonitorChecklist, heartbeatGetters, heartbeatUpdaters, &app_canTx_DCM_Heartbeat_set,
        heartbeatFaultSetters, heartbeatFaultGetters);
    app_stateMachine_init(app_initState_get());

    // broadcast commit info
    app_canTx_DCM_Hash_set(GIT_COMMIT_HASH);
    app_canTx_DCM_Clean_set(GIT_COMMIT_CLEAN);
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
        app_stateMachine_tick1Hz();
        hw_stackWaterMarkConfig_check();

        const bool debug_mode_enabled = app_canRx_Debug_EnableDebugMode_get();
        io_canTx_enableMode(CAN_MODE_DEBUG, debug_mode_enabled);
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
