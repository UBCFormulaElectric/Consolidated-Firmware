#include "tasks.h"
#include "main.h"
#include "cmsis_os.h"

#include "hw_can.h"
#include "hw_adc.h"
#include "hw_gpio.h"
#include "hw_hardFaultHandler.h"
// #include "hw_bootup.h"
#include "hw_utils.h"
#include "hw_spi.h"
#include "hw_pwmInput.h"
#include "hw_stackWaterMarkConfig.h"
#include "hw_watchdogConfig.h"
#include "hw_watchdog.h"

#include "io_canTx.h"
#include "io_canRx.h"
#include "io_jsoncan.h"
#include "io_led.h"
#include "io_time.h"
#include "io_can.h"
#include "io_airs.h"
#include "io_charger.h"
#include "io_faultLatch.h"
#include "io_imd.h"
#include "ltc6813/io_ltc6813Shared.h"
#include "io_thermistors.h"
#include "io_tractiveSystem.h"
#include "io_log.h"

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

#include "BMS.pb.h"
#include <pb_decode.h>
#include <pb_encode.h>

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

extern ADC_HandleTypeDef   hadc1;
extern FDCAN_HandleTypeDef hfdcan1;
extern IWDG_HandleTypeDef  hiwdg;
extern SPI_HandleTypeDef   hspi2;
extern TIM_HandleTypeDef   htim1;
extern TIM_HandleTypeDef   htim15;

static const CanConfig can_config = {
    .rx_msg_filter        = io_canRx_filterMessageId,
    .tx_overflow_callback = canTxQueueOverflowCallBack,
    .rx_overflow_callback = canRxQueueOverflowCallBack,
};

// TODO: Generate proper port/pin with cube
static const Gpio      accel_brake_ok_3v3     = { .port = ACCEL_BRAKE_OK_3V3_GPIO_Port, .pin = ACCEL_BRAKE_OK_3V3_Pin };
static const Gpio      airplus_en             = { .port = AIRPLUS_EN_GPIO_Port, .pin = AIRPLUS_EN_Pin };
static const Gpio      aux_tsense_mux0        = { .port = AUX_TSENSE_MUX0_GPIO_Port, .pin = AUX_TSENSE_MUX0_Pin };
static const Gpio      aux_tsense_mux1        = { .port = AUX_TSENSE_MUX1_GPIO_Port, .pin = AUX_TSENSE_MUX1_Pin };
static const Gpio      aux_tsense_mux2        = { .port = AUX_TSENSE_MUX2_GPIO_Port, .pin = AUX_TSENSE_MUX2_Pin };
static const Gpio      aux_tsense_mux3        = { .port = AUX_TSENSE_MUX3_GPIO_Port, .pin = AUX_TSENSE_MUX3_Pin };
static const Gpio      bms_latch              = { .port = BMS_LATCH_GPIO_Port, .pin = BMS_LATCH_Pin };
static const Gpio      bms_ok_3v3             = { .port = BMS_OK_3V3_GPIO_Port, .pin = BMS_OK_3V3_Pin };
static const Gpio      bspd_latch             = { .port = BSPD_LATCH_GPIO_Port, .pin = BSPD_LATCH_Pin };
static const Gpio      bspd_ok_3v3            = { .port = BSPD_OK_3V3_GPIO_Port, .pin = BSPD_OK_3V3_Pin };
static const Gpio      bspd_test_en           = { .port = BSPD_TEST_EN_GPIO_Port, .pin = BSPD_TEST_EN_Pin };
static const Gpio      hvd_shdn_ok            = { .port = HVD_SHDN_OK_GPIO_Port, .pin = HVD_SHDN_OK_Pin };
static const Gpio      imd_latch              = { .port = IMD_LATCH_GPIO_Port, .pin = IMD_LATCH_Pin };
static const Gpio      imd_ok_3v3             = { .port = IMD_OK_3V3_GPIO_Port, .pin = IMD_OK_3V3_Pin };
static const BinaryLed led                    = { .gpio = { .port = LED_GPIO_Port, .pin = LED_Pin } };
static const Gpio      nchimera               = { .port = NCHIMERA_GPIO_Port, .pin = NCHIMERA_Pin };
static const Gpio      nhigh_current_bspd_3v3 = { .port = NHIGH_CURRENT_BSPD_3V3_GPIO_Port,
                                                  .pin  = NHIGH_CURRENT_BSPD_3V3_Pin };
