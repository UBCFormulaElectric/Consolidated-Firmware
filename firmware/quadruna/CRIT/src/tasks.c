#include "tasks.h"
#include "main.h"
#include "cmsis_os.h"
// protobufs
#include "shared.pb.h"
#include "CRIT.pb.h"
// app
#include "app_heartbeatMonitor.h"
#include "app_stateMachine.h"
#include "app_mainState.h"
#include "app_globals.h"
#include "app_shdnLoop.h"
// io
#include "io_log.h"
#include "io_chimera.h"
#include "io_led.h"
#include "io_switch.h"
#include "io_rgbLed.h"
#include "io_critShdn.h"
#include "io_leds.h"
#include "io_switches.h"
// can
#include "io_jsoncan.h"
#include "io_can.h"
#include "io_canRx.h"
#include "io_driveMode.h"
#include "app_canTx.h"
#include "app_canRx.h"
#include "app_canAlerts.h"
#include "app_commitInfo.h"
// hw
#include "hw_gpio.h"
#include "hw_adc.h"
#include "hw_uart.h"
#include "hw_utils.h"
#include "hw_bootup.h"
#include "hw_watchdog.h"
#include "hw_watchdogConfig.h"
#include "hw_stackWaterMarkConfig.h"
#include "hw_hardFaultHandler.h"

extern ADC_HandleTypeDef  hadc1;
extern TIM_HandleTypeDef  htim3;
extern UART_HandleTypeDef huart2;
extern CAN_HandleTypeDef  hcan1;

static const CanHandle can = { .can = &hcan1, .can_msg_received_callback = io_can_pushRxMsgToQueue };

void canTxQueueOverflowCallback(uint32_t overflow_count)
{
    app_canTx_CRIT_TxOverflowCount_set(overflow_count);
    app_canAlerts_CRIT_Warning_TxOverflow_set(true);
    BREAK_IF_DEBUGGER_CONNECTED()
}

void canRxQueueOverflowCallback(uint32_t overflow_count)
{
    app_canTx_CRIT_RxOverflowCount_set(overflow_count);
    app_canAlerts_CRIT_Warning_RxOverflow_set(true);
    BREAK_IF_DEBUGGER_CONNECTED()
}

void canTxQueueOverflowClearCallback(void)
{
    app_canAlerts_CRIT_Warning_TxOverflow_set(false);
}

void canRxQueueOverflowClearCallback(void)
{
    app_canAlerts_CRIT_Warning_RxOverflow_set(false);
}

static const CanConfig can_config = { .rx_msg_filter              = io_canRx_filterMessageId,
                                      .tx_overflow_callback       = canTxQueueOverflowCallback,
                                      .rx_overflow_callback       = canRxQueueOverflowCallback,
                                      .tx_overflow_clear_callback = canTxQueueOverflowClearCallback,
                                      .rx_overflow_clear_callback = canRxQueueOverflowClearCallback };

static const BinaryLed imd_led       = { .gpio = {
                                             .port = IMD_R_GPIO_Port,
                                             .pin  = IMD_R_Pin,
                                   } };
static const BinaryLed bspd_led      = { .gpio = {
                                             .port = BSPD_R_GPIO_Port,
                                             .pin  = BSPD_R_Pin,
                                    } };
static const BinaryLed ams_led       = { .gpio = {
                                             .port = AMS_R_GPIO_Port,
                                             .pin  = AMS_R_Pin,
                                   } };
static const BinaryLed start_led     = { .gpio = {
                                             .port = START_LED_GPIO_Port,
                                             .pin  = START_LED_Pin,
                                     } };
static const BinaryLed regen_led     = { .gpio = {
                                             .port = REGEN_LED_GPIO_Port,
                                             .pin  = REGEN_LED_Pin,
                                     } };
static const BinaryLed torquevec_led = { .gpio = {
                                             .port = TORQUE_VECTORING_LED_GPIO_Port,
                                             .pin  = TORQUE_VECTORING_LED_Pin,
                                         } };

static const Switch start_switch = {
    .gpio = {
        .port = START_SIG_GPIO_Port,
        .pin = START_SIG_Pin,
    },
    .closed_state = true,
};
static const Switch regen_switch = {
    .gpio = {
        .port = REGEN_SIG_GPIO_Port,
        .pin = REGEN_SIG_Pin,
    },
    .closed_state = true,
};

