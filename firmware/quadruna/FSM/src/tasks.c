#include "tasks.h"
#include "main.h"
#include "cmsis_os.h"

#include "app_heartbeatMonitor.h"
#include "app_mainState.h"

#include "app_canTx.h"
#include "app_canRx.h"
#include "app_canAlerts.h"
#include "app_commitInfo.h"
#include "app_steering.h"
#include "app_brake.h"
#include "app_suspension.h"
#include "app_loadCell.h"

#include "io_jsoncan.h"
#include "io_canRx.h"
#include "io_log.h"
#include "io_can.h"
#include "io_led.h"
#include "io_chimera.h"
#include "io_steering.h"
#include "io_wheels.h"
#include "io_brake.h"
#include "io_suspension.h"
#include "io_loadCell.h"

#include "hw_bootup.h"
#include "hw_utils.h"
#include "hw_hardFaultHandler.h"
#include "hw_watchdog.h"
#include "hw_watchdogConfig.h"
#include "hw_stackWaterMark.h"
#include "hw_stackWaterMarkConfig.h"
#include "hw_adc.h"
#include "hw_gpio.h"
#include "hw_uart.h"

#include "shared.pb.h"
#include "FSM.pb.h"

extern ADC_HandleTypeDef hadc1;
extern TIM_HandleTypeDef htim3;
extern CAN_HandleTypeDef hcan1;
extern TIM_HandleTypeDef htim12;

const CanHandle           can = { .can = &hcan1, .can_msg_received_callback = io_can_msgReceivedCallback };
extern UART_HandleTypeDef huart1;
// extern IWDG_HandleTypeDef *hiwdg; TODO: Re-enable watchdog

void canRxQueueOverflowCallBack(uint32_t overflow_count)
{
    app_canTx_FSM_RxOverflowCount_set(overflow_count);
    app_canAlerts_FSM_Warning_TxOverflow_set(true);
}

void canTxQueueOverflowCallBack(uint32_t overflow_count)
{
    app_canTx_FSM_TxOverflowCount_set(overflow_count);
    app_canAlerts_FSM_Warning_TxOverflow_set(true);
}

const CanConfig can_config = {
    .rx_msg_filter        = io_canRx_filterMessageId,
    .tx_overflow_callback = canTxQueueOverflowCallBack,
    .rx_overflow_callback = canRxQueueOverflowCallBack,
};

static const Gpio      brake_ocsc_ok_3v3       = { .port = BRAKE_OCSC_OK_3V3_GPIO_Port, .pin = BRAKE_OCSC_OK_3V3_Pin };
static const Gpio      n_chimera_pin           = { .port = NCHIMERA_GPIO_Port, .pin = NCHIMERA_Pin };
static const BinaryLed led                     = { .gpio = { .port = LED_GPIO_Port, .pin = LED_Pin } };
static const Gpio      nbspd_brake_pressed_3v3 = { .port = NBSPD_BRAKE_PRESSED_3V3_GPIO_Port,
                                                   .pin  = NBSPD_BRAKE_PRESSED_3V3_Pin };
static const Gpio      nprogram_3v3            = { .port = NPROGRAM_3V3_GPIO_Port, .pin = NPROGRAM_3V3_Pin };
static const Gpio      fsm_shdn                = { .port = FSM_SHDN_GPIO_Port, .pin = FSM_SHDN_Pin };

const Gpio *id_to_gpio[] = { [FSM_GpioNetName_BRAKE_OCSC_OK_3V3]       = &brake_ocsc_ok_3v3,
                             [FSM_GpioNetName_NCHIMERA]                = &n_chimera_pin,
                             [FSM_GpioNetName_LED]                     = &led.gpio,
                             [FSM_GpioNetName_NBSPD_BRAKE_PRESSED_3V3] = &nbspd_brake_pressed_3v3,
                             [FSM_GpioNetName_NPROGRAM_3V3]            = &nprogram_3v3,
                             [FSM_GpioNetName_FSM_SHDN]                = &fsm_shdn };

AdcChannel id_to_adc[] = {
    [FSM_AdcNetName_SUSP_TRAVEL_FR_3V3] = ADC1_IN9_SUSP_TRAVEL_FR,
    [FSM_AdcNetName_SUSP_TRAVEL_FL_3V3] = ADC1_IN8_SUSP_TRAVEL_FL,
    [FSM_AdcNetName_LOAD_CELL_2_3V3]    = ADC1_IN1_LOAD_CELL_2,
    [FSM_AdcNetName_APPS2_3V3]          = ADC1_IN5_APPS2,
    [FSM_AdcNetName_BPS_F_3V3]          = ADC1_IN7_BPS_F,
    [FSM_AdcNetName_BPS_B_3V3]          = ADC1_IN15_BPS_B,
    [FSM_AdcNetName_LOAD_CELL_1_3V3]    = ADC1_IN13_LOAD_CELL_1,
    [FSM_AdcNetName_APPS1_3V3]          = ADC1_IN12_APPS1,
    [FSM_AdcNetName_SteeringAngle_3V3]  = ADC1_IN11_STEERING_ANGLE,
};

