#include "tasks.h"
#include "main.h"
#include "cmsis_os.h"
#include "string.h"

#include "app_globals.h"
#include "app_heartbeatMonitor.h"
#include "states/app_initState.h"
#include "app_canTx.h"
#include "app_canRx.h"
#include "app_canAlerts.h"
#include "app_commitInfo.h"

#include "io_jsoncan.h"
#include "io_log.h"
#include "io_canRx.h"
#include "io_led.h"

#include "hw_bootup.h"
#include "hw_utils.h"
#include "hw_hardFaultHandler.h"
#include "hw_watchdog.h"
#include "hw_watchdogConfig.h"
#include "hw_gpio.h"
#include "hw_stackWaterMark.h"
#include "hw_stackWaterMarkConfig.h"
#include "hw_uart.h"
#include "hw_adc.h"

#include "VC.pb.h"
#include <pb_decode.h>
#include <pb_encode.h>

extern ADC_HandleTypeDef   hadc1;
extern ADC_HandleTypeDef   hadc3;
extern FDCAN_HandleTypeDef hfdcan1;
extern UART_HandleTypeDef  huart7;
// extern IWDG_HandleTypeDef  hiwdg1;

void canRxQueueOverflowCallBack(uint32_t overflow_count)
{
    app_canTx_VC_RxOverflowCount_set(overflow_count);
    app_canAlerts_VC_Warning_RxOverflow_set(true);
}

void canTxQueueOverflowCallBack(uint32_t overflow_count)
{
    app_canTx_VC_TxOverflowCount_set(overflow_count);
    app_canAlerts_VC_Warning_TxOverflow_set(true);
}

static const CanConfig can_config = {
    .rx_msg_filter        = io_canRx_filterMessageId,
    .tx_overflow_callback = canTxQueueOverflowCallBack,
    .rx_overflow_callback = canRxQueueOverflowCallBack,
};

static const Gpio      buzzer_pwr_en    = { .port = BUZZER_PWR_EN_GPIO_Port, .pin = BUZZER_PWR_EN_Pin };
static const Gpio      bat_i_sns_nflt   = { .port = BAT_I_SNS_NFLT_GPIO_Port, .pin = BAT_I_SNS_NFLT_Pin };
static const BinaryLed led              = { .gpio = { .port = LED_GPIO_Port, .pin = LED_Pin } };
static const Gpio      telem_pwr_en     = { .port = TELEM_PWR_EN_GPIO_Port, .pin = TELEM_PWR_EN_Pin };
static const Gpio      npcm_en          = { .port = NPCM_EN_GPIO_Port, .pin = NPCM_EN_Pin };
static const Gpio      acc_i_sense_nflt = { .port = ACC_I_SENSE_NFLT_GPIO_Port, .pin = ACC_I_SENSE_NFLT_Pin };
static const Gpio      pgood            = { .port = PGOOD_GPIO_Port, .pin = PGOOD_Pin };
static const Gpio      lv_pwr_en        = { .port = LV_PWR_EN_GPIO_Port, .pin = LV_PWR_EN_Pin };
static const Gpio      aux_pwr_en       = { .port = AUX_PWR_EN_GPIO_Port, .pin = AUX_PWR_EN_Pin };
static const Gpio      pump_pwr_en      = { .port = PUMP_PWR_EN_GPIO_Port, .pin = PUMP_PWR_EN_Pin };
static const Gpio      nchrg_fault      = { .port = NCHRG_FAULT_GPIO_Port, .pin = NCHRG_FAULT_Pin };
static const Gpio      nchrg            = { .port = NCHRG_GPIO_Port, .pin = NCHRG_Pin };
static const Gpio      inv_l_pwr_en     = { .port = INV_L_PWR_EN_GPIO_Port, .pin = INV_L_PWR_EN_Pin };
static const Gpio      inv_r_pwr_en     = { .port = INV_R_PWR_EN_GPIO_Port, .pin = INV_R_PWR_EN_Pin };
static const Gpio      fr_stby1         = { .port = FR_STBY1_GPIO_Port, .pin = FR_STBY1_Pin };
static const Gpio      fr_stby2         = { .port = FR_STBY2_GPIO_Port, .pin = FR_STBY2_Pin };
static const Gpio      fr_stby3         = { .port = FR_STBY3_GPIO_Port, .pin = FR_STBY3_Pin };
static const Gpio      inv_l_program    = { .port = INV_L_PROGRAM_GPIO_Port, .pin = INV_L_PROGRAM_Pin };
static const Gpio      inv_r_program    = { .port = INV_R_PROGRAM_GPIO_Port, .pin = INV_R_PROGRAM_Pin };
static const Gpio      l_shdn_sns       = { .port = L_SHDN_SNS_GPIO_Port, .pin = L_SHDN_SNS_Pin };
static const Gpio      r_shdn_sns       = { .port = R_SHDN_SNS_GPIO_Port, .pin = R_SHDN_SNS_Pin };
static const Gpio      nchimera         = { .port = NCHIMERA_GPIO_Port, .pin = NCHIMERA_Pin };
static const Gpio      nprogram_3v3     = { .port = NPROGRAM_3V3_GPIO_Port, .pin = NPROGRAM_3V3_Pin };
static const Gpio      sb_ilck_shdn_sns = { .port = SB_ILCK_SHDN_SNS_GPIO_Port, .pin = SB_ILCK_SHDN_SNS_Pin };
static const Gpio      tsms_shdn_sns    = { .port = TSMS_SHDN_SNS_GPIO_Port, .pin = TSMS_SHDN_SNS_Pin };