static const Gpio      nprogram_3v3           = { .port = NPROGRAM_3V3_GPIO_Port, .pin = NPROGRAM_3V3_Pin };
static const Gpio      pre_charge_en          = { .port = PRE_CHARGE_EN_GPIO_Port, .pin = PRE_CHARGE_EN_Pin };
static const Gpio      ts_ilck_shdn_ok        = { .port = TS_ILCK_SHDN_OK_GPIO_Port, .pin = TS_ILCK_SHDN_OK_Pin };
static const Gpio ts_isense_ocsc_ok_3v3 = { .port = TS_ISENSE_OCSC_OK_3V3_GPIO_Port, .pin = TS_ISENSE_OCSC_OK_3V3_Pin };
static const Gpio sd_cd                 = { .port = SD_CD_GPIO_Port, .pin = SD_CD_Pin };
static const Gpio spi_cs                = { .port = SPI_CS_GPIO_Port, .pin = SPI_CS_Pin };

static const Gpio *id_to_gpio[] = { [BMS_GpioNetName_ACCEL_BRAKE_OK_3V3]     = &accel_brake_ok_3v3,
                                    [BMS_GpioNetName_AIRPLUS_EN]             = &airplus_en,
                                    [BMS_GpioNetName_AUX_TSENSE_MUX0]        = &aux_tsense_mux0,
                                    [BMS_GpioNetName_AUX_TSENSE_MUX1]        = &aux_tsense_mux1,
                                    [BMS_GpioNetName_AUX_TSENSE_MUX2]        = &aux_tsense_mux2,
                                    [BMS_GpioNetName_AUX_TSENSE_MUX3]        = &aux_tsense_mux3,
                                    [BMS_GpioNetName_BMS_LATCH]              = &bms_latch,
                                    [BMS_GpioNetName_BMS_OK_3V3]             = &bms_ok_3v3,
                                    [BMS_GpioNetName_BSPD_LATCH]             = &bspd_latch,
                                    [BMS_GpioNetName_BSPD_OK_3V3]            = &bspd_ok_3v3,
                                    [BMS_GpioNetName_BSPD_TEST_EN]           = &bspd_test_en,
                                    [BMS_GpioNetName_HVD_SHDN_OK]            = &hvd_shdn_ok,
                                    [BMS_GpioNetName_IMD_LATCH]              = &imd_latch,
                                    [BMS_GpioNetName_IMD_OK_3V3]             = &imd_ok_3v3,
                                    [BMS_GpioNetName_LED]                    = &led.gpio,
                                    [BMS_GpioNetName_NCHIMERA]               = &nchimera,
                                    [BMS_GpioNetName_NHIGH_CURRENT_BSPD_3V3] = &nhigh_current_bspd_3v3,
                                    [BMS_GpioNetName_NPROGRAM_3V3]           = &nprogram_3v3,
                                    [BMS_GpioNetName_PRE_CHARGE_EN]          = &pre_charge_en,
                                    [BMS_GpioNetName_TS_ILCK_SHDN_OK]        = &ts_ilck_shdn_ok,
                                    [BMS_GpioNetName_TS_ISENSE_OCSC_OK_3V3]  = &ts_isense_ocsc_ok_3v3,
                                    [BMS_GpioNetName_SD_CD]                  = &sd_cd,
                                    [BMS_GpioNetName_SPI_CS]                 = &spi_cs };

// TODO: Fill in correct channels from cube
static const AdcChannel aux_tsense     = ADC1_CHANNEL_4;
static const AdcChannel ts_isense_400a = ADC1_CHANNEL_4;
static const AdcChannel ts_isense_50a  = ADC1_CHANNEL_4;
static const AdcChannel ts_vsense_n    = ADC1_CHANNEL_4;
static const AdcChannel ts_vsense_p    = ADC1_CHANNEL_4;