static const Switch torquevec_switch = {
    .gpio = {
        .port = TORQUE_VECTORING_SIG_GPIO_Port,
        .pin = TORQUE_VECTORING_SIG_Pin,
    },
    .closed_state = true,
};

static const RgbLed shdn_led = {
    .red_gpio = {
        .port = SHDN_R_GPIO_Port,
        .pin  = SHDN_R_Pin,
    },
    .green_gpio = {
        .port = SHDN_G_GPIO_Port,
        .pin  = SHDN_G_Pin,
    },
    .blue_gpio = {
        .port = 0,
        .pin  = MAX_8_BITS_VALUE,
    },
};

static const RgbLed vc_status_led = {
    .red_gpio = {
        .port = VC_R_GPIO_Port,
        .pin = VC_R_Pin,
    },
    .green_gpio = {
        .port = VC_G_GPIO_Port,
        .pin = VC_G_Pin,
    },
    .blue_gpio = {
        .port = VC_B_GPIO_Port,
        .pin = VC_B_Pin,
    },
};

static const RgbLed bms_status_led = {
    .red_gpio = {
        .port = BMS_R_GPIO_Port,
        .pin = BMS_R_Pin,
    },
    .green_gpio = {
        .port = BMS_G_GPIO_Port,
        .pin = BMS_G_Pin,
    },
    .blue_gpio = {
        .port = BMS_B_GPIO_Port,
        .pin = BMS_B_Pin,
    },
};

static const RgbLed fsm_status_led = {
    .red_gpio = {
        .port = FSM_R_GPIO_Port,
        .pin = FSM_R_Pin,
    },
    .green_gpio = {
        .port = FSM_G_GPIO_Port,
        .pin = FSM_G_Pin,
    },
    .blue_gpio = {
        .port = FSM_B_GPIO_Port,
        .pin = FSM_B_Pin,
    },
};

static const RgbLed aux_status_led = {
    .red_gpio = {
        .port = AUX_DB_R_GPIO_Port,
        .pin = AUX_DB_R_Pin,
    },
    .green_gpio = {
        .port = AUX_DB_G_GPIO_Port,
        .pin = AUX_DB_G_Pin,
    },
    .blue_gpio = {
        .port = AUX_DB_B_GPIO_Port,
        .pin = AUX_DB_B_Pin,
    },
};

static const RgbLed rsm_status_led = {
    .red_gpio = {
        .port = RSM_R_GPIO_Port,
        .pin = RSM_R_Pin,
    },
    .green_gpio = {
        .port = RSM_G_GPIO_Port,
        .pin = RSM_G_Pin,
    },
    .blue_gpio = {
        .port = RSM_B_GPIO_Port,
        .pin = RSM_B_Pin,
    },
};

static const RgbLed crit_status_led = {
    .red_gpio = {
        .port = CRIT_DB_R_GPIO_Port,
        .pin = CRIT_DB_R_Pin,
    },
    .green_gpio = {
        .port = CRIT_DB_G_GPIO_Port,
        .pin = CRIT_DB_G_Pin,
    },
    .blue_gpio = {
        .port = CRIT_DB_B_GPIO_Port,
        .pin = CRIT_DB_B_Pin,
    },
};

// clang-format off
static const Gpio ams_r_pin  = { .port = AMS_R_GPIO_Port, .pin = AMS_R_Pin };
static const Gpio imd_r_pin  = { .port = IMD_R_GPIO_Port, .pin = IMD_R_Pin };
static const Gpio bspd_r_pin = { .port = BSPD_R_GPIO_Port, .pin = BSPD_R_Pin };
static const Gpio shdn_r_pin = { .port = SHDN_R_GPIO_Port, .pin = SHDN_R_Pin };
static const Gpio shdn_g_pin = { .port = SHDN_G_GPIO_Port, .pin = SHDN_G_Pin };

static const Gpio vc_r_pin = { .port = VC_R_GPIO_Port, .pin = VC_R_Pin };
static const Gpio vc_g_pin = { .port = VC_G_GPIO_Port, .pin = VC_G_Pin };
static const Gpio vc_b_pin = { .port = VC_B_GPIO_Port, .pin = VC_B_Pin };

