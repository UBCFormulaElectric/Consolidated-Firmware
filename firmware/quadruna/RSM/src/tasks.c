#include "tasks.h"
#include "main.h"
#include "cmsis_os.h"

#include "app_mainState.h"
#include "app_canTx.h"
#include "app_canRx.h"
#include "app_canAlerts.h"
#include "app_commitInfo.h"
#include "app_coolant.h"
#include "app_heartbeatMonitors.h"
#include "app_stackWaterMarks.h"
#include "utils.h"

#include "app_jsoncan.h"
#include "io_canTx.h"
#include "io_log.h"
#include "io_chimera.h"
#include "io_coolant.h"
#include "io_fans.h"
#include "io_brake_light.h"
#include "io_canQueue.h"
#include "io_bootHandler.h"

#include "hw_bootup.h"
#include "hw_utils.h"
#include "hw_hardFaultHandler.h"
#include "hw_watchdog.h"
#include "hw_watchdogConfig.h"
#include "hw_adcs.h"
#include "hw_gpio.h"
#include "hw_uart.h"
#include "hw_uarts.h"
#include "hw_can.h"

#include "shared.pb.h"
#include "RSM.pb.h"

#include <assert.h>

static CanHandle can = { .hcan = &hcan1, .bus_num = 2, .receive_callback = io_canQueue_pushRx };

const CanHandle *hw_can_getHandle(const CAN_HandleTypeDef *hcan)
{
    assert(hcan == can.hcan);
    return &can;
}

void canRxQueueOverflowCallBack(uint32_t overflow_count)
{
    app_canTx_RSM_RxOverflowCount_set(overflow_count);
    app_canAlerts_RSM_Warning_RxOverflow_set(true);
}

void canTxQueueOverflowCallBack(uint32_t overflow_count)
{
    app_canTx_RSM_TxOverflowCount_set(overflow_count);
    app_canAlerts_RSM_Warning_TxOverflow_set(true);
}

void canTxQueueOverflowClearCallback(void)
{
    app_canAlerts_RSM_Warning_TxOverflow_set(false);
}

void canRxQueueOverflowClearCallback(void)
{
    app_canAlerts_RSM_Warning_RxOverflow_set(false);
}

static const Gpio n_chimera_pin      = { .port = NCHIMERA_GPIO_Port, .pin = NCHIMERA_Pin };
static const Gpio led_pin            = { .port = LED_GPIO_Port, .pin = LED_Pin };
static const Gpio rad_fan_en_pin     = { .port = RAD_FAN_EN_GPIO_Port, .pin = RAD_FAN_EN_Pin };
static const Gpio fr_stby_pin        = { .port = FR_STBY_GPIO_Port, .pin = FR_STBY_Pin };
static const Gpio brake_light_en_pin = { .port = BRAKE_LIGHT_EN_3V3_GPIO_Port, .pin = BRAKE_LIGHT_EN_3V3_Pin };
static const Gpio acc_fan_en_pin     = { .port = ACC_FAN_EN_GPIO_Port, .pin = ACC_FAN_EN_Pin };
static const Gpio n_program_pin      = { .port = NProgram_3V3_GPIO_Port, .pin = NProgram_3V3_Pin };
static const Gpio acc_fan_pin        = { .port = ACC_FAN_EN_GPIO_Port, .pin = RAD_FAN_EN_Pin };
static const Gpio rad_fan_pin        = { .port = RAD_FAN_EN_GPIO_Port, .pin = ACC_FAN_EN_Pin };
static const Gpio brake_light_pin    = { .port = BRAKE_LIGHT_EN_3V3_GPIO_Port, .pin = BRAKE_LIGHT_EN_3V3_Pin };

static const BinaryLed brake_light = { .gpio = &brake_light_pin };

const Gpio *id_to_gpio[] = { [RSM_GpioNetName_NCHIMERA]           = &n_chimera_pin,
                             [RSM_GpioNetName_LED]                = &led_pin,
                             [RSM_GpioNetName_RAD_FAN_EN]         = &rad_fan_en_pin,
                             [RSM_GpioNetName_FR_STBY]            = &fr_stby_pin,
                             [RSM_GpioNetName_BRAKE_LIGHT_EN_3V3] = &brake_light_en_pin,
                             [RSM_GpioNetName_ACC_FAN_EN]         = &acc_fan_en_pin,
                             [RSM_GpioNetName_NProgram_3V3]       = &n_program_pin };

