#include "tasks.h"
#include "main.h"
#include "cmsis_os.h"

#include "app_utils.h"
#include "app_stateMachine.h"
#include "app_mainState.h"
#include "App_CanTx.h"
#include "App_CommitInfo.h"
#include "App_CanRx.h"
#include "App_CanAlerts.h"
#include "app_globals.h"
#include "app_sevenSegDisplays.h"
#include "app_avgPower.h"
#include "app_utils.h"
#include "app_units.h"

#include "Io_CanTx.h"
#include "Io_CanRx.h"
#include "io_time.h"
#include "io_led.h"
#include "io_switch.h"
#include "io_rgbLed.h"
#include "io_sevenSegDisplays.h"
#include "io_can.h"
#include "io_jsoncan.h"
#include "io_log.h"

#include "hw_hardFaultHandler.h"
#include "hw_bootup.h"
#include "hw_gpio.h"
#include "hw_can.h"
#include "hw_utils.h"
#include "hw_watchdog.h"
#include "hw_watchdogConfig.h"
#include "hw_stackWaterMarkConfig.h"

#define HEARTBEAT_MONITOR_TIMEOUT_PERIOD_MS 300U

void canTxQueueOverflowCallback(uint32_t overflow_count)
{
    App_CanTx_DIM_TxOverflowCount_Set(overflow_count);
    App_CanAlerts_DIM_Warning_TxOverflow_Set(true);
    BREAK_IF_DEBUGGER_CONNECTED();
}

void canRxQueueOverflowCallback(uint32_t overflow_count)
{
    App_CanTx_DIM_RxOverflowCount_Set(overflow_count);
    App_CanAlerts_DIM_Warning_RxOverflow_Set(true);
    BREAK_IF_DEBUGGER_CONNECTED();
}

extern CAN_HandleTypeDef hcan1;

static const CanConfig can_config = {
    .rx_msg_filter        = Io_CanRx_FilterMessageId,
    .tx_overflow_callback = canTxQueueOverflowCallback,
    .rx_overflow_callback = canRxQueueOverflowCallback,
};

static const BinaryLed imd_led   = { .gpio = {
                                       .port = IMD_LED_GPIO_Port,
                                       .pin  = IMD_LED_Pin,
                                   } };
static const BinaryLed bspd_led  = { .gpio = {
                                        .port = BSPD_LED_GPIO_Port,
                                        .pin  = BSPD_LED_Pin,
                                    } };
static const BinaryLed shdn_led  = { .gpio = {
                                        .port = SHDN_LED_GPIO_Port,
                                        .pin  = SHDN_LED_Pin,
                                    } };
static const BinaryLed drive_led = { .gpio = {
                                         .port = IGNTN_LED_GPIO_Port,
                                         .pin  = IGNTN_LED_Pin,
                                     } };

static const Switch start_switch = {
    .gpio = {
        .port = IGNTN_IN_GPIO_Port,
        .pin = IGNTN_IN_Pin,
    },
    .closed_state = true,
};
static const Switch aux_switch = {
    .gpio = {
        .port = AUX_IN_GPIO_Port,
        .pin = AUX_IN_Pin,
    },
    .closed_state = true,
};

static const RgbLed bms_status_led = {
    .red_gpio = {
        .port = BMS_RED_GPIO_Port,
        .pin = BMS_RED_Pin,
    },
    .green_gpio = {
        .port = BMS_GREEN_GPIO_Port,
        .pin = BMS_GREEN_Pin,
    },
    .blue_gpio = {
        .port = BMS_BLUE_GPIO_Port,
        .pin = BMS_BLUE_Pin,
    },
};
static const RgbLed dcm_status_led = {
    .red_gpio = {
        .port = DCM_RED_GPIO_Port,
        .pin = DCM_RED_Pin,
    },
    .green_gpio = {
        .port = DCM_GREEN_GPIO_Port,
        .pin = DCM_GREEN_Pin,
    },
    .blue_gpio = {
        .port = DCM_BLUE_GPIO_Port,
        .pin = DCM_BLUE_Pin,
    },
};
static const RgbLed fsm_status_led = {
    .red_gpio = {
        .port = FSM_RED_GPIO_Port,
        .pin = FSM_RED_Pin,
    },
    .green_gpio = {
        .port = FSM_GREEN_GPIO_Port,
        .pin = FSM_GREEN_Pin,
    },
    .blue_gpio = {
        .port = FSM_BLUE_GPIO_Port,
        .pin = FSM_BLUE_Pin,
    },
};
static const RgbLed pdm_status_led = {
    .red_gpio = {
        .port = PDM_RED_GPIO_Port,
        .pin = PDM_RED_Pin,
    },
    .green_gpio = {
        .port = PDM_GREEN_GPIO_Port,
        .pin = PDM_GREEN_Pin,
    },
    .blue_gpio = {
        .port = PDM_BLUE_GPIO_Port,
        .pin = PDM_BLUE_Pin,
    },
};
static const RgbLed dim_status_led = {
    .red_gpio = {
        .port = DIM_RED_GPIO_Port,
        .pin = DIM_RED_Pin,
    },
    .green_gpio = {
        .port = DIM_GREEN_GPIO_Port,
        .pin = DIM_GREEN_Pin,
    },
    .blue_gpio = {
        .port = DIM_BLUE_GPIO_Port,
        .pin = DIM_BLUE_Pin,
    },
};

