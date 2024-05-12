#include "tasks.h"
#include "main.h"
#include "cmsis_os.h"

#include "app_heartbeatMonitor.h"
#include "app_mainState.h"

#include "app_canTx.h"
#include "app_canRx.h"
#include "app_canAlerts.h"
#include "app_commitInfo.h"
#include "app_shdnLoop.h"

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
#include "io_fsmShdn.h"
#include "io_apps.h"

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
#include "hw_pwmInputFreqOnly.h"

#include "shared.pb.h"
#include "FSM.pb.h"

extern ADC_HandleTypeDef hadc1;
extern TIM_HandleTypeDef htim3;
extern CAN_HandleTypeDef hcan1;
extern TIM_HandleTypeDef htim12;

static const CanHandle     can = { .can = &hcan1, .can_msg_received_callback = io_can_pushRxMsgToQueue };
extern UART_HandleTypeDef  huart1;
extern IWDG_HandleTypeDef *hiwdg;

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

void canTxQueueOverflowClearCallback(void)
{
    app_canAlerts_FSM_Warning_TxOverflow_set(false);
}

void canRxQueueOverflowClearCallback(void)
{
    app_canAlerts_FSM_Warning_RxOverflow_set(false);
}

const CanConfig can_config = {
    .rx_msg_filter              = io_canRx_filterMessageId,
    .tx_overflow_callback       = canTxQueueOverflowCallBack,
    .rx_overflow_callback       = canRxQueueOverflowCallBack,
    .tx_overflow_clear_callback = canTxQueueOverflowClearCallback,
    .rx_overflow_clear_callback = canRxQueueOverflowClearCallback,
};

static const Gpio      brake_ocsc_ok_3v3       = { .port = BRAKE_OCSC_OK_3V3_GPIO_Port, .pin = BRAKE_OCSC_OK_3V3_Pin };
static const Gpio      n_chimera_pin           = { .port = NCHIMERA_GPIO_Port, .pin = NCHIMERA_Pin };
static const BinaryLed led                     = { .gpio = { .port = LED_GPIO_Port, .pin = LED_Pin } };
static const Gpio      nbspd_brake_pressed_3v3 = { .port = NBSPD_BRAKE_PRESSED_3V3_GPIO_Port,
                                                   .pin  = NBSPD_BRAKE_PRESSED_3V3_Pin };
static const Gpio      nprogram_3v3            = { .port = NPROGRAM_3V3_GPIO_Port, .pin = NPROGRAM_3V3_Pin };
static const Gpio      fsm_shdn                = { .port = FSM_SHDN_GPIO_Port, .pin = FSM_SHDN_Pin };

const Gpio *const id_to_gpio[] = { [FSM_GpioNetName_BRAKE_OCSC_OK_3V3]       = &brake_ocsc_ok_3v3,
                                   [FSM_GpioNetName_NCHIMERA]                = &n_chimera_pin,
                                   [FSM_GpioNetName_LED]                     = &led.gpio,
                                   [FSM_GpioNetName_NBSPD_BRAKE_PRESSED_3V3] = &nbspd_brake_pressed_3v3,
                                   [FSM_GpioNetName_NPROGRAM_3V3]            = &nprogram_3v3,
                                   [FSM_GpioNetName_FSM_SHDN]                = &fsm_shdn };