const AdcChannel *const id_to_adc[] = {
    [RSM_AdcNetName_ACC_FAN_I_SNS]        = &acc_fan_i_sns,
    [RSM_AdcNetName_RAD_FAN_I_SNS]        = &rad_fan_i_sns,
    [RSM_AdcNetName_CoolantPressure1_3V3] = &coolant_pressure_1,
    [RSM_AdcNetName_CoolantPressure2_3V3] = &coolant_pressure_2,
    [RSM_AdcNetName_LC3_OUT]              = &lc3,
    [RSM_AdcNetName_SUSP_TRAVEL_RR_3V3]   = &susp_travel_rr,
    [RSM_AdcNetName_SUSP_TRAVEL_RL_3V3]   = &susp_travel_rl,
    [RSM_AdcNetName_CoolantTemp2_3V3]     = &coolant_temp_2,
    [RSM_AdcNetName_CoolantTemp1_3V3]     = &coolant_temp_1,
    [RSM_AdcNetName_LC4_OUT]              = &lc4,
};

PwmInputFreqOnlyConfig coolant_config = { .htim                = &htim3,
                                          .tim_frequency_hz    = TIMx_FREQUENCY / TIM12_PRESCALER,
                                          .tim_channel         = TIM_CHANNEL_1,
                                          .tim_auto_reload_reg = TIM12_AUTO_RELOAD_REG,
                                          .tim_active_channel  = HAL_TIM_ACTIVE_CHANNEL_1 };

const Gpio *n_chimera_gpio = &n_chimera_pin;

void tasks_preInit(void)
{
    hw_bootup_enableInterruptsForApp();
}

static void jsoncan_transmit(const JsonCanMsg *tx_msg)
{
    const CanMsg msg = app_jsoncan_copyToCanMsg(tx_msg);
    io_canQueue_pushTx(&msg);
}

void tasks_init(void)
{
    // Configure and initialize SEGGER SystemView.
    // NOTE: Needs to be done after clock config!
    SEGGER_SYSVIEW_Conf();
    LOG_INFO("RSM reset!");

    __HAL_DBGMCU_FREEZE_IWDG();

    hw_adcs_chipsInit();

    hw_watchdog_init(hw_watchdogConfig_refresh, hw_watchdogConfig_timeoutCallback);

    hw_can_init(&can);
    io_canTx_init(jsoncan_transmit);
    io_canTx_enableMode_Can(CAN_MODE_DEFAULT, true);
    io_canQueue_init();
    io_chimera_init(GpioNetName_rsm_net_name_tag, AdcNetName_rsm_net_name_tag);

    app_canTx_init();
    app_canRx_init();

    io_coolant_init(&coolant_config);
    io_fan_init(&acc_fan_pin, &rad_fan_pin);
    io_brake_light_init(&brake_light);
    app_coolant_init();

    app_heartbeatMonitor_init(&hb_monitor);
    app_stateMachine_init(app_mainState_get());

    app_canTx_RSM_Hash_set(GIT_COMMIT_HASH);
    app_canTx_RSM_Clean_set(GIT_COMMIT_CLEAN);
    app_canTx_RSM_Heartbeat_set(true);
}

void tasks_deinit(void)
{
    HAL_TIM_Base_Stop_IT(&htim3);
    HAL_TIM_Base_DeInit(&htim3);

    HAL_UART_Abort_IT(&huart1);
    HAL_UART_DeInit(&huart1);

    HAL_ADC_Stop_IT(&hadc1);
    HAL_ADC_DeInit(&hadc1);

    HAL_DMA_Abort_IT(&hdma_adc1);
    HAL_DMA_DeInit(&hdma_adc1);
}

_Noreturn void tasks_run1Hz(void)
{
    io_chimera_sleepTaskIfEnabled();

    static const TickType_t period_ms = 1000U;
    WatchdogHandle         *watchdog  = hw_watchdog_allocateWatchdog();
    hw_watchdog_initWatchdog(watchdog, RTOS_TASK_1HZ, period_ms);

    static uint32_t start_ticks = 0;
    start_ticks                 = osKernelGetTickCount();

    for (;;)
    {
        app_stackWaterMark_check();
        app_stateMachine_tick1Hz();

        const bool debug_mode_enabled = app_canRx_Debug_EnableDebugMode_get();
        io_canTx_enableMode_Can(CAN_MODE_DEBUG, debug_mode_enabled);
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
        CanMsg tx_msg = io_canQueue_popTx();
        LOG_IF_ERR(hw_can_transmit(&can, &tx_msg));
    }
}

_Noreturn void tasks_runCanRx(void)
{
    io_chimera_sleepTaskIfEnabled();
    for (;;)
    {
        CanMsg     rx_msg         = io_canQueue_popRx();
        JsonCanMsg jsoncan_rx_msg = app_jsoncan_copyFromCanMsg(&rx_msg);

        io_bootHandler_processBootRequest(&rx_msg);
        io_canRx_updateRxTableWithMessage(&jsoncan_rx_msg);
    }
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    io_coolant_inputCaptureCallback(htim);
}