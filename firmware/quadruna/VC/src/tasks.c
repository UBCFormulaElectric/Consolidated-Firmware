#include "tasks.h"
#include "main.h"
#include "cmsis_os.h"
#include "string.h"
#include "shared.pb.h"

#include "states/app_allStates.h"
#include "states/app_initState.h"
#include "app_heartbeatMonitor.h"
#include "app_canTx.h"
#include "app_canRx.h"
#include "app_canAlerts.h"
#include "app_canDataCapture.h"
#include "app_commitInfo.h"
#include "app_powerManager.h"
#include "app_efuse.h"
#include "app_shdnLoop.h"
#include "app_faultCheck.h"

#include "io_jsoncan.h"
#include "io_log.h"
#include "io_canRx.h"
#include "io_led.h"
#include "io_chimera.h"
#include "io_efuse.h"
#include "io_lowVoltageBattery.h"
#include "io_vcShdn.h"
#include "io_currentSensing.h"
#include "io_sbgEllipse.h"
#include "io_canLogging.h"
#include "io_fileSystem.h"
#include "io_imu.h"
#include "io_telemMessage.h"
#include "io_pcm.h"
#include "io_time.h"

#include "hw_bootup.h"
#include "hw_utils.h"
#include "hw_hardFaultHandler.h"
#include "hw_watchdog.h"
#include "hw_watchdogConfig.h"
#include "hw_gpio.h"
#include "hw_stackWaterMarkConfig.h"
#include "hw_uart.h"
#include "hw_adc.h"
#include "hw_sd.h"

extern ADC_HandleTypeDef   hadc1;
extern ADC_HandleTypeDef   hadc3;
extern FDCAN_HandleTypeDef hfdcan1;
extern UART_HandleTypeDef  huart7;
extern TIM_HandleTypeDef   htim3;
extern UART_HandleTypeDef  huart2;
extern UART_HandleTypeDef  huart1;
extern UART_HandleTypeDef  huart3;
extern SD_HandleTypeDef    hsd1;

static uint32_t can_logging_overflow_count = 0;
static uint32_t read_count                 = 0; // TODO debugging variables
static uint32_t write_count                = 0; // TODO debugging variables
static bool     sd_card_present            = true;

static void canRxCallback(CanMsg *rx_msg)
{
    io_can_pushRxMsgToQueue(rx_msg); // push to queue

    if (sd_card_present && app_dataCapture_needsLog((uint16_t)rx_msg->std_id, io_time_getCurrentMs()))
    {
        io_canLogging_loggingQueuePush(rx_msg); // push to logging queue
        read_count++;
    }
}

SdCard                 sd  = { .hsd = &hsd1, .timeout = 1000 };
static const CanHandle can = { .can = &hfdcan1, .can_msg_received_callback = canRxCallback };

void canRxQueueOverflowCallBack(uint32_t overflow_count)
{
    app_canTx_VC_RxOverflowCount_set(overflow_count);
    app_canAlerts_VC_Warning_RxOverflow_set(true);
    LOG_INFO("CAN RX OVERFLOW");
}

void canTxQueueOverflowCallBack(uint32_t overflow_count)
{
    app_canTx_VC_TxOverflowCount_set(overflow_count);
    app_canAlerts_VC_Warning_TxOverflow_set(true);
    LOG_INFO("CAN TX OVERFLOW");
}

void canTxQueueOverflowClearCallback(void)
{
    app_canAlerts_VC_Warning_TxOverflow_set(false);
}

void canRxQueueOverflowClearCallback(void)
{
    app_canAlerts_VC_Warning_RxOverflow_set(false);
}

static const CanConfig io_can_config = {
    .rx_msg_filter              = io_canRx_filterMessageId,
    .tx_overflow_callback       = canTxQueueOverflowCallBack,
    .rx_overflow_callback       = canRxQueueOverflowCallBack,
    .tx_overflow_clear_callback = canTxQueueOverflowClearCallback,
    .rx_overflow_clear_callback = canRxQueueOverflowClearCallback,
};

static bool canLoggingFilter(uint32_t msg_id)
{
    return true;
}

static void canLoggingQueueOverflowCallback(uint32_t f)
{
    can_logging_overflow_count++;
}

static const CanConfig canLogging_config = {
    .rx_msg_filter              = canLoggingFilter,
    .tx_overflow_callback       = canLoggingQueueOverflowCallback,
    .rx_overflow_callback       = canLoggingQueueOverflowCallback,
    .tx_overflow_clear_callback = NULL,
    .rx_overflow_clear_callback = NULL,
};

