#include "tasks.h"
#include "main.h"
#include "cmsis_os.h"

#include "app_canTx.h"
#include "app_canRx.h"
#include "app_canAlerts.h"
#include "app_commitInfo.h"
#include "app_utils.h"
#include "app_units.h"
#include "app_stateMachine.h"
#include "app_heartbeatMonitor.h"
#include "app_timer.h"
#include "app_mainState.h"
#include "app_apps.h"
#include "app_brake.h"
#include "app_coolant.h"
#include "app_steering.h"
#include "app_wheels.h"

#include "io_canTx.h"
#include "io_canRx.h"
#include "io_can.h"
#include "io_jsoncan.h"
#include "io_coolant.h"
#include "io_wheels.h"
#include "io_steering.h"
#include "io_apps.h"
#include "io_brake.h"
#include "io_time.h"
#include "io_log.h"

#include "hw_hardFaultHandler.h"
#include "hw_bootup.h"
#include "hw_can.h"
#include "hw_adc.h"
#include "hw_utils.h"
#include "hw_stackWaterMark.h"
#include "hw_stackWaterMarkConfig.h"
#include "hw_watchdog.h"
#include "hw_watchdogConfig.h"

static void canRxQueueOverflowCallBack(uint32_t overflow_count)
{
    app_canTx_FSM_RxOverflowCount_set(overflow_count);
    app_canAlerts_FSM_Warning_RxOverflow_set(true);
}

static void canTxQueueOverflowCallBack(uint32_t overflow_count)
{
    app_canTx_FSM_TxOverflowCount_set(overflow_count);
    app_canAlerts_FSM_Warning_TxOverflow_set(true);
}

extern ADC_HandleTypeDef  hadc1;
extern CAN_HandleTypeDef  hcan1;
extern IWDG_HandleTypeDef hiwdg;
extern TIM_HandleTypeDef  htim3;
extern TIM_HandleTypeDef  htim8;
extern TIM_HandleTypeDef  htim12;

static const CanConfig can_config = {
    .rx_msg_filter        = io_canRx_filterMessageId,
    .tx_overflow_callback = canTxQueueOverflowCallBack,
    .rx_overflow_callback = canRxQueueOverflowCallBack,
};
static CanHandle can = {
    .can                      = &hcan1,
    .can_msg_recieve_callback = NULL,
};
// config to forward can functions to shared heartbeat
// FSM rellies on BMS
bool heartbeatMonitorChecklist[HEARTBEAT_BOARD_COUNT] = { [BMS_HEARTBEAT_BOARD] = true,
                                                          [DCM_HEARTBEAT_BOARD] = false,
                                                          [PDM_HEARTBEAT_BOARD] = false,
                                                          [FSM_HEARTBEAT_BOARD] = false,
                                                          [DIM_HEARTBEAT_BOARD] = false };

// heartbeatGetters - get heartbeat signals from other boards
bool (*heartbeatGetters[HEARTBEAT_BOARD_COUNT])() = { [BMS_HEARTBEAT_BOARD] = &app_canRx_BMS_Heartbeat_get,
                                                      [DCM_HEARTBEAT_BOARD] = NULL,
                                                      [PDM_HEARTBEAT_BOARD] = NULL,
                                                      [FSM_HEARTBEAT_BOARD] = NULL,
                                                      [DIM_HEARTBEAT_BOARD] = NULL };

// heartbeatUpdaters - update local CAN table with heartbeat status
void (*heartbeatUpdaters[HEARTBEAT_BOARD_COUNT])(bool) = { [BMS_HEARTBEAT_BOARD] = &app_canRx_BMS_Heartbeat_update,
                                                           [DCM_HEARTBEAT_BOARD] = NULL,
                                                           [PDM_HEARTBEAT_BOARD] = NULL,
                                                           [FSM_HEARTBEAT_BOARD] = NULL,
                                                           [DIM_HEARTBEAT_BOARD] = NULL };

// heartbeatFaultSetters - broadcast heartbeat faults over CAN
void (*heartbeatFaultSetters[HEARTBEAT_BOARD_COUNT])(bool) = {
    [BMS_HEARTBEAT_BOARD] = &app_canAlerts_FSM_Fault_MissingBMSHeartbeat_set,
    [DCM_HEARTBEAT_BOARD] = NULL,
    [PDM_HEARTBEAT_BOARD] = NULL,
    [FSM_HEARTBEAT_BOARD] = NULL,
    [DIM_HEARTBEAT_BOARD] = NULL
};

// heartbeatFaultGetters - gets fault statuses over CAN
bool (*heartbeatFaultGetters[HEARTBEAT_BOARD_COUNT])() = {
    [BMS_HEARTBEAT_BOARD] = &app_canAlerts_FSM_Fault_MissingBMSHeartbeat_get,
    [DCM_HEARTBEAT_BOARD] = NULL,
    [PDM_HEARTBEAT_BOARD] = NULL,
    [FSM_HEARTBEAT_BOARD] = NULL,
    [DIM_HEARTBEAT_BOARD] = NULL
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
    LOG_INFO("FSM reset!");

    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)hw_adc_getRawValuesBuffer(), hadc1.Init.NbrOfConversion);
    HAL_TIM_Base_Start(&htim3);

    hw_hardFaultHandler_init();
    hw_can_init(&can);
    hw_watchdog_init(hw_watchdogConfig_refresh, hw_watchdogConfig_timeoutCallback);

    io_canTx_init(io_jsoncan_pushTxMsgToQueue);
    io_canTx_enableMode(CAN_MODE_DEFAULT, true);
    io_can_init(&can_config);

    io_apps_init();
    io_coolant_init(&htim8);
    io_wheels_init(&htim12, &htim12);

    app_canTx_init();
    app_canRx_init();

    app_apps_init();
    app_coolant_init();
    app_stateMachine_init(app_mainState_get());

    app_heartbeatMonitor_init(
        heartbeatMonitorChecklist, heartbeatGetters, heartbeatUpdaters, &app_canTx_FSM_Heartbeat_set,
        heartbeatFaultSetters, heartbeatFaultGetters);

    // broadcast commit info
    app_canTx_FSM_Hash_set(GIT_COMMIT_HASH);
    app_canTx_FSM_Clean_set(GIT_COMMIT_CLEAN);
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
