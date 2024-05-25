#include "tasks.h"
#include "main.h"
#include "cmsis_os.h"

#include "hw_can.h"
#include "hw_adc.h"
#include "hw_gpio.h"
#include "hw_hardFaultHandler.h"
#include "hw_bootup.h"
#include "hw_utils.h"
#include "hw_spi.h"
#include "hw_pwmInput.h"
#include "hw_stackWaterMarkConfig.h"
#include "hw_watchdogConfig.h"
#include "hw_watchdog.h"
#include "hw_uart.h"
#include "hw_sd.h"
#include "hw_crc.h"

#include "io_canTx.h"
#include "io_canRx.h"
#include "io_jsoncan.h"
#include "io_can.h"
#include "io_airs.h"
#include "io_charger.h"
#include "io_sd.h"
#include "io_faultLatch.h"
#include "io_imd.h"
#include "ltc6813/io_ltc6813Shared.h"
#include "io_thermistors.h"
#include "io_tractiveSystem.h"
#include "io_log.h"
#include "io_chimera.h"
#include "io_bmsShdn.h"

#include "app_canTx.h"
#include "app_canRx.h"
#include "app_canAlerts.h"
#include "app_commitInfo.h"
#include "app_thermistors.h"
#include "app_accumulator.h"
#include "app_soc.h"
#include "app_globals.h"
#include "states/app_initState.h"
#include "states/app_inverterOnState.h"
#include "app_stateMachine.h"
#include "app_shdnLoop.h"

#include "shared.pb.h"
#include "BMS.pb.h"

extern ADC_HandleTypeDef   hadc1;
extern FDCAN_HandleTypeDef hfdcan1;
extern SPI_HandleTypeDef   hspi2;
extern TIM_HandleTypeDef   htim1;
extern TIM_HandleTypeDef   htim3;
extern TIM_HandleTypeDef   htim15;
extern UART_HandleTypeDef  huart1;
extern SD_HandleTypeDef    hsd1;
extern CRC_HandleTypeDef   hcrc;

static void canRxQueueOverflowCallBack(uint32_t overflow_count)
{
    app_canTx_BMS_RxOverflowCount_set(overflow_count);
    app_canAlerts_BMS_Warning_RxOverflow_set(true);
    LOG_INFO("CAN RX QUEUE OVERFLOW, overflow_count = %d", overflow_count);
}

static void canTxQueueOverflowCallBack(uint32_t overflow_count)
{
    app_canTx_BMS_TxOverflowCount_set(overflow_count);
    app_canAlerts_BMS_Warning_TxOverflow_set(true);
    LOG_INFO("CAN TX QUEUE OVERFLOW, overflow_count = %d", overflow_count);
}

void canTxQueueOverflowClearCallback(void)
{
    app_canAlerts_BMS_Warning_TxOverflow_set(false);
}

void canRxQueueOverflowClearCallback(void)
{
    app_canAlerts_BMS_Warning_RxOverflow_set(false);
}

static const CanConfig can_config = {
    .rx_msg_filter              = io_canRx_filterMessageId,
    .tx_overflow_callback       = canTxQueueOverflowCallBack,
    .rx_overflow_callback       = canRxQueueOverflowCallBack,
    .tx_overflow_clear_callback = canTxQueueOverflowClearCallback,
    .rx_overflow_clear_callback = canRxQueueOverflowClearCallback,
};

// clang-format off
static const Gpio accel_brake_ok_pin      = { .port = ACCEL_BRAKE_OK_3V3_GPIO_Port, .pin = ACCEL_BRAKE_OK_3V3_Pin };
static const Gpio bspd_test_en_pin        = { .port = BSPD_TEST_EN_GPIO_Port, .pin = BSPD_TEST_EN_Pin };
static const Gpio led_pin                 = { .port = LED_GPIO_Port, .pin = LED_Pin };
static const Gpio n_chimera_pin           = { .port = nCHIMERA_GPIO_Port, .pin = nCHIMERA_Pin };
static const Gpio n_high_current_bspd_pin = { .port = nHIGH_CURRENT_BSPD_3V3_GPIO_Port, .pin  = nHIGH_CURRENT_BSPD_3V3_Pin };
static const Gpio n_program_pin               = { .port = nPROGRAM_3V3_GPIO_Port, .pin = nPROGRAM_3V3_Pin };
static const Gpio ts_ilck_shdn_pin        = { .port = TS_ILCK_SHDN_OK_GPIO_Port, .pin = TS_ILCK_SHDN_OK_Pin };
static const Gpio hvd_ok_shdn_pin = { .port = HVD_SHDN_OK_GPIO_Port, .pin = HVD_SHDN_OK_Pin };
static const Gpio ts_isense_ocsc_ok_pin = { .port = TS_ISENSE_OCSC_OK_3V3_GPIO_Port, .pin = TS_ISENSE_OCSC_OK_3V3_Pin };
static const Gpio sd_cd_pin             = { .port = SD_CD_GPIO_Port, .pin = SD_CD_Pin };
static const Gpio spi_cs_pin                = { .port = SPI_CS_GPIO_Port, .pin = SPI_CS_Pin };
// clang-format on

