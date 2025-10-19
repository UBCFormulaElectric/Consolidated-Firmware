#include "tasks.h"
#include "main.h"
#include "cmsis_os.h"
#include <assert.h>
// protobufs
#include "shared.pb.h"
#include "CRIT.pb.h"
// app
#include "app_heartbeatMonitors.h"
#include "app_stateMachine.h"
#include "app_mainState.h"
#include "app_stackWaterMarks.h"
#include "utils.h"
// io
#include "io_log.h"
#include "io_chimera.h"
#include "io_led.h"
#include "io_switch.h"
#include "io_rgbLed.h"
#include "io_critShdn.h"
#include "io_leds.h"
#include "io_switches.h"
#include "io_canQueue.h"
#include "io_bootHandler.h"
// can
#include "app_jsoncan.h"
#include "io_canRx.h"
#include "io_driveMode.h"
#include "app_canTx.h"
#include "app_canRx.h"
#include "app_canAlerts.h"
#include "app_commitInfo.h"
// hw
#include "hw_gpios.h"
#include "hw_adcs.h"
#include "hw_uart.h"
#include "hw_utils.h"
#include "hw_bootup.h"
#include "hw_watchdog.h"
#include "hw_watchdogConfig.h"
#include "hw_hardFaultHandler.h"
#include "hw_can.h"

#include <assert.h>

static CanHandle can = { .hcan = &hcan1, .bus_num = 1, .receive_callback = io_canQueue_pushRx };

const CanHandle *hw_can_getHandle(const CAN_HandleTypeDef *hcan)
{
    assert(hcan == can.hcan);
    return &can;
}
void canTxQueueOverflowCallback(uint32_t overflow_count)
{
    app_canTx_CRIT_TxOverflowCount_set(overflow_count);
    app_canAlerts_CRIT_Warning_TxOverflow_set(true);
}

void canRxQueueOverflowCallback(uint32_t overflow_count)
{
    app_canTx_CRIT_RxOverflowCount_set(overflow_count);
    app_canAlerts_CRIT_Warning_RxOverflow_set(true);
}

void canTxQueueOverflowClearCallback(void)
{
    app_canAlerts_CRIT_Warning_TxOverflow_set(false);
}

void canRxQueueOverflowClearCallback(void)
{
    app_canAlerts_CRIT_Warning_RxOverflow_set(false);
}

static const BinaryLed imd_led       = { .gpio = &imd_r_pin };
static const BinaryLed bspd_led      = { .gpio = &bspd_r_pin };
static const BinaryLed ams_led       = { .gpio = &ams_r_pin };
static const BinaryLed start_led     = { .gpio = &start_led_pin };
static const BinaryLed regen_led     = { .gpio = &regen_led_pin };
static const BinaryLed torquevec_led = { .gpio = &torque_vectoring_led_pin };

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

const AdcChannel *id_to_adc[] = {
    [CRIT_AdcNetName_REGEN_3V3] = &regen,
};

const Gpio *n_chimera_gpio = &n_chimera_pin;

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

static const CritShdnConfig crit_shdn_pin_config = { .cockpit_estop_gpio  = &shdn_sen_pin,
                                                     .inertia_sen_ok_gpio = &inertia_sen_pin };

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
    LOG_INFO("CRIT reset!");

    // Start DMA/TIM3 for the ADC.
    hw_adcs_chipsInit();

    io_chimera_init(GpioNetName_crit_net_name_tag, AdcNetName_crit_net_name_tag);

    // Re-enable watchdog.
    __HAL_DBGMCU_FREEZE_IWDG();

    hw_can_init(&can);
    hw_watchdog_init(hw_watchdogConfig_refresh, hw_watchdogConfig_timeoutCallback);

    io_canTx_init(jsoncan_transmit);
    io_canTx_enableMode_Can(CAN_MODE_DEFAULT, true);
    io_critShdn_init(&crit_shdn_pin_config);
    io_canQueue_init();

    io_led_init(&led_config);
    io_switches_init(&switch_config);
    io_driveMode_init(&drive_mode);

    app_canTx_init();
    app_canRx_init();

    app_heartbeatMonitor_init(&hb_monitor);
    app_stateMachine_init(app_mainState_get());

    // broadcast commit info
    app_canTx_CRIT_Hash_set(GIT_COMMIT_HASH);
    app_canTx_CRIT_Clean_set(GIT_COMMIT_CLEAN);
    app_canTx_CRIT_Heartbeat_set(true);
}

void tasks_deinit(void)
{
    HAL_TIM_Base_Stop_IT(&htim3);
    HAL_TIM_Base_DeInit(&htim3);

    HAL_UART_Abort_IT(&huart2);
    HAL_UART_DeInit(&huart2);

    HAL_DMA_Abort_IT(&hdma_adc1);
    HAL_DMA_DeInit(&hdma_adc1);

    HAL_ADC_Stop_IT(&hadc1);
    HAL_ADC_DeInit(&hadc1);
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
        CanMsg tx_msg = io_canQueue_popTx();
        LOG_IF_ERR(hw_can_transmit(&can, &tx_msg));
    }
}

_Noreturn void tasks_runCanRx(void)
{
    io_chimera_sleepTaskIfEnabled();

    // Setup tasks.
    for (;;)
    {
        CanMsg     rx_msg         = io_canQueue_popRx(&rx_msg);
        JsonCanMsg jsoncan_rx_msg = app_jsoncan_copyFromCanMsg(&rx_msg);

        io_bootHandler_processBootRequest(&rx_msg);
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