static const SevenSegsConfig seven_segs_config = {
    .srck_gpio = {
        .port = SEVENSEGS_SRCK_GPIO_Port,
        .pin = SEVENSEGS_SRCK_Pin,
    },
    .rck_gpio = {
        .port = SEVENSEGS_RCK_GPIO_Port,
        .pin = SEVENSEGS_RCK_Pin,
    },
    .ser_out_gpio = {
        .port = SEVENSEGS_SEROUT_GPIO_Port,
        .pin = SEVENSEGS_SEROUT_Pin,
    },
    .dimming_gpio = {
        .port = SEVENSEGS_DIMMING_GPIO_Port,
        .pin = SEVENSEGS_DIMMING_Pin,
    },
};

static const GlobalsConfig globals_config = {
    .imd_led        = &imd_led,
    .bspd_led       = &bspd_led,
    .shdn_led       = &shdn_led,
    .drive_led      = &drive_led,
    .start_switch   = &start_switch,
    .aux_switch     = &aux_switch,
    .bms_status_led = &bms_status_led,
    .dcm_status_led = &dcm_status_led,
    .fsm_status_led = &fsm_status_led,
    .pdm_status_led = &pdm_status_led,
    .dim_status_led = &dim_status_led,
};

// config to forward can functions to shared heartbeat
// DIM rellies on all boards but itself
bool heartbeatMonitorChecklist[HEARTBEAT_BOARD_COUNT] = { [BMS_HEARTBEAT_BOARD] = true,
                                                          [DCM_HEARTBEAT_BOARD] = true,
                                                          [PDM_HEARTBEAT_BOARD] = true,
                                                          [FSM_HEARTBEAT_BOARD] = true,
                                                          [DIM_HEARTBEAT_BOARD] = false };

// heartbeatGetters - get heartbeat signals from other boards
bool (*heartbeatGetters[HEARTBEAT_BOARD_COUNT])() = { [BMS_HEARTBEAT_BOARD] = &App_CanRx_BMS_Heartbeat_Get,
                                                      [DCM_HEARTBEAT_BOARD] = &App_CanRx_DCM_Heartbeat_Get,
                                                      [PDM_HEARTBEAT_BOARD] = &App_CanRx_PDM_Heartbeat_Get,
                                                      [FSM_HEARTBEAT_BOARD] = &App_CanRx_FSM_Heartbeat_Get,
                                                      [DIM_HEARTBEAT_BOARD] = NULL };

// heartbeatUpdaters - update local CAN table with heartbeat status
void (*heartbeatUpdaters[HEARTBEAT_BOARD_COUNT])(bool) = { [BMS_HEARTBEAT_BOARD] = &App_CanRx_BMS_Heartbeat_Update,
                                                           [DCM_HEARTBEAT_BOARD] = &App_CanRx_DCM_Heartbeat_Update,
                                                           [PDM_HEARTBEAT_BOARD] = &App_CanRx_PDM_Heartbeat_Update,
                                                           [FSM_HEARTBEAT_BOARD] = &App_CanRx_FSM_Heartbeat_Update,
                                                           [DIM_HEARTBEAT_BOARD] = NULL };