static const Gpio bms_r_pin = { .port = BMS_R_GPIO_Port, .pin = BMS_R_Pin };
static const Gpio bms_g_pin = { .port = BMS_G_GPIO_Port, .pin = BMS_G_Pin };
static const Gpio bms_b_pin = { .port = BMS_B_GPIO_Port, .pin = BMS_B_Pin };

static const Gpio fsm_r_pin = { .port = FSM_R_GPIO_Port, .pin = FSM_R_Pin };
static const Gpio fsm_g_pin = { .port = FSM_G_GPIO_Port, .pin = FSM_G_Pin };
static const Gpio fsm_b_pin = { .port = FSM_B_GPIO_Port, .pin = FSM_B_Pin };

static const Gpio rsm_r_pin = { .port = RSM_R_GPIO_Port, .pin = RSM_R_Pin };
static const Gpio rsm_g_pin = { .port = RSM_G_GPIO_Port, .pin = RSM_G_Pin };
static const Gpio rsm_b_pin = { .port = RSM_B_GPIO_Port, .pin = RSM_B_Pin };

static const Gpio aux_r_pin = { .port = AUX_DB_R_GPIO_Port, .pin = AUX_DB_R_Pin };
static const Gpio aux_g_pin = { .port = AUX_DB_G_GPIO_Port, .pin = AUX_DB_G_Pin };
static const Gpio aux_b_pin = { .port = AUX_DB_B_GPIO_Port, .pin = AUX_DB_B_Pin };

static const Gpio crit_r_pin = { .port = CRIT_DB_R_GPIO_Port, .pin = CRIT_DB_R_Pin };
static const Gpio crit_g_pin = { .port = CRIT_DB_G_GPIO_Port, .pin = CRIT_DB_G_Pin };
static const Gpio crit_b_pin = { .port = CRIT_DB_B_GPIO_Port, .pin = CRIT_DB_B_Pin };

static const Gpio start_led_pin            = { .port = START_LED_GPIO_Port, .pin = START_LED_Pin };
static const Gpio torque_vectoring_led_pin = { .port = TORQUE_VECTORING_LED_GPIO_Port, .pin  = TORQUE_VECTORING_LED_Pin };
static const Gpio regen_led_pin            = { .port = REGEN_LED_GPIO_Port, .pin = REGEN_LED_Pin };
static const Gpio led_pin                  = { .port = LED_GPIO_Port, .pin = LED_Pin };

static const Gpio start_sig_pin            = { .port = START_SIG_GPIO_Port, .pin = START_SIG_Pin };
static const Gpio torque_vectoring_sig_pin = { .port = TORQUE_VECTORING_SIG_GPIO_Port, .pin  = TORQUE_VECTORING_SIG_Pin };
static const Gpio regen_sig_pin            = { .port = REGEN_SIG_GPIO_Port, .pin = REGEN_SIG_Pin };

static const Gpio n_drive_mode_0_pin = { .port = NDRIVE_MODE_0b_GPIO_Port, .pin = NDRIVE_MODE_0b_Pin };
static const Gpio n_drive_mode_1_pin = { .port = NDRIVE_MODE_1b_GPIO_Port, .pin = NDRIVE_MODE_1b_Pin };
static const Gpio n_drive_mode_2_pin = { .port = NDRIVE_MODE_2b_GPIO_Port, .pin = NDRIVE_MODE_2b_Pin };
static const Gpio n_drive_mode_3_pin = { .port = NDRIVE_MODE_3b_GPIO_Port, .pin = NDRIVE_MODE_3b_Pin };

static const Gpio n_program_pin   = { .port = NPROGRAM_3V3_GPIO_Port, .pin = NPROGRAM_3V3_Pin };
static const Gpio n_chimera_pin   = { .port = NCHIMERA_GPIO_Port, .pin = NCHIMERA_Pin };
static const Gpio shdn_sen_pin    = { .port = SHDN_SEN_GPIO_Port, .pin = SHDN_SEN_Pin };
static const Gpio inertia_sen_pin = { .port = INERTIA_SEN_GPIO_Port, .pin = INERTIA_SEN_Pin };
// clang-format on