static const AdcChannel *id_to_adc[] = {
    [BMS_AdcNetName_AUX_TSENSE] = &aux_tsense,       [BMS_AdcNetName_TS_ISENSE_400A] = &ts_isense_400a,
    [BMS_AdcNetName_TS_ISENSE_50A] = &ts_isense_50a, [BMS_AdcNetName_TS_VSENSE_N] = &ts_vsense_n,
    [BMS_AdcNetName_TS_VSENSE_P] = &ts_vsense_p,
};

// TODO: Set up UART with cube
#define MAX_DEBUG_BUF_SIZE 100
#define DEBUG_SIZE_MSG_BUF_SIZE 1
static UART    debug_uart = { .handle = &huart7 };
static uint8_t data[MAX_DEBUG_BUF_SIZE];
static bool    is_mid_debug_msg = false;
static uint8_t packet_size;

PwmInputConfig imd_pwm_input_config = {
    .htim                     = &htim1,
    .timer_frequency_hz       = TIM1_FREQUENCY / TIM1_PRESCALER,
    .rising_edge_tim_channel  = TIM_CHANNEL_2,
    .falling_edge_tim_channel = TIM_CHANNEL_1,
};

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
                                   .thermistor_adc_channel = ADC1_CHANNEL_4
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
                                       .port = PRECHARGE_EN_GPIO_Port,
                                       .pin  = PRECHARGE_EN_Pin,
                                   }
};

static const TractiveSystemConfig ts_config = { .ts_vsense_p_channel        = ADC1_CHANNEL_10,
                                                .ts_vsense_n_channel        = ADC1_CHANNEL_11,
                                                .ts_isense_high_res_channel = ADC1_CHANNEL_5,
                                                .ts_isense_low_res_channel  = ADC1_CHANNEL_9

};

static const FaultLatch bms_ok_latch = {
    .current_status_gpio = {
        .port = BMS_OK_GPIO_Port,
        .pin = BMS_OK_Pin,
    },
    .latch_status_gpio = {
        .port = BMS_LATCH_GPIO_Port,
        .pin = BMS_LATCH_Pin,
    },
    .read_only = false,
};

static const FaultLatch imd_ok_latch = {
    .current_status_gpio = {
        .port = IMD_OK_GPIO_Port,
        .pin = IMD_OK_Pin,
    },
    .latch_status_gpio = {
        .port = IMD_LATCH_GPIO_Port,
        .pin = IMD_LATCH_Pin,
    },
    .read_only = true,
};

