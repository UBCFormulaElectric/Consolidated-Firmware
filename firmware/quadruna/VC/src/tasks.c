#include "tasks.h"
#include "main.h"
#include "cmsis_os.h"
#include "string.h"
#include "shared.pb.h"

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
#include "io_chimera.h"

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

extern ADC_HandleTypeDef   hadc1;
extern ADC_HandleTypeDef   hadc3;
extern FDCAN_HandleTypeDef hfdcan1;
extern UART_HandleTypeDef  huart7;
extern TIM_HandleTypeDef   htim3;
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
static const Gpio      bat_i_sns_nflt   = { .port = BAT_I_SNS_nFLT_GPIO_Port, .pin = BAT_I_SNS_nFLT_Pin };
static const BinaryLed led              = { .gpio = { .port = LED_GPIO_Port, .pin = LED_Pin } };
static const Gpio      telem_pwr_en     = { .port = TELEM_PWR_EN_GPIO_Port, .pin = TELEM_PWR_EN_Pin };
static const Gpio      npcm_en          = { .port = nPCM_EN_GPIO_Port, .pin = nPCM_EN_Pin };
static const Gpio      acc_i_sense_nflt = { .port = ACC_I_SENSE_nFLT_GPIO_Port, .pin = ACC_I_SENSE_nFLT_Pin };
static const Gpio      pgood            = { .port = PGOOD_GPIO_Port, .pin = PGOOD_Pin };
static const Gpio      lv_pwr_en        = { .port = LV_PWR_EN_GPIO_Port, .pin = LV_PWR_EN_Pin };
static const Gpio      aux_pwr_en       = { .port = AUX_PWR_EN_GPIO_Port, .pin = AUX_PWR_EN_Pin };
static const Gpio      pump_pwr_en      = { .port = PUMP_PWR_EN_GPIO_Port, .pin = PUMP_PWR_EN_Pin };
static const Gpio      _900k_gpio       = { .port = _900K_GPIO_GPIO_Port, .pin = _900K_GPIO_Pin };
static const Gpio      nchrg_fault      = { .port = nCHRG_FAULT_GPIO_Port, .pin = nCHRG_FAULT_Pin };
static const Gpio      nchrg            = { .port = nCHRG_GPIO_Port, .pin = nCHRG_Pin };
static const Gpio      inv_l_pwr_en     = { .port = INV_L_PWR_EN_GPIO_Port, .pin = INV_L_PWR_EN_Pin };
static const Gpio      inv_r_pwr_en     = { .port = INV_R_PWR_EN_GPIO_Port, .pin = INV_R_PWR_EN_Pin };
static const Gpio      shdn_pwr_en      = { .port = SHDN_PWR_EN_GPIO_Port, .pin = SHDN_PWR_EN_Pin };
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

const Gpio *id_to_gpio[] = { [VC_GpioNetName_BUZZER_PWR_EN]    = &buzzer_pwr_en,
                             [VC_GpioNetName_BAT_I_SNS_NFLT]   = &bat_i_sns_nflt,
                             [VC_GpioNetName_LED]              = &led.gpio,
                             [VC_GpioNetName_TELEM_PWR_EN]     = &telem_pwr_en,
                             [VC_GpioNetName_NPCM_EN]          = &npcm_en,
                             [VC_GpioNetName_ACC_I_SENSE_NFLT] = &acc_i_sense_nflt,
                             [VC_GpioNetName_PGOOD]            = &pgood,
                             [VC_GpioNetName_LV_PWR_EN]        = &lv_pwr_en,
                             [VC_GpioNetName_AUX_PWR_EN]       = &aux_pwr_en,
                             [VC_GpioNetName_PUMP_PWR_EN]      = &pump_pwr_en,
                             [VC_GpioNetName__900K_GPIO]       = &_900k_gpio,
                             [VC_GpioNetName_NCHRG_FAULT]      = &nchrg_fault,
                             [VC_GpioNetName_NCHRG]            = &nchrg,
                             [VC_GpioNetName_INV_L_PWR_EN]     = &inv_l_pwr_en,
                             [VC_GpioNetName_INV_R_PWR_EN]     = &inv_r_pwr_en,
                             [VC_GpioNetName_SHDN_PWR_EN]      = &shdn_pwr_en,
                             [VC_GpioNetName_FR_STBY1]         = &fr_stby1,
                             [VC_GpioNetName_FR_STBY2]         = &fr_stby2,
                             [VC_GpioNetName_FR_STBY3]         = &fr_stby3,
                             [VC_GpioNetName_INV_L_PROGRAM]    = &inv_l_program,
                             [VC_GpioNetName_INV_R_PROGRAM]    = &inv_r_program,
                             [VC_GpioNetName_L_SHDN_SNS]       = &l_shdn_sns,
                             [VC_GpioNetName_R_SHDN_SNS]       = &r_shdn_sns,
                             [VC_GpioNetName_NCHIMERA]         = &nchimera,
                             [VC_GpioNetName_NPROGRAM_3V3]     = &nprogram_3v3,
                             [VC_GpioNetName_SB_ILCK_SHDN_SNS] = &sb_ilck_shdn_sns,
                             [VC_GpioNetName_TSMS_SHDN_SNS]    = &tsms_shdn_sns };