static const DriveMode drive_mode = { .n_drive_mode_0_pin = &n_drive_mode_0_pin,
                                      .n_drive_mode_1_pin = &n_drive_mode_1_pin,
                                      .n_drive_mode_2_pin = &n_drive_mode_2_pin,
                                      .n_drive_mode_3_pin = &n_drive_mode_3_pin };

const Gpio *id_to_gpio[] = {
    [CRIT_GpioNetName_TORQUE_VECTORING_LED] = &torque_vectoring_led_pin,
    [CRIT_GpioNetName_START_LED]            = &start_led_pin,
    [CRIT_GpioNetName_REGEN_LED]            = &regen_led_pin,
    [CRIT_GpioNetName_AUX_DB_R]             = &aux_r_pin,
    [CRIT_GpioNetName_CRIT_DB_R]            = &crit_r_pin,
    [CRIT_GpioNetName_CRIT_DB_G]            = &crit_g_pin,
    [CRIT_GpioNetName_CRIT_DB_B]            = &crit_b_pin,
    [CRIT_GpioNetName_AUX_DB_G]             = &aux_g_pin,
    [CRIT_GpioNetName_AUX_DB_B]             = &aux_b_pin,
    [CRIT_GpioNetName_BSPD_R]               = &bspd_r_pin,
    [CRIT_GpioNetName_SHDN_R]               = &shdn_r_pin,
    [CRIT_GpioNetName_RSM_B]                = &rsm_b_pin,
    [CRIT_GpioNetName_VC_R]                 = &vc_r_pin,
    [CRIT_GpioNetName_VC_B]                 = &vc_b_pin,
    [CRIT_GpioNetName_FSM_R]                = &fsm_r_pin,
    [CRIT_GpioNetName_BMS_R]                = &bms_r_pin,
    [CRIT_GpioNetName_BMS_G]                = &bms_g_pin,
    [CRIT_GpioNetName_BMS_B]                = &bms_b_pin,
    [CRIT_GpioNetName_AMS_R]                = &ams_r_pin,
    [CRIT_GpioNetName_NDRIVE_MODE_1b]       = &n_drive_mode_1_pin,
    [CRIT_GpioNetName_IMD_R]                = &imd_r_pin,
    [CRIT_GpioNetName_SDHN_SEN]             = &shdn_sen_pin,
    [CRIT_GpioNetName_INERTIA_SEN]          = &inertia_sen_pin,
    [CRIT_GpioNetName_FSM_G]                = &fsm_g_pin,
    [CRIT_GpioNetName_FSM_B]                = &fsm_b_pin,
    [CRIT_GpioNetName_RSM_R]                = &rsm_r_pin,
    [CRIT_GpioNetName_RSM_G]                = &rsm_g_pin,
    [CRIT_GpioNetName_VC_G]                 = &vc_g_pin,
    [CRIT_GpioNetName_SHDN_G]               = &shdn_g_pin,
    [CRIT_GpioNetName_NDRIVE_MODE_2b]       = &n_drive_mode_2_pin,
    [CRIT_GpioNetName_NDRIVE_MODE_3b]       = &n_drive_mode_3_pin,
    [CRIT_GpioNetName_NDRIVE_MODE_0b]       = &n_drive_mode_0_pin,
    [CRIT_GpioNetName_TORQUE_VECTORING_SIG] = &torque_vectoring_sig_pin,
    [CRIT_GpioNetName_REGEN_SIG]            = &regen_sig_pin,
    [CRIT_GpioNetName_LED]                  = &led_pin,
    [CRIT_GpioNetName_NPROGRAM_3V3]         = &n_program_pin,
    [CRIT_GpioNetName_START_SIG]            = &start_sig_pin,
    [CRIT_GpioNetName_NCHIMERA]             = &n_chimera_pin,
};

const AdcChannel id_to_adc[] = {
    [CRIT_AdcNetName_REGEN_3V3] = ADC1_IN14_REGEN,
};

static const UART debug_uart = { .handle = &huart2 };

static const GlobalsConfig globals_config = { .drive_mode = &drive_mode };