// heartbeatFaultSetters - broadcast heartbeat faults over CAN
void (*heartbeatFaultSetters[HEARTBEAT_BOARD_COUNT])(bool) = {
    [BMS_HEARTBEAT_BOARD] = &App_CanAlerts_DIM_Fault_MissingBMSHeartbeat_Set,
    [DCM_HEARTBEAT_BOARD] = &App_CanAlerts_DIM_Fault_MissingDCMHeartbeat_Set,
    [PDM_HEARTBEAT_BOARD] = &App_CanAlerts_DIM_Fault_MissingPDMHeartbeat_Set,
    [FSM_HEARTBEAT_BOARD] = &App_CanAlerts_DIM_Fault_MissingFSMHeartbeat_Set,
    [DIM_HEARTBEAT_BOARD] = NULL
};

// heartbeatFaultGetters - gets fault statuses over CAN
bool (*heartbeatFaultGetters[HEARTBEAT_BOARD_COUNT])() = {
    [BMS_HEARTBEAT_BOARD] = &App_CanAlerts_DIM_Fault_MissingBMSHeartbeat_Get,
    [DCM_HEARTBEAT_BOARD] = &App_CanAlerts_DIM_Fault_MissingDCMHeartbeat_Get,
    [PDM_HEARTBEAT_BOARD] = &App_CanAlerts_DIM_Fault_MissingPDMHeartbeat_Get,
    [FSM_HEARTBEAT_BOARD] = &App_CanAlerts_DIM_Fault_MissingFSMHeartbeat_Get,
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

    SEGGER_SYSVIEW_Conf();
    LOG_INFO("DIM reset!");

    hw_hardFaultHandler_init();
    hw_can_init(&hcan1);
    hw_watchdog_init(hw_watchdogConfig_refresh, hw_watchdogConfig_timeoutCallback);

    Io_CanTx_Init(io_jsoncan_pushTxMsgToQueue);
    Io_CanTx_EnableMode(CAN_MODE_DEFAULT, true);
    io_sevenSegDisplays_init(&seven_segs_config);
    io_can_init(&can_config);

    App_CanTx_Init();
    App_CanRx_Init();

    app_heartbeatMonitor_init(
        HEARTBEAT_MONITOR_TIMEOUT_PERIOD_MS, heartbeatMonitorChecklist, heartbeatGetters, heartbeatUpdaters,
        &App_CanTx_DIM_Heartbeat_Set, heartbeatFaultSetters, heartbeatFaultGetters);
    app_sevenSegDisplays_init();
    app_avgPower_init();
    app_stateMachine_init(app_mainState_get());
    app_globals_init(&globals_config);

    // broadcast commit info
    App_CanTx_DIM_Hash_Set(GIT_COMMIT_HASH);
    App_CanTx_DIM_Clean_Set(GIT_COMMIT_CLEAN);
}

void tasks_run1Hz(void)
{
    static const TickType_t period_ms = 1000U;
    WatchdogHandle *        watchdog  = hw_watchdog_allocateWatchdog();
    hw_watchdog_initWatchdog(watchdog, RTOS_TASK_1HZ, period_ms);

    static uint32_t start_ticks = 0;
    start_ticks                 = osKernelGetTickCount();

    for (;;)
    {
        hw_stackWaterMarkConfig_check();
        app_stateMachine_tick1Hz();

        const bool debug_mode_enabled = App_CanRx_Debug_EnableDebugMode_Get();
        Io_CanTx_EnableMode(CAN_MODE_DEBUG, debug_mode_enabled);
        Io_CanTx_Enqueue1HzMsgs();

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
    WatchdogHandle *        watchdog  = hw_watchdog_allocateWatchdog();
    hw_watchdog_initWatchdog(watchdog, RTOS_TASK_100HZ, period_ms);

    static uint32_t start_ticks = 0;
    start_ticks                 = osKernelGetTickCount();

    for (;;)
    {
        app_stateMachine_tick100Hz();
        Io_CanTx_Enqueue100HzMsgs();

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
    WatchdogHandle *        watchdog  = hw_watchdog_allocateWatchdog();
    hw_watchdog_initWatchdog(watchdog, RTOS_TASK_1KHZ, period_ms);

    static uint32_t start_ticks = 0;
    start_ticks                 = osKernelGetTickCount();

    /* Infinite loop */
    for (;;)
    {
        // Check in for timeouts for all RTOS tasks
        hw_watchdog_checkForTimeouts();
        const uint32_t task_start_ms = TICK_TO_MS(osKernelGetTickCount());

        Io_CanTx_EnqueueOtherPeriodicMsgs(task_start_ms);

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
        Io_CanRx_UpdateRxTableWithMessage(&jsoncan_rx_msg);
    }
}