const AdcChannel id_to_adc[] = {
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

static const UART debug_uart = { .handle = &huart1 };

static const AppsConfig       apps_config       = { .papps = ADC1_IN12_APPS1, .sapps = ADC1_IN5_APPS2 };
static const BrakeConfig      brake_config      = { .rear_brake = ADC1_IN15_BPS_B, .front_brake = ADC1_IN7_BPS_F };
static const LoadCellConfig   load_cell_config  = { .cell_1 = ADC1_IN13_LOAD_CELL_1, .cell_2 = ADC1_IN1_LOAD_CELL_2 };
static const SteeringConfig   steering_config   = { .steering = ADC1_IN11_STEERING_ANGLE };
static const SuspensionConfig suspension_config = { .front_left_suspension  = ADC1_IN8_SUSP_TRAVEL_FL,
                                                    .front_right_suspension = ADC1_IN9_SUSP_TRAVEL_FR };
static const PwmInputFreqOnlyConfig left_wheel_config = { .htim                = &htim12,
                                                          .tim_frequency_hz    = TIMx_FREQUENCY / TIM12_PRESCALER,
                                                          .tim_channel         = TIM_CHANNEL_2,
                                                          .tim_auto_reload_reg = TIM12_AUTO_RELOAD_REG,
                                                          .tim_active_channel  = HAL_TIM_ACTIVE_CHANNEL_2 };

static const PwmInputFreqOnlyConfig right_wheel_config = { .htim                = &htim12,
                                                           .tim_frequency_hz    = TIMx_FREQUENCY / TIM12_PRESCALER,
                                                           .tim_channel         = TIM_CHANNEL_1,
                                                           .tim_auto_reload_reg = TIM12_AUTO_RELOAD_REG,
                                                           .tim_active_channel  = HAL_TIM_ACTIVE_CHANNEL_1 };

// config for heartbeat monitor (can funcs and flags)
// FSM rellies on BMS
static const bool heartbeatMonitorChecklist[HEARTBEAT_BOARD_COUNT] = {
    [BMS_HEARTBEAT_BOARD] = true,  [VC_HEARTBEAT_BOARD] = false,  [RSM_HEARTBEAT_BOARD] = false,
    [FSM_HEARTBEAT_BOARD] = false, [DIM_HEARTBEAT_BOARD] = false, [CRIT_HEARTBEAT_BOARD] = false
};

// heartbeatGetters - get heartbeat signals from other boards
static bool (*const heartbeatGetters[HEARTBEAT_BOARD_COUNT])(void) = {
    [BMS_HEARTBEAT_BOARD]  = app_canRx_BMS_Heartbeat_get,
    [VC_HEARTBEAT_BOARD]   = NULL,
    [RSM_HEARTBEAT_BOARD]  = NULL,
    [FSM_HEARTBEAT_BOARD]  = NULL,
    [DIM_HEARTBEAT_BOARD]  = NULL,
    [CRIT_HEARTBEAT_BOARD] = NULL
};

// heartbeatUpdaters - update local CAN table with heartbeat status
static void (*const heartbeatUpdaters[HEARTBEAT_BOARD_COUNT])(bool) = {
    [BMS_HEARTBEAT_BOARD]  = app_canRx_BMS_Heartbeat_update,
    [VC_HEARTBEAT_BOARD]   = NULL,
    [RSM_HEARTBEAT_BOARD]  = NULL,
    [FSM_HEARTBEAT_BOARD]  = NULL,
    [DIM_HEARTBEAT_BOARD]  = NULL,
    [CRIT_HEARTBEAT_BOARD] = NULL
};

// heartbeatFaultSetters - broadcast heartbeat faults over CAN
static void (*const heartbeatFaultSetters[HEARTBEAT_BOARD_COUNT])(bool) = {
    [BMS_HEARTBEAT_BOARD]  = app_canAlerts_FSM_Fault_MissingBMSHeartbeat_set,
    [VC_HEARTBEAT_BOARD]   = NULL,
    [RSM_HEARTBEAT_BOARD]  = NULL,
    [FSM_HEARTBEAT_BOARD]  = NULL,
    [DIM_HEARTBEAT_BOARD]  = NULL,
    [CRIT_HEARTBEAT_BOARD] = NULL
};

// heartbeatFaultGetters - gets fault statuses over CAN
static bool (*const heartbeatFaultGetters[HEARTBEAT_BOARD_COUNT])(void) = {
    [BMS_HEARTBEAT_BOARD]  = app_canAlerts_FSM_Fault_MissingBMSHeartbeat_get,
    [VC_HEARTBEAT_BOARD]   = NULL,
    [RSM_HEARTBEAT_BOARD]  = NULL,
    [FSM_HEARTBEAT_BOARD]  = NULL,
    [DIM_HEARTBEAT_BOARD]  = NULL,
    [CRIT_HEARTBEAT_BOARD] = NULL
};

void tasks_preInit(void)
{
    hw_bootup_enableInterruptsForApp();

    // Configure and initialize SEGGER SystemView.
    SEGGER_SYSVIEW_Conf();
    LOG_INFO("FSM reset!");
}

static const FsmShdnConfig fsm_shdn_pin_config = { .fsm_shdn_ok_gpio = fsm_shdn };

static const BoardShdnNode fsm_bshdn_nodes[FsmShdnNodeCount] = { { &io_fsmShdn_FSM_SHDN_OK_get,
                                                                   &app_canTx_FSM_BOTSOKStatus_set } };

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
    io_fsmShdn_init(&fsm_shdn_pin_config);
    app_canTx_init();
    app_canRx_init();

    app_shdn_loop_init(fsm_bshdn_nodes, FsmShdnNodeCount);

    io_apps_init(&apps_config);
    io_brake_init(&brake_config);
    io_loadCell_init(&load_cell_config);
    io_steering_init(&steering_config);
    io_suspension_init(&suspension_config);
    io_wheels_init(&left_wheel_config, &right_wheel_config);

    app_heartbeatMonitor_init(
        heartbeatMonitorChecklist, heartbeatGetters, heartbeatUpdaters, &app_canTx_FSM_Heartbeat_set,
        heartbeatFaultSetters, heartbeatFaultGetters);

    app_stateMachine_init(app_mainState_get());

    app_canTx_FSM_Hash_set(GIT_COMMIT_HASH);
    app_canTx_FSM_Clean_set(GIT_COMMIT_CLEAN);
}

_Noreturn void tasks_run1Hz(void)
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

_Noreturn void tasks_run100Hz(void)
{
    io_chimera_sleepTaskIfEnabled();

    static const TickType_t period_ms = 10;
    WatchdogHandle         *watchdog  = hw_watchdog_allocateWatchdog();
    hw_watchdog_initWatchdog(watchdog, RTOS_TASK_100HZ, period_ms);

    static uint32_t start_ticks = 0;
    start_ticks                 = osKernelGetTickCount();

    for (;;)
    {
        //        const uint32_t start_time_ms = osKernelGetTickCount();

        app_stateMachine_tick100Hz();
        io_canTx_enqueue100HzMsgs();

        // Watchdog check-in must be the last function called before putting the
        // task to sleep.
        hw_watchdog_checkIn(watchdog);

        start_ticks += period_ms;
        osDelayUntil(start_ticks);
    }
}

_Noreturn void tasks_run1kHz(void)
{
    io_chimera_sleepTaskIfEnabled();

    static const TickType_t period_ms = 1U;
    WatchdogHandle         *watchdog  = hw_watchdog_allocateWatchdog();
    hw_watchdog_initWatchdog(watchdog, RTOS_TASK_1KHZ, period_ms);

    static uint32_t start_ticks = 0;
    start_ticks                 = osKernelGetTickCount();

    for (;;)
    {
        //        const uint32_t start_time_ms = osKernelGetTickCount();

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

_Noreturn void tasks_runCanTx(void)
{
    io_chimera_sleepTaskIfEnabled();

    for (;;)
    {
        io_can_transmitMsgFromQueue();
    }
}

_Noreturn void tasks_runCanRx(void)
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