const AdcChannel id_to_adc[] = {
    [VC_AdcNetName_INV_R_PWR_I_SNS]  = ADC1_IN10_INV_R_PWR_I_SNS,
    [VC_AdcNetName_INV_L_PWR_I_SNS]  = ADC1_IN11_INV_L_PWR_I_SNS,
    [VC_AdcNetName_AUX_PWR_I_SNS]    = ADC3_IN0_AUX_PWR_I_SNS,
    [VC_AdcNetName_SHDN_PWR_I_SNS]   = ADC1_IN18_SHDN_PWR_I_SNS,
    [VC_AdcNetName_VBAT_SENSE]       = ADC1_IN19_VBAT_SENSE,
    [VC_AdcNetName__24V_ACC_SENSE]   = ADC1_IN3_24V_ACC_SENSE,
    [VC_AdcNetName__22V_BOOST_SENSE] = ADC1_IN7_22V_BOOST_SENSE,
    [VC_AdcNetName_LV_PWR_I_SNS]     = ADC1_IN4_LV_PWR_I_SNS,
    [VC_AdcNetName_ACC_I_SENSE]      = ADC1_IN5_ACC_I_SENSE,
    [VC_AdcNetName_PUMP_PWR_I_SNS]   = ADC3_IN1_PUMP_PWR_I_SNS,
};

static UART debug_uart = { .handle = &huart7 };

void tasks_preInit(void)
{
    hw_bootup_enableInterruptsForApp();
}

void tasks_init(void)
{
    __HAL_DBGMCU_FREEZE_IWDG1();

    // Configure and initialize SEGGER SystemView.
    SEGGER_SYSVIEW_Conf();
    LOG_INFO("VC reset!");

    can.can                   = hfdcan1;
    can.canMsgRecievecallback = NULL;
    
    hw_hardFaultHandler_init();
    hw_can_init(&hfdcan1);

    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)hw_adc_getRawValuesBuffer(), hadc1.Init.NbrOfConversion);
    HAL_TIM_Base_Start(&htim3);

    // Start interrupt mode for ADC3, since we can't use DMA (see `firmware/quadruna/VC/src/hw/hw_adc.c` for a more
    // in-depth comment).
    HAL_ADC_Start_IT(&hadc3);

    // TODO: Re-enable watchdog (disabled because it can get annoying when bringing up a board).
    hw_watchdog_init(hw_watchdogConfig_refresh, hw_watchdogConfig_timeoutCallback);

    io_canTx_init(io_jsoncan_pushTxMsgToQueue);
    io_canTx_enableMode(CAN_MODE_DEFAULT, true);
    io_can_init(&can_config);
    io_chimera_init(&debug_uart, GpioNetName_vc_net_name_tag, AdcNetName_vc_net_name_tag);

    app_canTx_init();
    app_canRx_init();
    app_stateMachine_init(app_initState_get());

    app_canTx_VC_Hash_set(GIT_COMMIT_HASH);
    app_canTx_VC_Clean_set(GIT_COMMIT_CLEAN);
}

void tasks_run1Hz(void)
{
    // TODO: TemporarilQy disabled for hardware testing (chimera).
    osDelay(osWaitForever);

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
    // TODO: TemporarilQy disabled for hardware testing (chimera).
    osDelay(osWaitForever);

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
    // TODO: TemporarilQy disabled for hardware testing (chimera).
    osDelay(osWaitForever);

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
    // TODO: TemporarilQy disabled for hardware testing (chimera).
    osDelay(osWaitForever);

    for (;;)
    {
        io_can_transmitMsgFromQueue();
    }
}

void tasks_runCanRx(void)
{
    // TODO: TemporarilQy disabled for hardware testing (chimera).
    osDelay(osWaitForever);

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
