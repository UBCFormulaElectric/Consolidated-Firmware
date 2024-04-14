#include "tasks.h"
#include "main.h"
#include "cmsis_os.h"

#include "app_heartbeatMonitor.h"
#include "app_mainState.h"

#include "app_canTx.h"
#include "app_canRx.h"
#include "app_canAlerts.h"
#include "app_commitInfo.h"
#include "app_globals.h"

#include "io_jsoncan.h"
#include "io_canRx.h"
#include "io_log.h"
#include "io_chimera.h"
#include "io_jsoncan.h"
#include "io_coolant.h"

#include "hw_bootup.h"
#include "hw_utils.h"
#include "hw_hardFaultHandler.h"
#include "hw_watchdog.h"
#include "hw_stackWaterMark.h"
#include "hw_stackWaterMarkConfig.h"
#include "hw_adc.h"
#include "hw_gpio.h"
#include "hw_uart.h"

#include "shared.pb.h"
#include "RSM.pb.h"

extern ADC_HandleTypeDef  hadc1;
extern TIM_HandleTypeDef  htim3;
extern CAN_HandleTypeDef  hcan1;
extern UART_HandleTypeDef huart1;

const CanHandle can = { .can = &hcan1, .can_msg_received_callback = io_can_msgReceivedCallback };

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

void canTxQueueOverflowClearCallback()
{
    app_canAlerts_RSM_Warning_TxOverflow_set(false);
}

void canRxQueueOverflowClearCallback()
{
    app_canAlerts_RSM_Warning_RxOverflow_set(false);
}

const CanConfig can_config = {
    .rx_msg_filter              = io_canRx_filterMessageId,
    .tx_overflow_callback       = canTxQueueOverflowCallBack,
    .rx_overflow_callback       = canRxQueueOverflowCallBack,
    .tx_overflow_clear_callback = canTxQueueOverflowClearCallback,
    .rx_overflow_clear_callback = canRxQueueOverflowClearCallback,
};

static const Gpio n_chimera_pin      = { .port = NCHIMERA_GPIO_Port, .pin = NCHIMERA_Pin };
static const Gpio led_pin            = { .port = LED_GPIO_Port, .pin = LED_Pin };
static const Gpio rad_fan_en_pin     = { .port = RAD_FAN_EN_GPIO_Port, .pin = RAD_FAN_EN_Pin };
static const Gpio fr_stby_pin        = { .port = FR_STBY_GPIO_Port, .pin = FR_STBY_Pin };
static const Gpio brake_light_en_pin = { .port = BRAKE_LIGHT_EN_3V3_GPIO_Port, .pin = BRAKE_LIGHT_EN_3V3_Pin };
static const Gpio acc_fan_en_pin     = { .port = ACC_FAN_EN_GPIO_Port, .pin = ACC_FAN_EN_Pin };
static const Gpio n_program_pin      = { .port = NProgram_3V3_GPIO_Port, .pin = NProgram_3V3_Pin };

const Gpio *id_to_gpio[] = { [RSM_GpioNetName_NCHIMERA]           = &n_chimera_pin,
                             [RSM_GpioNetName_LED]                = &led_pin,
                             [RSM_GpioNetName_RAD_FAN_EN]         = &rad_fan_en_pin,
                             [RSM_GpioNetName_FR_STBY]            = &fr_stby_pin,
                             [RSM_GpioNetName_BRAKE_LIGHT_EN_3V3] = &brake_light_en_pin,
                             [RSM_GpioNetName_ACC_FAN_EN]         = &acc_fan_en_pin,
                             [RSM_GpioNetName_NProgram_3V3]       = &n_program_pin };

static const BinaryLed brake_light = { .gpio = {
                                           .port = BRAKE_LIGHT_EN_3V3_GPIO_Port,
                                           .pin  = BRAKE_LIGHT_EN_3V3_Pin,
                                       } };