static const Leds led_config = {
    .imd_led         = &imd_led,
    .bspd_led        = &bspd_led,
    .ams_led         = &ams_led,
    .shdn_led        = &shdn_led,
    .start_led       = &start_led,
    .regen_led       = &regen_led,
    .torquevec_led   = &torquevec_led,
    .aux_status_led  = &aux_status_led,
    .bms_status_led  = &bms_status_led,
    .crit_status_led = &crit_status_led,
    .fsm_status_led  = &fsm_status_led,
    .rsm_status_led  = &rsm_status_led,
    .vc_status_led   = &vc_status_led,
};

static const Switches switch_config = {
    .start_switch     = &start_switch,
    .regen_switch     = &regen_switch,
    .torquevec_switch = &torquevec_switch,
};

// CRIT rellies on BMS, VC, RSM, FSM
static const bool heartbeatMonitorChecklist[HEARTBEAT_BOARD_COUNT] = {
    [BMS_HEARTBEAT_BOARD] = true, [VC_HEARTBEAT_BOARD] = true,   [RSM_HEARTBEAT_BOARD] = true,
    [FSM_HEARTBEAT_BOARD] = true, [DIM_HEARTBEAT_BOARD] = false, [CRIT_HEARTBEAT_BOARD] = false
};

// heartbeatGetters - get heartbeat signals from other boards
static bool (*const heartbeatGetters[HEARTBEAT_BOARD_COUNT])(void) = {
    [BMS_HEARTBEAT_BOARD]  = app_canRx_BMS_Heartbeat_get,
    [VC_HEARTBEAT_BOARD]   = app_canRx_VC_Heartbeat_get,
    [RSM_HEARTBEAT_BOARD]  = app_canRx_RSM_Heartbeat_get,
    [FSM_HEARTBEAT_BOARD]  = app_canRx_FSM_Heartbeat_get,
    [DIM_HEARTBEAT_BOARD]  = NULL,
    [CRIT_HEARTBEAT_BOARD] = NULL
};

// heartbeatUpdaters - update local CAN table with heartbeat status
static void (*const heartbeatUpdaters[HEARTBEAT_BOARD_COUNT])(bool) = {
    [BMS_HEARTBEAT_BOARD]  = app_canRx_BMS_Heartbeat_update,
    [VC_HEARTBEAT_BOARD]   = app_canRx_VC_Heartbeat_update,
    [RSM_HEARTBEAT_BOARD]  = app_canRx_RSM_Heartbeat_update,
    [FSM_HEARTBEAT_BOARD]  = app_canRx_FSM_Heartbeat_update,
    [DIM_HEARTBEAT_BOARD]  = NULL,
    [CRIT_HEARTBEAT_BOARD] = NULL
};

// heartbeatFaultSetters - broadcast heartbeat faults over CAN
static void (*const heartbeatFaultSetters[HEARTBEAT_BOARD_COUNT])(bool) = {
    [BMS_HEARTBEAT_BOARD]  = app_canAlerts_CRIT_Fault_MissingBMSHeartbeat_set,
    [VC_HEARTBEAT_BOARD]   = app_canAlerts_CRIT_Fault_MissingVCHeartbeat_set,
    [RSM_HEARTBEAT_BOARD]  = app_canAlerts_CRIT_Fault_MissingRSMHeartbeat_set,
    [FSM_HEARTBEAT_BOARD]  = app_canAlerts_CRIT_Fault_MissingFSMHeartbeat_set,
    [DIM_HEARTBEAT_BOARD]  = NULL,
    [CRIT_HEARTBEAT_BOARD] = NULL
};

// heartbeatFaultGetters - gets fault statuses over CAN
static bool (*const heartbeatFaultGetters[HEARTBEAT_BOARD_COUNT])(void) = {
    [BMS_HEARTBEAT_BOARD]  = app_canAlerts_CRIT_Fault_MissingBMSHeartbeat_get,
    [VC_HEARTBEAT_BOARD]   = app_canAlerts_CRIT_Fault_MissingVCHeartbeat_get,
    [RSM_HEARTBEAT_BOARD]  = app_canAlerts_CRIT_Fault_MissingRSMHeartbeat_get,
    [FSM_HEARTBEAT_BOARD]  = app_canAlerts_CRIT_Fault_MissingFSMHeartbeat_get,
    [DIM_HEARTBEAT_BOARD]  = NULL,
    [CRIT_HEARTBEAT_BOARD] = NULL
};