static const FaultLatch bspd_ok_latch = {
    .current_status_gpio = {
        .port = BSPD_OK_GPIO_Port,
        .pin = BSPD_OK_Pin,
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
                                                      [DCM_HEARTBEAT_BOARD] = NULL,
                                                      [PDM_HEARTBEAT_BOARD] = NULL,
                                                      [FSM_HEARTBEAT_BOARD] = &app_canRx_FSM_Heartbeat_get,
                                                      [DIM_HEARTBEAT_BOARD] = NULL };

// heartbeatUpdaters - update local CAN table with heartbeat status
void (*heartbeatUpdaters[HEARTBEAT_BOARD_COUNT])(bool) = { [BMS_HEARTBEAT_BOARD] = NULL,
                                                           [DCM_HEARTBEAT_BOARD] = NULL,
                                                           [PDM_HEARTBEAT_BOARD] = NULL,
                                                           [FSM_HEARTBEAT_BOARD] = &app_canRx_FSM_Heartbeat_update,
                                                           [DIM_HEARTBEAT_BOARD] = NULL };

// heartbeatFaultSetters - broadcast heartbeat faults over CAN
void (*heartbeatFaultSetters[HEARTBEAT_BOARD_COUNT])(bool) = {
    [BMS_HEARTBEAT_BOARD] = NULL,
    [DCM_HEARTBEAT_BOARD] = NULL,
    [PDM_HEARTBEAT_BOARD] = NULL,
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
    // hw_bootup_enableInterruptsForApp();
}

void tasks_init(void)
{
    __HAL_DBGMCU_FREEZE_IWDG1();

    // Configure and initialize SEGGER SystemView.
    SEGGER_SYSVIEW_Conf();
    LOG_INFO("BMS reset!");

    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)hw_adc_getRawValuesBuffer(), hadc1.Init.NbrOfConversion);
    HAL_TIM_Base_Start(&htim15);

    hw_hardFaultHandler_init();
    hw_can_init(&hfdcan1);
    hw_watchdog_init(hw_watchdogConfig_refresh, hw_watchdogConfig_timeoutCallback);

    io_canTx_init(io_jsoncan_pushTxMsgToQueue);
    io_canTx_enableMode(CAN_MODE_DEFAULT, true);
    io_can_init(&can_config);

    io_tractiveSystem_init(&ts_config);
    io_thermistors_init(&thermistors_config);
    io_ltc6813Shared_init(&ltc6813_spi);
    io_airs_init(&airs_config);
    io_imd_init(&imd_pwm_input_config);
    // io_charger_init(&charger_config);

    app_canTx_init();
    app_canRx_init();

    app_inverterOnState_init();
    app_accumulator_init();
    app_thermistors_init();
    app_soc_init();
    app_globals_init(&globals_config);
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

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart == debug_uart.handle)
    {
        // This interrupt is fired when DEBUG_BUF_SIZE bytes are received via UART.
        // NOTE: If you send more or less data in a UART transaction, seems like the
        // peripheral can get confused...

        if (is_mid_debug_msg)
        {
            BMS_DebugMessage msg       = BMS_DebugMessage_init_zero;
            pb_istream_t     in_stream = pb_istream_from_buffer(data, packet_size);
            bool             status    = pb_decode(&in_stream, BMS_DebugMessage_fields, &msg);

            if (!status)
            {
                printf("Decoding failed: %s\n", PB_GET_ERROR(&in_stream));
            }

            switch (msg.which_payload)
            {
                case 1:
                {
                    const Gpio *gpio            = id_to_gpio[msg.payload.gpio_read.net_name];
                    msg.payload.gpio_read.value = hw_gpio_readPin(gpio) + 1; // add one for enum scale offset
                    break;
                }
                case 2:
                {
                    const Gpio *gpio = id_to_gpio[msg.payload.gpio_write.net_name];
                    hw_gpio_writePin(gpio, msg.payload.gpio_write.value - 1); // add one for enum scale offset
                    break;
                }
                case 3:
                {
                    const AdcChannel *adc = id_to_adc[msg.payload.adc.net_name];
                    msg.payload.adc.value = hw_adc_getVoltage(*adc);
                    break;
                }
                default:
                    break;
            }

            pb_ostream_t out_stream = pb_ostream_from_buffer(data, sizeof(data));
            status                  = pb_encode(&out_stream, BMS_DebugMessage_fields, &msg);
            packet_size             = (u_int8_t)out_stream.bytes_written;

            if (!status)
            {
                printf("Encoding failed: %s\n", PB_GET_ERROR(&out_stream));
            }

            uint8_t size_data[DEBUG_SIZE_MSG_BUF_SIZE];
            size_data[0] = packet_size;
            hw_uart_transmitPoll(&debug_uart, size_data, DEBUG_SIZE_MSG_BUF_SIZE, osWaitForever);

            hw_uart_transmitPoll(&debug_uart, data, packet_size, osWaitForever);
            is_mid_debug_msg = false;
            packet_size      = DEBUG_SIZE_MSG_BUF_SIZE;
        }
        else
        {
            is_mid_debug_msg = true;
            packet_size      = data[0];
        }

        // Start receiving data in interrupt mode again so this interrupt will get fired if
        // more data is recieved.
        hw_uart_receiveIt(&debug_uart, data, packet_size);
    }
}