AdcChannel id_to_adc[] = {
    [RSM_AdcNetName_ACC_FAN_I_SNS]        = ADC1_IN15_ACC_FAN_I_SNS,
    [RSM_AdcNetName_RAD_FAN_I_SNS]        = ADC1_IN14_RAD_FAN_I_SNS,
    [RSM_AdcNetName_CoolantPressure1_3V3] = ADC1_IN12_COOLANT_PRESSURE_1,
    [RSM_AdcNetName_CoolantPressure2_3V3] = ADC1_IN11_COOLANT_PRESSURE_2,
    [RSM_AdcNetName_LC3_OUT]              = ADC1_IN10_LC3_OUT,
    [RSM_AdcNetName_SUSP_TRAVEL_RR_3V3]   = ADC1_IN4_SUSP_TRAVEL_RR,
    [RSM_AdcNetName_SUSP_TRAVEL_RL_3V3]   = ADC1_IN1_SUSP_TRAVEL_RL,
    [RSM_AdcNetName_CoolantTemp2_3V3]     = ADC1_IN3_COOLANT_TEMP_2,
    [RSM_AdcNetName_CoolantTemp1_3V3]     = ADC1_IN2_COOLANT_TEMP_1,
    [RSM_AdcNetName_LC4_OUT]              = ADC1_IN0_LC4_OUT,
};

static const GlobalsConfig config = { .brake_light = &brake_light,
                                      .acc_fan     = &acc_fan_en_pin,
                                      .rad_fan     = &rad_fan_en_pin };

static UART debug_uart = { .handle = &huart1 };

// config for heartbeat monitor
/// RSM rellies on BMS and FSM
bool heartbeatMonitorChecklist[HEARTBEAT_BOARD_COUNT] = {
    [BMS_HEARTBEAT_BOARD] = true, [VC_HEARTBEAT_BOARD] = false,  [RSM_HEARTBEAT_BOARD] = false,
    [FSM_HEARTBEAT_BOARD] = true, [DIM_HEARTBEAT_BOARD] = false, [CRIT_HEARTBEAT_BOARD] = false
};

// heartbeatGetters - get heartbeat signals from other boards
bool (*heartbeatGetters[HEARTBEAT_BOARD_COUNT])() = {
    [BMS_HEARTBEAT_BOARD] = app_canRx_BMS_Heartbeat_get, [VC_HEARTBEAT_BOARD] = NULL,  [RSM_HEARTBEAT_BOARD] = NULL,
    [FSM_HEARTBEAT_BOARD] = app_canRx_FSM_Heartbeat_get, [DIM_HEARTBEAT_BOARD] = NULL, [CRIT_HEARTBEAT_BOARD] = NULL
};

// heartbeatUpdaters - update local CAN table with heartbeat status
void (*heartbeatUpdaters[HEARTBEAT_BOARD_COUNT])(bool) = {
    [BMS_HEARTBEAT_BOARD] = app_canRx_BMS_Heartbeat_update, [VC_HEARTBEAT_BOARD] = NULL,  [RSM_HEARTBEAT_BOARD] = NULL,
    [FSM_HEARTBEAT_BOARD] = app_canRx_FSM_Heartbeat_update, [DIM_HEARTBEAT_BOARD] = NULL, [CRIT_HEARTBEAT_BOARD] = NULL
};

// heartbeatFaultSetters - broadcast heartbeat faults over CAN
void (*heartbeatFaultSetters[HEARTBEAT_BOARD_COUNT])(bool) = {
    [BMS_HEARTBEAT_BOARD]  = app_canAlerts_RSM_Fault_MissingBMSHeartbeat_set,
    [VC_HEARTBEAT_BOARD]   = NULL,
    [RSM_HEARTBEAT_BOARD]  = NULL,
    [FSM_HEARTBEAT_BOARD]  = app_canAlerts_RSM_Fault_MissingFSMHeartbeat_set,
    [DIM_HEARTBEAT_BOARD]  = NULL,
    [CRIT_HEARTBEAT_BOARD] = NULL
};