static SdCard sd = {
    .hsd     = &hsd1,
    .timeout = 0x20, // osWaitForever,
    // .sd_present       = { .port = SD_CD_GPIO_Port, .pin = SD_CD_Pin },
    .sd_init_complete = false,
};

static const PwmInputConfig imd_pwm_input_config = {
    .htim                     = &htim1,
    .timer_frequency_hz       = TIM1_FREQUENCY / TIM1_PRESCALER,
    .rising_edge_tim_channel  = TIM_CHANNEL_1,
    .falling_edge_tim_channel = TIM_CHANNEL_2,
};

static const CanHandle can = { .can = &hfdcan1, .can_msg_received_callback = io_can_pushRxMsgToQueue };

static const SpiInterface ltc6813_spi = { .spi_handle = &hspi2,
                                          .nss_port   = SPI_CS_GPIO_Port,
                                          .nss_pin    = SPI_CS_Pin,
                                          .timeout_ms = LTC6813_SPI_TIMEOUT_MS };

// TODO: Update for new charger

// static const Charger charger_config  = { .enable_gpio = {
//                                        .port = CHRG_EN_3V3_GPIO_Port,
//                                        .pin  = CHRG_EN_3V3_Pin,
//                                    },
//                                    .connected_gpio = {
//                                        .port = CHRG_STATE_3V3_GPIO_Port,
//                                        .pin  = CHRG_STATE_3V3_Pin,
//                                    },
//                                    .faulted_gpio = {
//                                        .port = CHRG_FLT_3V3_GPIO_Port,
//                                        .pin  = CHRG_FLT_3V3_Pin,
//                                    }};

static const SdGpio sd_gpio = { .sd_present = {
                                    .port = SD_CD_GPIO_Port,
                                    .pin  = SD_CD_Pin,
                                } };

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
                                   .thermistor_adc_channel = ADC1_IN4_AUX_TSENSE
                                   };

static const AirsConfig airs_config = { .air_p_gpio = {
                                       .port = AIR_P_EN_GPIO_Port,
                                       .pin  = AIR_P_EN_Pin,
                                   },
                                   .air_n_gpio = {
                                       .port = HVD_SHDN_OK_GPIO_Port,
                                       .pin  = HVD_SHDN_OK_Pin,
                                   },
                                   .precharge_gpio = {
                                       .port = PRE_CHARGE_EN_GPIO_Port,
                                       .pin  = PRE_CHARGE_EN_Pin,
                                   }
};