extern Gpio            sd_present;
static const Gpio      buzzer_pwr_en    = { .port = BUZZER_PWR_EN_GPIO_Port, .pin = BUZZER_PWR_EN_Pin };
static const Gpio      bat_i_sns_nflt   = { .port = BAT_I_SNS_nFLT_GPIO_Port, .pin = BAT_I_SNS_nFLT_Pin };
static const BinaryLed led              = { .gpio = { .port = LED_GPIO_Port, .pin = LED_Pin } };
static const Gpio      telem_pwr_en     = { .port = TELEM_PWR_EN_GPIO_Port, .pin = TELEM_PWR_EN_Pin };
static const Gpio      npcm_en          = { .port = nPCM_EN_GPIO_Port, .pin = nPCM_EN_Pin };
static const Gpio      acc_i_sns_nflt   = { .port = ACC_I_SENSE_nFLT_GPIO_Port, .pin = ACC_I_SENSE_nFLT_Pin };
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
static const Gpio      n_chimera_pin    = { .port = NCHIMERA_GPIO_Port, .pin = NCHIMERA_Pin };
static const Gpio      nprogram_3v3     = { .port = NPROGRAM_3V3_GPIO_Port, .pin = NPROGRAM_3V3_Pin };
static const Gpio      sb_ilck_shdn_sns = { .port = SB_ILCK_SHDN_SNS_GPIO_Port, .pin = SB_ILCK_SHDN_SNS_Pin };
static const Gpio      tsms_shdn_sns    = { .port = TSMS_SHDN_SNS_GPIO_Port, .pin = TSMS_SHDN_SNS_Pin };

