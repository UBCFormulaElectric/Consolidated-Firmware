#include "tasks.h"
#include "main.h"
#include "cmsis_os.h"

#include "app_mainState.h"
#include "app_canTx.h"
#include "app_canRx.h"
#include "app_canAlerts.h"
#include "app_commitInfo.h"
#include "app_apps.h"
#include "app_stackWaterMarks.h"
#include "app_utils.h"

#include "app_jsoncan.h"
#include "io_canRx.h"
#include "io_log.h"
#include "io_canQueue.h"
#include "io_led.h"
#include "io_fsmShdn.h"
#include "io_chimera.h"
#include "io_steering.h"
#include "io_wheels.h"
#include "io_brake.h"
#include "io_suspension.h"
#include "io_loadCell.h"
#include "io_apps.h"
#include "io_bootHandler.h"

#include "hw_bootup.h"
#include "hw_utils.h"
#include "hw_hardFaultHandler.h"
#include "hw_watchdog.h"
#include "hw_watchdogConfig.h"
#include "hw_adcs.h"
#include "hw_gpios.h"
#include "hw_uarts.h"
#include "hw_pwmInputFreqOnly.h"
#include "hw_can.h"

#include "shared.pb.h"

#include <assert.h>

static CanHandle can = { .hcan = &hcan1, .bus_num = 1, .receive_callback = io_canQueue_pushRx };
const CanHandle *hw_can_getHandle(const CAN_HandleTypeDef *hcan)
{
    assert(hcan == can.hcan);
    return &can;
}

void canRxQueueOverflowCallBack(const uint32_t overflow_count)
{
    app_canTx_FSM_RxOverflowCount_set(overflow_count);
    app_canAlerts_FSM_Warning_TxOverflow_set(true);
}

void canTxQueueOverflowCallBack(const uint32_t overflow_count)
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

static const BinaryLed led = { .gpio = &led_gpio_pin };

static const AppsConfig             apps_config       = { .papps = &apps1, .sapps = &apps2 };
static const BrakeConfig            brake_config      = { .rear_brake          = &bps_b,
                                                          .front_brake         = &bps_f,
                                                          .brake_hardware_ocsc = &brake_ocsc_ok_3v3,
                                                          .nbspd_brake_pressed = &nbspd_brake_pressed_3v3 };
static const LoadCellConfig         load_cell_config  = { .cell_1 = &lc1, .cell_2 = &lc2 };
static const SteeringConfig         steering_config   = { .steering = &steering_angle };
static const SuspensionConfig       suspension_config = { .front_left_suspension  = &susp_travel_fl,
                                                          .front_right_suspension = &susp_travel_fr };
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
    LOG_INFO("FSM reset!");

    __HAL_DBGMCU_FREEZE_IWDG();

    hw_adcs_chipsInit();

    hw_watchdog_init(hw_watchdogConfig_refresh, hw_watchdogConfig_timeoutCallback);

    io_canTx_init(jsoncan_transmit);
    io_canTx_enableMode_Can(CAN_MODE_DEFAULT, true);
    hw_can_init(&can);
    io_chimera_init(GpioNetName_fsm_net_name_tag, AdcNetName_fsm_net_name_tag);
    app_canTx_init();
    app_canRx_init();
    io_canQueue_init();

    io_apps_init(&apps_config);
    io_brake_init(&brake_config);
    io_loadCell_init(&load_cell_config);
    io_steering_init(&steering_config);
    io_suspension_init(&suspension_config);
    io_wheels_init(&left_wheel_config, &right_wheel_config);
    app_apps_init();

    app_stateMachine_init(app_mainState_get());

    app_canTx_FSM_Hash_set(GIT_COMMIT_HASH);
    app_canTx_FSM_Clean_set(GIT_COMMIT_CLEAN);
    app_canTx_FSM_Heartbeat_set(true);
}

void tasks_deinit(void)
{
    HAL_TIM_Base_Stop_IT(&htim3);
    HAL_TIM_Base_DeInit(&htim3);
    HAL_TIM_Base_Stop_IT(&htim12);
    HAL_TIM_Base_DeInit(&htim12);

    HAL_UART_Abort_IT(&huart1);
    HAL_UART_DeInit(&huart1);

    HAL_DMA_Abort_IT(&hdma_adc1);
    HAL_DMA_DeInit(&hdma_adc1);

    HAL_ADC_Stop_IT(&hadc1);
    HAL_ADC_DeInit(&hadc1);
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
        CanMsg rx_msg = io_canQueue_popRx();
        io_bootHandler_processBootRequest(&rx_msg);

        JsonCanMsg jsoncan_rx_msg = app_jsoncan_copyFromCanMsg(&rx_msg);
        io_canRx_updateRxTableWithMessage(&jsoncan_rx_msg);
    }
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    io_wheels_inputCaptureCallback(htim);
}