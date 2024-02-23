#include "tasks.h"
#include "main.h"
#include "cmsis_os.h"

#include "app_globals.h"
#include "app_heartbeatMonitor.h"
#include "states/app_driveState.h"

#include "app_canTx.h"
#include "app_canRx.h"
#include "app_canAlerts.h"
#include "app_commitInfo.h"

#include "io_jsoncan.h"
#include "io_canRx.h"
#include "io_led.h"

#include "hw_bootup.h"
#include "hw_utils.h"
#include "hw_hardFaultHandler.h"
#include "hw_watchdog.h"
#include "hw_watchdogConfig.h"
#include "hw_stackWaterMark.h"
#include "hw_stackWaterMarkConfig.h"
#include "hw_adc.h"
#include "hw_gpio.h"

#include "FSM.pb.h"
#include <pb_decode.h>
#include <pb_encode.h>

extern ADC_HandleTypeDef  *hadc1;
extern CAN_HandleTypeDef  *hcan1;
extern IWDG_HandleTypeDef *hiwdg;
extern TIM_HandleTypeDef  *htim12;

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

// TODO: Generate proper port/pin with cube
static const Gpio      brake_ocsc_ok_3v3       = { .port = BRAKE_OCSC_OK_3V3_GPIO_Port, .pin = BRAKE_OCSC_OK_3V3_Pin };
static const Gpio      nchimera                = { .port = NCHIMERA_GPIO_Port, .pin = NCHIMERA_Pin };
static const BinaryLed led                     = { .gpio = { .port = LED_GPIO_Port, .pin = LED_Pin } };
static const Gpio      nbspd_brake_pressed_3v3 = { .port = NBSPD_BRAKE_PRESSED_3V3_GPIO_Port,
                                                   .pin  = NBSPD_BRAKE_PRESSED_3V3_Pin };
static const Gpio      nprogram_3v3            = { .port = NPROGRAM_3V3_GPIO_Port, .pin = NPROGRAM_3V3_Pin };
static const Gpio      fsm_shdn                = { .port = FSM_SHDN_GPIO_Port, .pin = FSM_SHDN_Pin };

static const Gpio *id_to_gpio[] = { [FSM_GpioNetName_BRAKE_OCSC_OK_3V3]       = &brake_ocsc_ok_3v3,
                                    [FSM_GpioNetName_NCHIMERA]                = &nchimera,
                                    [FSM_GpioNetName_LED]                     = &led.gpio,
                                    [FSM_GpioNetName_NBSPD_BRAKE_PRESSED_3V3] = &nbspd_brake_pressed_3v3,
                                    [FSM_GpioNetName_NPROGRAM_3V3]            = &nprogram_3v3,
                                    [FSM_GpioNetName_FSM_SHDN]                = &fsm_shdn };

// TODO: Fill in correct channels from cube
static const AdcChannel susp_travel_fr_3v3 = ADC1_CHANNEL_4;
static const AdcChannel susp_travel_fl_3v3 = ADC1_CHANNEL_4;
static const AdcChannel load_cell_2_3v3    = ADC1_CHANNEL_4;
static const AdcChannel apps2_3v3          = ADC1_CHANNEL_4;
static const AdcChannel bps_f_3v3          = ADC1_CHANNEL_4;
static const AdcChannel bps_b_3v3          = ADC1_CHANNEL_4;
static const AdcChannel load_cell_1_3v3    = ADC1_CHANNEL_4;
static const AdcChannel apps1_3v3          = ADC1_CHANNEL_4;
static const AdcChannel steeringangle_3v3  = ADC1_CHANNEL_4;

static const AdcChannel *id_to_adc[] = {
    [FSM_AdcNetName_SUSP_TRAVEL_FR_3V3] = &susp_travel_fr_3v3,
    [FSM_AdcNetName_SUSP_TRAVEL_FL_3V3] = &susp_travel_fl_3v3,
    [FSM_AdcNetName_LOAD_CELL_2_3V3]    = &load_cell_2_3v3,
    [FSM_AdcNetName_APPS2_3V3]          = &apps2_3v3,
    [FSM_AdcNetName_BPS_F_3V3]          = &bps_f_3v3,
    [FSM_AdcNetName_BPS_B_3V3]          = &bps_b_3v3,
    [FSM_AdcNetName_LOAD_CELL_1_3V3]    = &load_cell_1_3v3,
    [FSM_AdcNetName_APPS1_3V3]          = &apps1_3v3,
    [FSM_AdcNetName_SteeringAngle_3V3]  = &steeringangle_3v3,
};

// TODO: Set up UART with cube
#define MAX_DEBUG_BUF_SIZE 100
#define DEBUG_SIZE_MSG_BUF_SIZE 1
static UART    debug_uart = { .handle = &huart7 };
static uint8_t data[MAX_DEBUG_BUF_SIZE];
static bool    is_mid_debug_msg = false;
static uint8_t packet_size;

void tasks_preInit(void) {}

void tasks_init(void)
{
    __HAL_DBGMCU_FREEZE_IWDG();

    // Configure and initialize SEGGER SystemView.
    SEGGER_SYSVIEW_Conf();

    hw_hardFaultHandler_init();
    hw_can_init(hcan1);
    hw_watchdog_init(hw_watchdogConfig_refresh, hw_watchdogConfig_timeoutCallback);

    io_canTx_init(io_jsoncan_pushTxMsgToQueue);
    io_canTx_enableMode(CAN_MODE_DEFAULT, true);
    io_can_init(&can_config);

    app_canTx_init();
    app_canRx_init();

    // TODO: add heartbeat monitor
    app_stateMachine_init(app_driveState_get());
    // TODO: add globals

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
            FSM_DebugMessage msg       = FSM_DebugMessage_init_zero;
            pb_istream_t     in_stream = pb_istream_from_buffer(data, packet_size);
            bool             status    = pb_decode(&in_stream, FSM_DebugMessage_fields, &msg);

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
            status                  = pb_encode(&out_stream, FSM_DebugMessage_fields, &msg);
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