// heartbeatFaultGetters - gets fault statuses over CAN
bool (*heartbeatFaultGetters[HEARTBEAT_BOARD_COUNT])() = {
    [BMS_HEARTBEAT_BOARD]  = app_canAlerts_RSM_Fault_MissingBMSHeartbeat_get,
    [VC_HEARTBEAT_BOARD]   = NULL,
    [RSM_HEARTBEAT_BOARD]  = NULL,
    [FSM_HEARTBEAT_BOARD]  = app_canAlerts_RSM_Fault_MissingFSMHeartbeat_get,
    [DIM_HEARTBEAT_BOARD]  = NULL,
    [CRIT_HEARTBEAT_BOARD] = NULL
};

void tasks_preInit(void)
{
    hw_bootup_enableInterruptsForApp();

    // Configure and initialize SEGGER SystemView.
    SEGGER_SYSVIEW_Conf();
    LOG_INFO("RSM reset!");
}

void tasks_init(void)
{
    __HAL_DBGMCU_FREEZE_IWDG();

    // Start DMA/TIM3 for the ADC.
    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)hw_adc_getRawValuesBuffer(), hadc1.Init.NbrOfConversion);
    HAL_TIM_Base_Start(&htim3);

    hw_hardFaultHandler_init();
    hw_can_init(&can);

    io_canTx_init(io_jsoncan_pushTxMsgToQueue);
    io_canTx_enableMode(CAN_MODE_DEFAULT, true);
    io_can_init(&can_config);
    io_chimera_init(&debug_uart, GpioNetName_rsm_net_name_tag, AdcNetName_rsm_net_name_tag, &n_chimera_pin);

    app_canTx_init();
    app_canRx_init();

    app_globals_init(&config);

    app_heartbeatMonitor_init(
        heartbeatMonitorChecklist, heartbeatGetters, heartbeatUpdaters, &app_canTx_RSM_Heartbeat_set,
        heartbeatFaultSetters, heartbeatFaultGetters);

    app_stateMachine_init(app_mainState_get());

    app_canTx_RSM_Hash_set(GIT_COMMIT_HASH);
    app_canTx_RSM_Clean_set(GIT_COMMIT_CLEAN);
    // TODO: Re-enable watchdog.
}

void tasks_run100Hz(void)
{
    io_chimera_sleepTaskIfEnabled();

    static const TickType_t period_ms   = 10;
    static uint32_t         start_ticks = 0;
    start_ticks                         = osKernelGetTickCount();

    for (;;)
    {
        const uint32_t start_time_ms = osKernelGetTickCount();

        app_stateMachine_tick100Hz();
        io_canTx_enqueue100HzMsgs();

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

void tasks_run1kHz(void)
{
    io_chimera_sleepTaskIfEnabled();

    static const TickType_t period_ms   = 1U;
    static uint32_t         start_ticks = 0;
    start_ticks                         = osKernelGetTickCount();

    for (;;)
    {
        const uint32_t start_time_ms = osKernelGetTickCount();

        const uint32_t task_start_ms = TICK_TO_MS(osKernelGetTickCount());
        io_canTx_enqueueOtherPeriodicMsgs(task_start_ms);

        start_ticks += period_ms;
        osDelayUntil(start_ticks);
    }
}

void tasks_run1Hz(void)
{
    io_chimera_sleepTaskIfEnabled();

    static const TickType_t period_ms   = 1000U;
    static uint32_t         start_ticks = 0;
    start_ticks                         = osKernelGetTickCount();

    for (;;)
    {
        hw_stackWaterMarkConfig_check();
        app_stateMachine_tick1Hz();

        const bool debug_mode_enabled = app_canRx_Debug_EnableDebugMode_get();
        io_canTx_enableMode(CAN_MODE_DEBUG, debug_mode_enabled);
        io_canTx_enqueue1HzMsgs();

        start_ticks += period_ms;
        osDelayUntil(start_ticks);
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    io_coolant_inputCaptureCallback(&htim3);

    if (huart == debug_uart.handle)
    {
        io_chimera_msgRxCallback();
    }
}