const Gpio *id_to_gpio[] = { [VC_GpioNetName_BUZZER_PWR_EN]    = &buzzer_pwr_en,
                             [VC_GpioNetName_BAT_I_SNS_NFLT]   = &bat_i_sns_nflt,
                             [VC_GpioNetName_LED]              = &led.gpio,
                             [VC_GpioNetName_TELEM_PWR_EN]     = &telem_pwr_en,
                             [VC_GpioNetName_NPCM_EN]          = &npcm_en,
                             [VC_GpioNetName_ACC_I_SENSE_NFLT] = &acc_i_sns_nflt,
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
                             [VC_GpioNetName_NCHIMERA]         = &n_chimera_pin,
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

static const CurrentSensingConfig current_sensing_config = {
    .bat_fault_gpio  = bat_i_sns_nflt,
    .acc_fault_gpio  = acc_i_sns_nflt,
    .bat_current_adc = ADC1_IN14_BAT_I_SNS,
    .acc_current_adc = ADC1_IN5_ACC_I_SENSE,
};

static const VcShdnConfig shutdown_config = { .tsms_gpio                   = &tsms_shdn_sns,
                                              .LE_stop_gpio                = &l_shdn_sns,
                                              .RE_stop_gpio                = &r_shdn_sns,
                                              .splitter_box_interlock_gpio = &sb_ilck_shdn_sns };

static const BoardShdnNode vc_shdn_nodes[VC_SHDN_NODE_COUNT] = {
    { io_vcShdn_TsmsFault_get, &app_canTx_VC_TSMSOKStatus_set },
    { io_vcShdn_LEStopFault_get, &app_canTx_VC_LEStopOKStatus_set },
    { io_vcShdn_REStopFault_get, &app_canTx_VC_REStopOKStatus_set },
    { io_vcShdn_SplitterBoxInterlockFault_get, &app_canTx_VC_SplitterBoxInterlockOKStatus_set },
};

static const LvBatteryConfig lv_battery_config = { .lt3650_charger_fault_gpio = nchrg_fault,
                                                   .ltc3786_boost_fault_gpio  = pgood,
                                                   .vbat_vsense_adc_channel   = id_to_adc[VC_AdcNetName_VBAT_SENSE],
                                                   .boost_vsense_adc_channel =
                                                       id_to_adc[VC_AdcNetName__22V_BOOST_SENSE],
                                                   .acc_vsense_adc_channel = id_to_adc[VC_AdcNetName__24V_ACC_SENSE] };

static const EfuseConfig efuse_configs[NUM_EFUSE_CHANNELS] = {
    [EFUSE_CHANNEL_SHDN] = {
        .enable_gpio = &shdn_pwr_en,
        .stby_reset_gpio = &fr_stby1,
        .cur_sns_adc_channel = ADC1_IN18_SHDN_PWR_I_SNS,
    },
    [EFUSE_CHANNEL_LV] = {
        .enable_gpio = &lv_pwr_en,
        .stby_reset_gpio = &fr_stby1,
        .cur_sns_adc_channel = ADC1_IN4_LV_PWR_I_SNS,
    },
    [EFUSE_CHANNEL_PUMP] = {
        .enable_gpio = &pump_pwr_en,
        .stby_reset_gpio = &fr_stby2,
        .cur_sns_adc_channel = ADC3_IN1_PUMP_PWR_I_SNS
    },
    [EFUSE_CHANNEL_AUX] = {
        .enable_gpio = &aux_pwr_en,
        .stby_reset_gpio = &fr_stby2,
        .cur_sns_adc_channel = ADC3_IN0_AUX_PWR_I_SNS
    },
    [EFUSE_CHANNEL_INV_R] = {
        .enable_gpio = &inv_r_pwr_en,
        .stby_reset_gpio = &fr_stby3,
        .cur_sns_adc_channel = ADC1_IN10_INV_R_PWR_I_SNS
    },
    [EFUSE_CHANNEL_INV_L] = {
        .enable_gpio = &inv_l_pwr_en,
        .stby_reset_gpio = &fr_stby3,
        .cur_sns_adc_channel = ADC1_IN11_INV_L_PWR_I_SNS
    },
    [EFUSE_CHANNEL_TELEM] = {
        .enable_gpio = &telem_pwr_en,
        .stby_reset_gpio = NULL,
        .cur_sns_adc_channel = NO_ADC_CHANNEL
    },
    [EFUSE_CHANNEL_BUZZER] = {
        .enable_gpio = &buzzer_pwr_en,
        .stby_reset_gpio = NULL,
        .cur_sns_adc_channel = NO_ADC_CHANNEL
    }
};

static const PcmConfig pcm_config = { .pcm_gpio = &npcm_en };

static void (*const efuse_enabled_can_setters[NUM_EFUSE_CHANNELS])(bool) = {
    [EFUSE_CHANNEL_SHDN]   = app_canTx_VC_ShdnStatus_set,
    [EFUSE_CHANNEL_LV]     = app_canTx_VC_LvStatus_set,
    [EFUSE_CHANNEL_PUMP]   = app_canTx_VC_PumpStatus_set,
    [EFUSE_CHANNEL_AUX]    = app_canTx_VC_AuxStatus_set,
    [EFUSE_CHANNEL_INV_R]  = app_canTx_VC_InvRStatus_set,
    [EFUSE_CHANNEL_INV_L]  = app_canTx_VC_InvLStatus_set,
    [EFUSE_CHANNEL_TELEM]  = NULL,
    [EFUSE_CHANNEL_BUZZER] = NULL,
};

static void (*const efuse_current_can_setters[NUM_EFUSE_CHANNELS])(float) = {
    [EFUSE_CHANNEL_SHDN]   = app_canTx_VC_ShdnCurrent_set,
    [EFUSE_CHANNEL_LV]     = app_canTx_VC_LvCurrent_set,
    [EFUSE_CHANNEL_PUMP]   = app_canTx_VC_PumpCurrent_set,
    [EFUSE_CHANNEL_AUX]    = app_canTx_VC_AuxCurrent_set,
    [EFUSE_CHANNEL_INV_R]  = app_canTx_VC_InvRCurrent_set,
    [EFUSE_CHANNEL_INV_L]  = app_canTx_VC_InvLCurrent_set,
    [EFUSE_CHANNEL_TELEM]  = NULL,
    [EFUSE_CHANNEL_BUZZER] = NULL,
};
static const UART  debug_uart    = { .handle = &huart7 };
static const UART  imu_uart      = { .handle = &huart2 };
static const UART  modem2G4_uart = { .handle = &huart3 };
static const UART  modem900_uart = { .handle = &huart1 };
static const Modem modem         = { .modem2_4G = &modem2G4_uart, .modem900M = &modem900_uart };

// config for heartbeat monitor (can funcs and flags)
// VC relies on FSM, RSM, BMS, CRIT
const bool heartbeatMonitorChecklist[HEARTBEAT_BOARD_COUNT] = {
    [BMS_HEARTBEAT_BOARD] = true, [VC_HEARTBEAT_BOARD] = false,  [RSM_HEARTBEAT_BOARD] = true,
    [FSM_HEARTBEAT_BOARD] = true, [DIM_HEARTBEAT_BOARD] = false, [CRIT_HEARTBEAT_BOARD] = true
};

// heartbeatGetters - get heartbeat signals from other boards
bool (*const heartbeatGetters[HEARTBEAT_BOARD_COUNT])(void) = { [BMS_HEARTBEAT_BOARD]  = app_canRx_BMS_Heartbeat_get,
                                                                [VC_HEARTBEAT_BOARD]   = NULL,
                                                                [RSM_HEARTBEAT_BOARD]  = app_canRx_RSM_Heartbeat_get,
                                                                [FSM_HEARTBEAT_BOARD]  = app_canRx_FSM_Heartbeat_get,
                                                                [DIM_HEARTBEAT_BOARD]  = NULL,
                                                                [CRIT_HEARTBEAT_BOARD] = app_canRx_CRIT_Heartbeat_get };

// heartbeatUpdaters - update local CAN table with heartbeat status
void (*const heartbeatUpdaters[HEARTBEAT_BOARD_COUNT])(bool) = {
    [BMS_HEARTBEAT_BOARD]  = app_canRx_BMS_Heartbeat_update,
    [VC_HEARTBEAT_BOARD]   = NULL,
    [RSM_HEARTBEAT_BOARD]  = app_canRx_RSM_Heartbeat_update,
    [FSM_HEARTBEAT_BOARD]  = app_canRx_FSM_Heartbeat_update,
    [DIM_HEARTBEAT_BOARD]  = NULL,
    [CRIT_HEARTBEAT_BOARD] = app_canRx_CRIT_Heartbeat_update
};

// heartbeatFaultSetters - broadcast heartbeat faults over CAN
void (*const heartbeatFaultSetters[HEARTBEAT_BOARD_COUNT])(bool) = {
    [BMS_HEARTBEAT_BOARD]  = app_canAlerts_VC_Fault_MissingBMSHeartbeat_set,
    [VC_HEARTBEAT_BOARD]   = NULL,
    [RSM_HEARTBEAT_BOARD]  = app_canAlerts_VC_Fault_MissingRSMHeartbeat_set,
    [FSM_HEARTBEAT_BOARD]  = app_canAlerts_VC_Fault_MissingFSMHeartbeat_set,
    [DIM_HEARTBEAT_BOARD]  = NULL,
    [CRIT_HEARTBEAT_BOARD] = app_canAlerts_VC_Fault_MissingCRITHeartbeat_set
};

// heartbeatFaultGetters - gets fault statuses over CAN
bool (*const heartbeatFaultGetters[HEARTBEAT_BOARD_COUNT])(void) = {
    [BMS_HEARTBEAT_BOARD]  = app_canAlerts_VC_Fault_MissingBMSHeartbeat_get,
    [VC_HEARTBEAT_BOARD]   = NULL,
    [RSM_HEARTBEAT_BOARD]  = app_canAlerts_VC_Fault_MissingRSMHeartbeat_get,
    [FSM_HEARTBEAT_BOARD]  = app_canAlerts_VC_Fault_MissingFSMHeartbeat_get,
    [DIM_HEARTBEAT_BOARD]  = NULL,
    [CRIT_HEARTBEAT_BOARD] = app_canAlerts_VC_Fault_MissingCRITHeartbeat_get
};

void tasks_preInit(void)
{
    hw_bootup_enableInterruptsForApp();
}

void tasks_preInitWatchdog(void)
{
    hw_sd_init(&sd);

    sd_card_present = !hw_gpio_readPin(&sd_present);
    if (sd_card_present)
    {
        if (io_fileSystem_init() == FILE_OK)
        {
            io_canLogging_init(&canLogging_config);
        }
        else
        {
            sd_card_present = false;
        }
    }
}

void tasks_init(void)
{
    // Configure and initialize SEGGER SystemView.
    // NOTE: Needs to be done after clock config!
    SEGGER_SYSVIEW_Conf();
    LOG_INFO("VC reset!");

    __HAL_DBGMCU_FREEZE_IWDG1();

    hw_hardFaultHandler_init();
    hw_can_init(&can);
    hw_watchdog_init(hw_watchdogConfig_refresh, hw_watchdogConfig_timeoutCallback);

    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)hw_adc_getRawValuesBuffer(), hadc1.Init.NbrOfConversion);
    HAL_TIM_Base_Start(&htim3);

    // Start interrupt mode for ADC3, since we can't use DMA (see `firmware/quadruna/VC/src/hw/hw_adc.c` for a more
    // in-depth comment).
    HAL_ADC_Start_IT(&hadc3);

    io_canTx_init(io_jsoncan_pushTxMsgToQueue);
    io_canTx_enableMode(CAN_MODE_DEFAULT, true);
    io_can_init(&io_can_config);
    io_chimera_init(&debug_uart, GpioNetName_vc_net_name_tag, AdcNetName_vc_net_name_tag, &n_chimera_pin);

    io_lowVoltageBattery_init(&lv_battery_config);
    io_vcShdn_init(&shutdown_config);
    io_currentSensing_init(&current_sensing_config);
    io_efuse_init(efuse_configs);

    if (!io_sbgEllipse_init(&imu_uart))
    {
        Error_Handler();
    }

    if (!io_imu_init())
    {
        app_canAlerts_VC_Warning_ImuInitFailed_set(true);
    }

    app_canTx_init();
    app_canRx_init();
    app_canDataCapture_init();

    // Empirically, mounting slows down (takes ~500ms) at 200 CAN logs on disk.
    // This is not correlated to the size of each file.
    app_canTx_VC_NumberOfCanDataLogs_set(io_canLogging_getCurrentLog());
    app_canAlerts_VC_Warning_HighNumberOfCanDataLogs_set(io_canLogging_getCurrentLog() > HIGH_NUMBER_OF_LOGS_THRESHOLD);
    app_canAlerts_VC_Warning_CanLoggingSdCardNotPresent_set(!sd_card_present);

    app_heartbeatMonitor_init(
        heartbeatMonitorChecklist, heartbeatGetters, heartbeatUpdaters, &app_canTx_VC_Heartbeat_set,
        heartbeatFaultSetters, heartbeatFaultGetters);
    app_efuse_init(efuse_enabled_can_setters, efuse_current_can_setters);
    app_stateMachine_init(app_initState_get());
    io_telemMessage_init(&modem);

    io_lowVoltageBattery_init(&lv_battery_config);
    app_shdnLoop_init(vc_shdn_nodes, VC_SHDN_NODE_COUNT);
    io_currentSensing_init(&current_sensing_config);
    io_efuse_init(efuse_configs);
    app_efuse_init(efuse_enabled_can_setters, efuse_current_can_setters);

    app_canTx_VC_Hash_set(GIT_COMMIT_HASH);
    app_canTx_VC_Clean_set(GIT_COMMIT_CLEAN);

    app_bspd_init();

    // enable these for inverter programming
    // hw_gpio_writePin(&inv_l_program, true);
    // hw_gpio_writePin(&inv_r_program, true);
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
        app_allStates_runOnTick100Hz();
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
        CanMsg tx_msg;
        io_can_popTxMsgFromQueue(&tx_msg);
        io_telemMessage_pushMsgtoQueue(&tx_msg);

        if (sd_card_present)
        {
            io_canLogging_loggingQueuePush(&tx_msg);
        }

        io_can_transmitMsgFromQueue(&tx_msg);
    }
}

_Noreturn void tasks_runTelem(void)
{
    for (;;)
    {
        io_telemMessage_broadcastMsgFromQueue();
    }
}

_Noreturn void tasks_runCanRx(void)
{
    io_chimera_sleepTaskIfEnabled();

    for (;;)
    {
        CanMsg rx_msg;
        io_can_popRxMsgFromQueue(&rx_msg);
        io_telemMessage_pushMsgtoQueue(&rx_msg);

        JsonCanMsg jsoncan_rx_msg;
        io_jsoncan_copyFromCanMsg(&rx_msg, &jsoncan_rx_msg);
        io_canRx_updateRxTableWithMessage(&jsoncan_rx_msg);
    }
}

_Noreturn void tasks_runLogging(void)
{
    if (!sd_card_present)
    {
        osThreadSuspend(osThreadGetId());
    }

    static uint32_t message_batch_count = 0;
    for (;;)
    {
        io_canLogging_recordMsgFromQueue();
        message_batch_count++;
        write_count++;
        if (message_batch_count > 256)
        {
            io_canLogging_sync();
            message_batch_count = 0;
        }
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart == debug_uart.handle)
    {
        io_chimera_msgRxCallback();
    }
}
