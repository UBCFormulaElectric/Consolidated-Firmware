#include "tasks.h"
#include "main.h"
#include "cmsis_os.h"

#include "app_canTx.h"
#include "app_canRx.h"
#include "app_canAlerts.h"
#include "app_commitInfo.h"
#include "app_timer.h"
#include "app_thermistors.h"
#include "app_accumulator.h"
#include "app_soc.h"
#include "app_globals.h"
#include "states/app_allStates.h"
#include "states/app_initState.h"
#include "states/app_inverterOnState.h"
#include "app_stateMachine.h"

#include "io_canTx.h"
#include "io_canRx.h"
#include "io_jsoncan.h"
#include "io_time.h"
#include "io_can.h"
#include "io_airs.h"
#include "io_charger.h"
#include "io_eeprom.h"
#include "io_faultLatch.h"
#include "io_imd.h"
#include "ltc6813/io_ltc6813Shared.h"
#include "io_thermistors.h"
#include "io_tractiveSystem.h"
#include "io_log.h"

#include "hw_can.h"
#include "hw_adc.h"
#include "hw_hardFaultHandler.h"
#include "hw_bootup.h"
#include "hw_utils.h"
#include "hw_spi.h"
#include "hw_pwmInput.h"
#include "hw_stackWaterMarkConfig.h"
#include "hw_watchdogConfig.h"
#include "hw_watchdog.h"
#include "hw_spi.h"

#define HEARTBEAT_MONITOR_TIMEOUT_PERIOD_MS 300U

static void canRxQueueOverflowCallBack(uint32_t overflow_count)
{
    app_canTx_BMS_RxOverflowCount_set(overflow_count);
    app_canAlerts_BMS_Warning_RxOverflow_set(true);
}

static void canTxQueueOverflowCallBack(uint32_t overflow_count)
{
    app_canTx_BMS_TxOverflowCount_set(overflow_count);
    app_canAlerts_BMS_Warning_TxOverflow_set(true);
}

extern ADC_HandleTypeDef  hadc1;
extern CAN_HandleTypeDef  hcan1;
extern IWDG_HandleTypeDef hiwdg;
extern SPI_HandleTypeDef  hspi1;
extern TIM_HandleTypeDef  htim1;
extern TIM_HandleTypeDef  htim13;

static const CanConfig can_config = {
    .rx_msg_filter        = io_canRx_filterMessageId,
    .tx_overflow_callback = canTxQueueOverflowCallBack,
    .rx_overflow_callback = canRxQueueOverflowCallBack,
};

static CanHandle can = {
    .can                   = &hcan1,
    .canMsgRecievecallback = NULL,
};

PwmInputConfig imd_pwm_input_config = {
    .htim                     = &htim1,
    .timer_frequency_hz       = TIM1_FREQUENCY / TIM1_PRESCALER,
    .rising_edge_tim_channel  = TIM_CHANNEL_2,
    .falling_edge_tim_channel = TIM_CHANNEL_1,
};

static const SpiInterface ltc6813_spi = { .spi_handle = &hspi1,
                                          .nss_port   = SPI1_NSS_GPIO_Port,
                                          .nss_pin    = SPI1_NSS_Pin,
                                          .timeout_ms = LTC6813_SPI_TIMEOUT_MS };

static const Charger charger_config  = { .enable_gpio = {
                                       .port = CHRG_EN_3V3_GPIO_Port,
                                       .pin  = CHRG_EN_3V3_Pin,
                                   },
                                   .connected_gpio = {
                                       .port = CHRG_STATE_3V3_GPIO_Port,
                                       .pin  = CHRG_STATE_3V3_Pin,
                                   },
                                   .faulted_gpio = {
                                       .port = CHRG_FLT_3V3_GPIO_Port,
                                       .pin  = CHRG_FLT_3V3_Pin,
                                   }};