static const Gpio *id_to_gpio[] = { [GpioNetName_BUZZER_PWR_EN]    = &buzzer_pwr_en,
                                    [GpioNetName_BAT_I_SNS_NFLT]   = &bat_i_sns_nflt,
                                    [GpioNetName_LED]              = &led.gpio,
                                    [GpioNetName_TELEM_PWR_EN]     = &telem_pwr_en,
                                    [GpioNetName_NPCM_EN]          = &npcm_en,
                                    [GpioNetName_ACC_I_SENSE_NFLT] = &acc_i_sense_nflt,
                                    [GpioNetName_PGOOD]            = &pgood,
                                    [GpioNetName_LV_PWR_EN]        = &lv_pwr_en,
                                    [GpioNetName_AUX_PWR_EN]       = &aux_pwr_en,
                                    [GpioNetName_PUMP_PWR_EN]      = &pump_pwr_en,
                                    [GpioNetName_NCHRG_FAULT]      = &nchrg_fault,
                                    [GpioNetName_NCHRG]            = &nchrg,
                                    [GpioNetName_INV_L_PWR_EN]     = &inv_l_pwr_en,
                                    [GpioNetName_INV_R_PWR_EN]     = &inv_r_pwr_en,
                                    [GpioNetName_FR_STBY1]         = &fr_stby1,
                                    [GpioNetName_FR_STBY2]         = &fr_stby2,
                                    [GpioNetName_FR_STBY3]         = &fr_stby3,
                                    [GpioNetName_INV_L_PROGRAM]    = &inv_l_program,
                                    [GpioNetName_INV_R_PROGRAM]    = &inv_r_program,
                                    [GpioNetName_L_SHDN_SNS]       = &l_shdn_sns,
                                    [GpioNetName_R_SHDN_SNS]       = &r_shdn_sns,
                                    [GpioNetName_NCHIMERA]         = &nchimera,
                                    [GpioNetName_NPROGRAM_3V3]     = &nprogram_3v3,
                                    [GpioNetName_SB_ILCK_SHDN_SNS] = &sb_ilck_shdn_sns,
                                    [GpioNetName_TSMS_SHDN_SNS]    = &tsms_shdn_sns };