static UART debug_uart = { .handle = &huart1 };

// config for heartbeat monitor (can funcs and flags)
// FSM rellies on BMS
bool heartbeatMonitorChecklist[HEARTBEAT_BOARD_COUNT] = {
    [BMS_HEARTBEAT_BOARD] = true,  [VC_HEARTBEAT_BOARD] = false,  [RSM_HEARTBEAT_BOARD] = false,
    [FSM_HEARTBEAT_BOARD] = false, [DIM_HEARTBEAT_BOARD] = false, [CRIT_HEARTBEAT_BOARD] = false
};

// heartbeatGetters - get heartbeat signals from other boards
bool (*heartbeatGetters[HEARTBEAT_BOARD_COUNT])() = { [BMS_HEARTBEAT_BOARD]  = app_canRx_BMS_Heartbeat_get,
                                                      [VC_HEARTBEAT_BOARD]   = NULL,
                                                      [RSM_HEARTBEAT_BOARD]  = NULL,
                                                      [FSM_HEARTBEAT_BOARD]  = NULL,
                                                      [DIM_HEARTBEAT_BOARD]  = NULL,
                                                      [CRIT_HEARTBEAT_BOARD] = NULL };

// heartbeatUpdaters - update local CAN table with heartbeat status
void (*heartbeatUpdaters[HEARTBEAT_BOARD_COUNT])(bool) = { [BMS_HEARTBEAT_BOARD]  = app_canRx_BMS_Heartbeat_update,
                                                           [VC_HEARTBEAT_BOARD]   = NULL,
                                                           [RSM_HEARTBEAT_BOARD]  = NULL,
                                                           [FSM_HEARTBEAT_BOARD]  = NULL,
                                                           [DIM_HEARTBEAT_BOARD]  = NULL,
                                                           [CRIT_HEARTBEAT_BOARD] = NULL };

// heartbeatFaultSetters - broadcast heartbeat faults over CAN
void (*heartbeatFaultSetters[HEARTBEAT_BOARD_COUNT])(bool) = {
    [BMS_HEARTBEAT_BOARD]  = app_canAlerts_FSM_Fault_MissingBMSHeartbeat_set,
    [VC_HEARTBEAT_BOARD]   = NULL,
    [RSM_HEARTBEAT_BOARD]  = NULL,
    [FSM_HEARTBEAT_BOARD]  = NULL,
    [DIM_HEARTBEAT_BOARD]  = NULL,
    [CRIT_HEARTBEAT_BOARD] = NULL
};

// heartbeatFaultGetters - gets fault statuses over CAN
bool (*heartbeatFaultGetters[HEARTBEAT_BOARD_COUNT])() = {
    [BMS_HEARTBEAT_BOARD]  = app_canAlerts_FSM_Fault_MissingBMSHeartbeat_get,
    [VC_HEARTBEAT_BOARD]   = NULL,
    [RSM_HEARTBEAT_BOARD]  = NULL,
    [FSM_HEARTBEAT_BOARD]  = NULL,
    [DIM_HEARTBEAT_BOARD]  = NULL,
    [CRIT_HEARTBEAT_BOARD] = NULL
};

void tasks_preInit(void)
{
    // Configure and initialize SEGGER SystemView.
    SEGGER_SYSVIEW_Conf();
    LOG_INFO("FSM reset!");
}

void tasks_init(void)
{
    __HAL_DBGMCU_FREEZE_IWDG();

    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)hw_adc_getRawValuesBuffer(), hadc1.Init.NbrOfConversion);
    HAL_TIM_Base_Start(&htim3);

    hw_hardFaultHandler_init();
    hw_can_init(&can);
    hw_watchdog_init(hw_watchdogConfig_refresh, hw_watchdogConfig_timeoutCallback);

    io_canTx_init(io_jsoncan_pushTxMsgToQueue);
    io_canTx_enableMode(CAN_MODE_DEFAULT, true);
    io_can_init(&can_config);
    io_chimera_init(&debug_uart, GpioNetName_fsm_net_name_tag, AdcNetName_fsm_net_name_tag, &n_chimera_pin);

    app_canTx_init();
    app_canRx_init();

    app_heartbeatMonitor_init(
        heartbeatMonitorChecklist, heartbeatGetters, heartbeatUpdaters, &app_canTx_FSM_Heartbeat_set,
        heartbeatFaultSetters, heartbeatFaultGetters);

    app_stateMachine_init(app_mainState_get());

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
    io_chimera_sleepTaskIfEnabled();

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
    io_chimera_sleepTaskIfEnabled();

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
    io_chimera_sleepTaskIfEnabled();

    for (;;)
    {
        io_can_transmitMsgFromQueue();
    }
}

void tasks_runCanRx(void)
{
    io_chimera_sleepTaskIfEnabled();

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
        io_chimera_msgRxCallback();
    }
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    io_wheels_inputCaptureCallback(htim);
}