static const ThermistorsConfig thermistors_config = {.mux_0_gpio = {
                                       .port = AUX_TSENSE_MUX0_GPIO_Port,
                                       .pin  = AUX_TSENSE_MUX0_Pin,
                                   },
                                   .mux_1_gpio = {
                                       .port = AUX_TSENSE_MUX1_GPIO_Port,
                                       .pin  = AUX_TSENSE_MUX1_Pin,
                                   },
                                   .mux_2_gpio = {
                                       .port = AUX_TSENSE_MUX2_GPIO_Port,
                                       .pin  = AUX_TSENSE_MUX2_Pin,
                                   },
                                   .mux_3_gpio = {
                                       .port = AUX_TSENSE_MUX3_GPIO_Port,
                                       .pin  = AUX_TSENSE_MUX3_Pin,
                                   },
                                   .thermistor_adc_channel = ADC1_CHANNEL_14
                                   };

static const AirsConfig airs_config = { .air_p_gpio = {
                                       .port = AIR_P_EN_GPIO_Port,
                                       .pin  = AIR_P_EN_Pin,
                                   },
                                   .air_n_gpio = {
                                       .port = AIR_N_STATUS_GPIO_Port,
                                       .pin  = AIR_N_STATUS_Pin,
                                   },
                                   .precharge_gpio = {
                                       .port = PRECHARGE_EN_GPIO_Port,
                                       .pin  = PRECHARGE_EN_Pin,
                                   },
                                   .loop_isense_channel = ADC1_CHANNEL_3
};

static const TractiveSystemConfig ts_config = { .ts_vsense_p_channel        = ADC1_CHANNEL_10,
                                                .ts_vsense_n_channel        = ADC1_CHANNEL_11,
                                                .ts_isense_high_res_channel = ADC1_CHANNEL_8,
                                                .ts_isense_low_res_channel  = ADC1_CHANNEL_9

};

static const FaultLatch bms_ok_latch = {
    .current_status_gpio = {
        .port = BMS_OK_3V3_GPIO_Port,
        .pin = BMS_OK_3V3_Pin,
    },
    .latch_status_gpio = {
        .port = BMS_LATCH_GPIO_Port,
        .pin = BMS_LATCH_Pin,
    },
    .read_only = false,
};

static const FaultLatch imd_ok_latch = {
    .current_status_gpio = {
        .port = IMD_OK_3V3_GPIO_Port,
        .pin = IMD_OK_3V3_Pin,
    },
    .latch_status_gpio = {
        .port = IMD_LATCH_GPIO_Port,
        .pin = IMD_LATCH_Pin,
    },
    .read_only = true,
};

static const FaultLatch bspd_ok_latch = {
    .current_status_gpio = {
        .port = BSPD_OK_3V3_GPIO_Port,
        .pin = BSPD_OK_3V3_Pin,
    },
    .latch_status_gpio = {
        .port = BSPD_LATCH_GPIO_Port,
        .pin = BSPD_LATCH_Pin,
    },
    .read_only = true,
};

static const GlobalsConfig globals_config = {
    .bms_ok_latch  = &bms_ok_latch,
    .imd_ok_latch  = &imd_ok_latch,
    .bspd_ok_latch = &bspd_ok_latch,
};

// config to forward can functions to shared heartbeat
// BMS rellies on DCM, PDM, and FSM
bool heartbeatMonitorChecklist[HEARTBEAT_BOARD_COUNT] = { [BMS_HEARTBEAT_BOARD] = false,
                                                          [DCM_HEARTBEAT_BOARD] = true,
                                                          [PDM_HEARTBEAT_BOARD] = true,
                                                          [FSM_HEARTBEAT_BOARD] = true,
                                                          [DIM_HEARTBEAT_BOARD] = false };
// heartbeatGetters - get heartbeat signals from other boards
bool (*heartbeatGetters[HEARTBEAT_BOARD_COUNT])() = { [BMS_HEARTBEAT_BOARD] = NULL,
                                                      [DCM_HEARTBEAT_BOARD] = &app_canRx_DCM_Heartbeat_get,
                                                      [PDM_HEARTBEAT_BOARD] = &app_canRx_PDM_Heartbeat_get,
                                                      [FSM_HEARTBEAT_BOARD] = &app_canRx_FSM_Heartbeat_get,
                                                      [DIM_HEARTBEAT_BOARD] = NULL };