static const AdcChannel inv_r_pwr_i_sns  = ADC1_CHANNEL_10;
static const AdcChannel inv_l_pwr_i_sns  = ADC1_CHANNEL_11;
static const AdcChannel aux_pwr_i_sns    = ADC3_CHANNEL_0;
static const AdcChannel bat_i_sns        = ADC1_CHANNEL_14;
static const AdcChannel shdn_pwr_i_sns   = ADC1_CHANNEL_18;
static const AdcChannel vbat_sense       = ADC1_CHANNEL_19;
static const AdcChannel _24v_acc_sense   = ADC1_CHANNEL_3;
static const AdcChannel _22v_boost_sense = ADC1_CHANNEL_7;
static const AdcChannel lv_pwr_i_sns     = ADC1_CHANNEL_4;
static const AdcChannel acc_i_sense      = ADC1_CHANNEL_5;
static const AdcChannel pump_pwr_i_sns   = ADC3_CHANNEL_1;

static const AdcChannel *id_to_adc[] = {
    [AdcNetName_INV_R_PWR_I_SNS]  = &inv_r_pwr_i_sns,
    [AdcNetName_INV_L_PWR_I_SNS]  = &inv_l_pwr_i_sns,
    [AdcNetName_AUX_PWR_I_SNS]    = &aux_pwr_i_sns,
    [AdcNetName_SHDN_PWR_I_SNS]   = &shdn_pwr_i_sns,
    [AdcNetName_VBAT_SENSE]       = &vbat_sense,
    [AdcNetName__24V_ACC_SENSE]   = &_24v_acc_sense,
    [AdcNetName__22V_BOOST_SENSE] = &_22v_boost_sense,
    [AdcNetName_LV_PWR_I_SNS]     = &lv_pwr_i_sns,
    [AdcNetName_ACC_I_SENSE]      = &acc_i_sense,
    [AdcNetName_PUMP_PWR_I_SNS]   = &pump_pwr_i_sns,
};

#define MAX_DEBUG_BUF_SIZE 100
#define DEBUG_SIZE_MSG_BUF_SIZE 1
static UART    debug_uart = { .handle = &huart7 };
static uint8_t data[MAX_DEBUG_BUF_SIZE];
static bool    is_mid_debug_msg = false;
static uint8_t packet_size;

void tasks_preInit(void) {}

void tasks_init(void)
{
    __HAL_DBGMCU_FREEZE_IWDG1();

    // Configure and initialize SEGGER SystemView.
    SEGGER_SYSVIEW_Conf();
    LOG_INFO("VC reset!");

    // efuses:
    // HAL_ADC_Start_DMA(
    //     hw_tasks_config->hadc1, (uint32_t *)hw_adc_getRawValuesBuffer(),
    //     hw_tasks_config->hadc1->Init.NbrOfConversion);

    hw_hardFaultHandler_init();
    hw_can_init(&hfdcan1);

    // TODO: Re-enable watchdog (disabled because it can get annoying when bringing up a board).
    hw_watchdog_init(hw_watchdogConfig_refresh, hw_watchdogConfig_timeoutCallback);

    io_canTx_init(io_jsoncan_pushTxMsgToQueue);
    io_canTx_enableMode(CAN_MODE_DEFAULT, true);
    io_can_init(&can_config);

    app_canTx_init();
    app_canRx_init();
    app_stateMachine_init(app_initState_get());

    app_canTx_VC_Hash_set(GIT_COMMIT_HASH);
    app_canTx_VC_Clean_set(GIT_COMMIT_CLEAN);

    // Receive data over the debug UART line in interrupt mode.
    hw_uart_receiveIt(&debug_uart, data, DEBUG_SIZE_MSG_BUF_SIZE);
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

        // TODO: setup can debug
        // const bool debug_mode_enabled = app_canRx_Debug_EnableDebugMode_get();
        // io_canTx_enableMode(CAN_MODE_DEBUG, debug_mode_enabled);
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

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart == debug_uart.handle)
    {
        // This interrupt is fired when DEBUG_BUF_SIZE bytes are received via UART.
        // NOTE: If you send more or less data in a UART transaction, seems like the
        // peripheral can get confused...

        if (is_mid_debug_msg)
        {
            DebugMessage msg       = DebugMessage_init_zero;
            pb_istream_t in_stream = pb_istream_from_buffer(data, packet_size);
            bool         status    = pb_decode(&in_stream, DebugMessage_fields, &msg);

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
            status                  = pb_encode(&out_stream, DebugMessage_fields, &msg);
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
