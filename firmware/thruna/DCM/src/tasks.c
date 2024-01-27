#include "tasks.h"
#include "main.h"
#include "cmsis_os.h"

#include "app_globals.h"
#include "App_CanTx.h"
#include "App_CanRx.h"
#include "App_CanAlerts.h"
#include "app_utils.h"
#include "app_units.h"
#include "app_stateMachine.h"
#include "app_heartbeatMonitor.h"
#include "states/app_initState.h"
#include "App_CommitInfo.h"

#include "Io_CanTx.h"
#include "Io_CanRx.h"
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
    App_CanTx_DCM_TxOverflowCount_Set(overflow_count);
    App_CanAlerts_DCM_Warning_TxOverflow_Set(true);
    BREAK_IF_DEBUGGER_CONNECTED();
}

void canRxQueueOverflowCallBack(uint32_t overflow_count)
{
    App_CanTx_DCM_RxOverflowCount_Set(overflow_count);
    App_CanAlerts_DCM_Warning_RxOverflow_Set(true);
    BREAK_IF_DEBUGGER_CONNECTED();
}

extern CAN_HandleTypeDef  hcan1;
extern UART_HandleTypeDef huart1;

static const CanConfig can_config = {
    .rx_msg_filter        = Io_CanRx_FilterMessageId,
    .tx_overflow_callback = canTxQueueOverflowCallBack,
    .rx_overflow_callback = canRxQueueOverflowCallBack,
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
bool (*heartbeatGetters[HEARTBEAT_BOARD_COUNT])() = { [BMS_HEARTBEAT_BOARD] = &App_CanRx_BMS_Heartbeat_Get,
                                                      [DCM_HEARTBEAT_BOARD] = NULL,
                                                      [PDM_HEARTBEAT_BOARD] = NULL,
                                                      [FSM_HEARTBEAT_BOARD] = &App_CanRx_FSM_Heartbeat_Get,
                                                      [DIM_HEARTBEAT_BOARD] = &App_CanRx_DIM_Heartbeat_Get };

// heartbeatUpdaters - update local CAN table with heartbeat status
void (*heartbeatUpdaters[HEARTBEAT_BOARD_COUNT])(bool) = { [BMS_HEARTBEAT_BOARD] = &App_CanRx_BMS_Heartbeat_Update,
                                                           [DCM_HEARTBEAT_BOARD] = NULL,
                                                           [PDM_HEARTBEAT_BOARD] = NULL,
                                                           [FSM_HEARTBEAT_BOARD] = &App_CanRx_FSM_Heartbeat_Update,
                                                           [DIM_HEARTBEAT_BOARD] = &App_CanRx_DIM_Heartbeat_Update };

// heartbeatFaultSetters - broadcast heartbeat faults over CAN
void (*heartbeatFaultSetters[HEARTBEAT_BOARD_COUNT])(bool) = {
    [BMS_HEARTBEAT_BOARD] = &App_CanAlerts_DCM_Fault_MissingBMSHeartbeat_Set,
    [DCM_HEARTBEAT_BOARD] = NULL,
    [PDM_HEARTBEAT_BOARD] = NULL,
    [FSM_HEARTBEAT_BOARD] = &App_CanAlerts_DCM_Fault_MissingFSMHeartbeat_Set,
    [DIM_HEARTBEAT_BOARD] = &App_CanAlerts_DCM_Fault_MissingDIMHeartbeat_Set
};

// heartbeatFaultGetters - gets fault statuses over CAN
bool (*heartbeatFaultGetters[HEARTBEAT_BOARD_COUNT])() = {
    [BMS_HEARTBEAT_BOARD] = &App_CanAlerts_DCM_Fault_MissingBMSHeartbeat_Get,
    [DCM_HEARTBEAT_BOARD] = NULL,
    [PDM_HEARTBEAT_BOARD] = NULL,
    [FSM_HEARTBEAT_BOARD] = &App_CanAlerts_DCM_Fault_MissingFSMHeartbeat_Get,
    [DIM_HEARTBEAT_BOARD] = &App_CanAlerts_DCM_Fault_MissingDIMHeartbeat_Get
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
    hw_can_init(&hcan1);
    hw_watchdog_init(hw_watchdogConfig_refresh, hw_watchdogConfig_timeoutCallback);

    Io_CanTx_Init(io_jsoncan_pushTxMsgToQueue);
    Io_CanTx_EnableMode(CAN_MODE_DEFAULT, true);
    io_can_init(&can_config);

    if (!io_sbgEllipse_init(&imu_uart))
    {
        Error_Handler();
    }

    App_CanTx_Init();
    App_CanRx_Init();

    app_heartbeatMonitor_init(
        HEARTBEAT_MONITOR_TIMEOUT_PERIOD_MS, heartbeatMonitorChecklist, heartbeatGetters, heartbeatUpdaters,
        &App_CanTx_DCM_Heartbeat_Set, heartbeatFaultSetters, heartbeatFaultGetters);
    app_stateMachine_init(app_initState_get());
    app_globals_init(&globals_config);

    // broadcast commit info
    App_CanTx_DCM_Hash_Set(GIT_COMMIT_HASH);
    App_CanTx_DCM_Clean_Set(GIT_COMMIT_CLEAN);
}

void tasks_run1Hz(void)
{
    static const TickType_t period_ms = 1000U;
    WatchdogHandle *        watchdog  = hw_watchdog_allocateWatchdog();
    hw_watchdog_initWatchdog(watchdog, RTOS_TASK_1HZ, period_ms);

    static uint32_t start_ticks = 0;
    start_ticks                 = osKernelGetTickCount();

    for (;;)
    {
        io_sbgEllipse_getComStatus();
        app_stateMachine_tick1Hz();

        const bool debug_mode_enabled = App_CanRx_Debug_EnableDebugMode_Get();
        Io_CanTx_EnableMode(CAN_MODE_DEBUG, debug_mode_enabled);
        Io_CanTx_Enqueue1HzMsgs();

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
    WatchdogHandle *        watchdog  = hw_watchdog_allocateWatchdog();
    hw_watchdog_initWatchdog(watchdog, RTOS_TASK_100HZ, period_ms);

    static uint32_t start_ticks = 0;
    start_ticks                 = osKernelGetTickCount();

    for (;;)
    {
        app_stateMachine_tick100Hz();
        Io_CanTx_Enqueue100HzMsgs();

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
    WatchdogHandle *        watchdog  = hw_watchdog_allocateWatchdog();
    hw_watchdog_initWatchdog(watchdog, RTOS_TASK_1KHZ, period_ms);

    static uint32_t start_ticks = 0;
    start_ticks                 = osKernelGetTickCount();

    for (;;)
    {
        hw_stackWaterMarkConfig_check();
        hw_watchdog_checkForTimeouts();
        const uint32_t task_start_ms = TICK_TO_MS(osKernelGetTickCount());

        Io_CanTx_EnqueueOtherPeriodicMsgs(task_start_ms);

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
        Io_CanRx_UpdateRxTableWithMessage(&jsoncan_rx_msg);
    }
}