static const CritShdnConfig crit_shdn_pin_config = { .cockpit_estop_gpio  = shdn_sen_pin,
                                                     .inertia_sen_ok_gpio = inertia_sen_pin };

static const BoardShdnNode crit_bshdn_nodes[CRIT_SHDN_NODE_COUNT] = {
    { &io_critShdn_get_INERTIA_SEN_OK_get, &app_canTx_CRIT_InertiaSenOKStatus_set },
    { &io_critShdn_COCKPIT_ESTOP_OK_get, &app_canTx_CRIT_CockpitEStopOKStatus_set }
};

void tasks_preInit(void)
{
    hw_bootup_enableInterruptsForApp();
}

void tasks_init(void)
{
    // Configure and initialize SEGGER SystemView.
    // NOTE: Needs to be done after clock config!
    SEGGER_SYSVIEW_Conf();
    LOG_INFO("VC reset!");

    // Start DMA/TIM3 for the ADC.
    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)hw_adc_getRawValuesBuffer(), hadc1.Init.NbrOfConversion);
    HAL_TIM_Base_Start(&htim3);

    io_chimera_init(&debug_uart, GpioNetName_crit_net_name_tag, AdcNetName_crit_net_name_tag, &n_chimera_pin);

    // Re-enable watchdog.
    __HAL_DBGMCU_FREEZE_IWDG();

    hw_hardFaultHandler_init();
    hw_can_init(&can);
    hw_watchdog_init(hw_watchdogConfig_refresh, hw_watchdogConfig_timeoutCallback);

    io_canTx_init(io_jsoncan_pushTxMsgToQueue);
    io_canTx_enableMode(CAN_MODE_DEFAULT, true);
    io_can_init(&can_config);
    io_critShdn_init(&crit_shdn_pin_config);

    io_led_init(&led_config);
    io_switches_init(&switch_config);
    io_driveMode_init(&drive_mode);

    app_canTx_init();
    app_canRx_init();

    app_shdnLoop_init(crit_bshdn_nodes, CRIT_SHDN_NODE_COUNT);

    app_heartbeatMonitor_init(
        heartbeatMonitorChecklist, heartbeatGetters, heartbeatUpdaters, &app_canTx_CRIT_Heartbeat_set,
        heartbeatFaultSetters, heartbeatFaultGetters);

    app_stateMachine_init(app_mainState_get());
    app_globals_init(&globals_config);

    // broadcast commit info
    app_canTx_CRIT_Hash_set(GIT_COMMIT_HASH);
    app_canTx_CRIT_Clean_set(GIT_COMMIT_CLEAN);
}

_Noreturn void tasks_run100Hz(void)
{
    io_chimera_sleepTaskIfEnabled();

    // Setup tasks.
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

_Noreturn void tasks_runCanTx(void)
{
    io_chimera_sleepTaskIfEnabled();

    // Setup tasks.
    for (;;)
    {
        CanMsg tx_msg;
        io_can_popTxMsgFromQueue(&tx_msg);
        io_can_transmitMsgFromQueue(&tx_msg);
    }
}

_Noreturn void tasks_runCanRx(void)
{
    io_chimera_sleepTaskIfEnabled();

    // Setup tasks.
    for (;;)
    {
        CanMsg rx_msg;
        io_can_popRxMsgFromQueue(&rx_msg);

        JsonCanMsg jsoncan_rx_msg;
        io_jsoncan_copyFromCanMsg(&rx_msg, &jsoncan_rx_msg);
        io_canRx_updateRxTableWithMessage(&jsoncan_rx_msg);
    }
}

_Noreturn void tasks_run1kHz(void)
{
    io_chimera_sleepTaskIfEnabled();

    // Setup tasks.
    static const TickType_t period_ms = 1;
    WatchdogHandle         *watchdog  = hw_watchdog_allocateWatchdog();
    hw_watchdog_initWatchdog(watchdog, RTOS_TASK_1KHZ, period_ms);

    static uint32_t start_ticks = 0;
    start_ticks                 = osKernelGetTickCount();

    /* Infinite loop */
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

_Noreturn void tasks_run1Hz(void)
{
    io_chimera_sleepTaskIfEnabled();

    // Setup tasks.
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

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart == debug_uart.handle)
    {
        io_chimera_msgRxCallback();
    }
}