static const TractiveSystemConfig ts_config = { .ts_vsense_channel_P        = ADC1_IN10_TS_VSENSE_P,
                                                .ts_vsense_channel_N        = ADC1_IN11_TS_VSENSE_N,
                                                .ts_isense_high_res_channel = ADC1_IN5_TS_ISENSE_75A,
                                                .ts_isense_low_res_channel  = ADC1_IN9_TS_ISENSE_400A

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

const Gpio *id_to_gpio[] = { [BMS_GpioNetName_ACCEL_BRAKE_OK_3V3]     = &accel_brake_ok_pin,
                             [BMS_GpioNetName_AIR_P_EN]               = &airs_config.air_p_gpio,
                             [BMS_GpioNetName_AUX_TSENSE_MUX0]        = &thermistors_config.mux_0_gpio,
                             [BMS_GpioNetName_AUX_TSENSE_MUX1]        = &thermistors_config.mux_1_gpio,
                             [BMS_GpioNetName_AUX_TSENSE_MUX2]        = &thermistors_config.mux_2_gpio,
                             [BMS_GpioNetName_AUX_TSENSE_MUX3]        = &thermistors_config.mux_3_gpio,
                             [BMS_GpioNetName_BMS_LATCH]              = &bms_ok_latch.latch_status_gpio,
                             [BMS_GpioNetName_BMS_OK_3V3]             = &bms_ok_latch.current_status_gpio,
                             [BMS_GpioNetName_BSPD_LATCH]             = &bspd_ok_latch.latch_status_gpio,
                             [BMS_GpioNetName_BSPD_OK_3V3]            = &bspd_ok_latch.current_status_gpio,
                             [BMS_GpioNetName_BSPD_TEST_EN]           = &bspd_test_en_pin,
                             [BMS_GpioNetName_HVD_SHDN_OK]            = &airs_config.air_n_gpio,
                             [BMS_GpioNetName_IMD_LATCH]              = &imd_ok_latch.latch_status_gpio,
                             [BMS_GpioNetName_IMD_OK_3V3]             = &imd_ok_latch.current_status_gpio,
                             [BMS_GpioNetName_LED]                    = &led_pin,
                             [BMS_GpioNetName_NCHIMERA]               = &n_chimera_pin,
                             [BMS_GpioNetName_NHIGH_CURRENT_BSPD_3V3] = &n_high_current_bspd_pin,
                             [BMS_GpioNetName_NPROGRAM_3V3]           = &n_program_pin,
                             [BMS_GpioNetName_PRE_CHARGE_EN]          = &airs_config.precharge_gpio,
                             [BMS_GpioNetName_TS_ILCK_SHDN_OK]        = &ts_ilck_shdn_pin,
                             [BMS_GpioNetName_TS_ISENSE_OCSC_OK_3V3]  = &ts_isense_ocsc_ok_pin,
                             [BMS_GpioNetName_SD_CD]                  = &sd_cd_pin,
                             [BMS_GpioNetName_SPI_CS]                 = &spi_cs_pin };

const AdcChannel id_to_adc[] = {
    [BMS_AdcNetName_AUX_TSENSE] = ADC1_IN4_AUX_TSENSE,       [BMS_AdcNetName_TS_ISENSE_400A] = ADC1_IN9_TS_ISENSE_400A,
    [BMS_AdcNetName_TS_ISENSE_75A] = ADC1_IN5_TS_ISENSE_75A, [BMS_AdcNetName_TS_VSENSE_P] = ADC1_IN10_TS_VSENSE_P,
    [BMS_AdcNetName_TS_VSENSE_N] = ADC1_IN11_TS_VSENSE_N,
};

static const UART debug_uart = { .handle = &huart1 };

static const BmsShdnConfig bms_shdn_pin_config = {
    .ts_ilck_ok_gpio = ts_ilck_shdn_pin,
    .hvd_ok_gpio     = hvd_ok_shdn_pin,
};

static const BoardShdnNode bms_bshdn_nodes[BmsShdnNodeCount] = {
    { &io_get_TS_ILCK_OK, &app_canTx_BMS_TSIlckOKStatus_set },
    { &io_get_HVD_OK, &app_canTx_BMS_HVDShdnOKStatus_set }
};

void tasks_preInit(void)
{
    // After booting, re-enable interrupts and ensure the core is using the application's vector table.
    hw_bootup_enableInterruptsForApp();
}

void tasks_init(void)
{
    // Configure and initialize SEGGER SystemView.
    // NOTE: Needs to be done after clock config!
    SEGGER_SYSVIEW_Conf();
    LOG_INFO("VC reset!");

    __HAL_DBGMCU_FREEZE_IWDG1();

    HAL_ADCEx_Calibration_Start(&hadc1, ADC_CALIB_OFFSET_LINEARITY, ADC_SINGLE_ENDED);
    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)hw_adc_getRawValuesBuffer(), hadc1.Init.NbrOfConversion);
    HAL_TIM_Base_Start(&htim3);
    HAL_TIM_Base_Start(&htim15);

    hw_hardFaultHandler_init();
    hw_can_init(&can);
    hw_sd_init(&sd);
    hw_crc_init(&hcrc);
    hw_watchdog_init(hw_watchdogConfig_refresh, hw_watchdogConfig_timeoutCallback);

    io_canTx_init(io_jsoncan_pushTxMsgToQueue);
    io_canTx_enableMode(CAN_MODE_DEFAULT, true);
    io_can_init(&can_config);
    io_bmsShdn_init(&bms_shdn_pin_config);
    io_tractiveSystem_init(&ts_config);
    io_thermistors_init(&thermistors_config);
    io_ltc6813Shared_init(&ltc6813_spi);
    io_airs_init(&airs_config);
    io_imd_init(&imd_pwm_input_config);
    io_chimera_init(&debug_uart, GpioNetName_bms_net_name_tag, AdcNetName_bms_net_name_tag, &n_chimera_pin);
    // io_charger_init(&charger_config);
    io_sdGpio_init(&sd_gpio);

    app_canTx_init();
    app_canRx_init();

    HAL_GPIO_WritePin(BSPD_TEST_EN_GPIO_Port, BSPD_TEST_EN_Pin, GPIO_PIN_RESET);

    app_inverterOnState_init();
    app_accumulator_init();

    // Re-enable if auxiliary thermistors installed
    // app_thermistors_init();

    app_soc_init();
    app_globals_init(&globals_config);
    app_stateMachine_init(app_initState_get());

    app_shdn_loop_init(bms_bshdn_nodes, BmsShdnNodeCount);

    // broadcast commit info
    app_canTx_BMS_Hash_set(GIT_COMMIT_HASH);
    app_canTx_BMS_Clean_set(GIT_COMMIT_CLEAN);
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

    static const TickType_t period_ms = 1;
    WatchdogHandle         *watchdog  = hw_watchdog_allocateWatchdog();
    hw_watchdog_initWatchdog(watchdog, RTOS_TASK_1KHZ, period_ms);

    static uint32_t start_ticks = 0;
    start_ticks                 = osKernelGetTickCount();

    for (;;)
    {
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