// heartbeatUpdaters - update local CAN table with heartbeat status
void (*heartbeatUpdaters[HEARTBEAT_BOARD_COUNT])(bool) = { [BMS_HEARTBEAT_BOARD] = NULL,
                                                           [DCM_HEARTBEAT_BOARD] = &app_canRx_DCM_Heartbeat_update,
                                                           [PDM_HEARTBEAT_BOARD] = &app_canRx_PDM_Heartbeat_update,
                                                           [FSM_HEARTBEAT_BOARD] = &app_canRx_FSM_Heartbeat_update,
                                                           [DIM_HEARTBEAT_BOARD] = NULL };

// heartbeatFaultSetters - broadcast heartbeat faults over CAN
void (*heartbeatFaultSetters[HEARTBEAT_BOARD_COUNT])(bool) = {
    [BMS_HEARTBEAT_BOARD] = NULL,
    [DCM_HEARTBEAT_BOARD] = &app_canAlerts_BMS_Fault_MissingDCMHeartbeat_set,
    [PDM_HEARTBEAT_BOARD] = &app_canAlerts_BMS_Fault_MissingPDMHeartbeat_set,
    [FSM_HEARTBEAT_BOARD] = &app_canAlerts_BMS_Fault_MissingFSMHeartbeat_set,
    [DIM_HEARTBEAT_BOARD] = NULL
};

// heartbeatFaultGetters - gets fault statuses over CAN
bool (*heartbeatFaultGetters[HEARTBEAT_BOARD_COUNT])() = {
    [BMS_HEARTBEAT_BOARD] = NULL,
    [DCM_HEARTBEAT_BOARD] = &app_canAlerts_BMS_Fault_MissingDCMHeartbeat_get,
    [PDM_HEARTBEAT_BOARD] = &app_canAlerts_BMS_Fault_MissingPDMHeartbeat_get,
    [FSM_HEARTBEAT_BOARD] = &app_canAlerts_BMS_Fault_MissingFSMHeartbeat_get,
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
    LOG_INFO("BMS reset!");

    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)hw_adc_getRawValuesBuffer(), hadc1.Init.NbrOfConversion);
    HAL_TIM_Base_Start(&htim13);

    hw_hardFaultHandler_init();
    hw_can_init(&can);
    hw_watchdog_init(hw_watchdogConfig_refresh, hw_watchdogConfig_timeoutCallback);

    io_canTx_init(io_jsoncan_pushTxMsgToQueue);
    io_canTx_enableMode(CAN_MODE_DEFAULT, true);
    io_can_init(&can_config);

    io_tractiveSystem_init(&ts_config);
    io_thermistors_init(&thermistors_config);
    io_ltc6813Shared_init(&ltc6813_spi);
    io_airs_init(&airs_config);
    io_imd_init(&imd_pwm_input_config);
    io_charger_init(&charger_config);

    app_canTx_init();
    app_canRx_init();

    app_globals_init(&globals_config);
    app_inverterOnState_init();
    app_accumulator_init();
    app_thermistors_init();
    app_soc_init();
    app_stateMachine_init(app_initState_get());

    app_heartbeatMonitor_init(
        HEARTBEAT_MONITOR_TIMEOUT_PERIOD_MS, heartbeatMonitorChecklist, heartbeatGetters, heartbeatUpdaters,
        &app_canTx_BMS_Heartbeat_set, heartbeatFaultSetters, heartbeatFaultGetters);

    // broadcast commit info
    app_canTx_BMS_Hash_set(GIT_COMMIT_HASH);
    app_canTx_BMS_Clean_set(GIT_COMMIT_CLEAN);
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
    static const TickType_t period_ms = 1;
    WatchdogHandle         *watchdog  = hw_watchdog_allocateWatchdog();
    hw_watchdog_initWatchdog(watchdog, RTOS_TASK_1KHZ, period_ms);

    static uint32_t start_ticks = 0;
    start_ticks                 = osKernelGetTickCount();

    for (;;)
    {
        // ADC wasn't reading any voltages when triggered by TIM3 like on other boards
        // But worked fine when starting the conversion via software as below
        // TODO: Figure out why
        HAL_ADC_Start_DMA(&hadc1, (uint32_t *)hw_adc_getRawValuesBuffer(), hadc1.Init.NbrOfConversion);

        // Check in for timeouts for all RTOS tasks
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