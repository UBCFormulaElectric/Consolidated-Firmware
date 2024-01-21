#include "tasks.h"
#include "main.h"
#include "cmsis_os.h"

#include "App_CanTx.h"
#include "App_CanRx.h"
#include "App_CanAlerts.h"
#include "app_utils.h"
#include "app_units.h"
#include "app_stateMachine.h"
#include "states/app_initState.h"
#include "configs/App_HeartbeatMonitorConfig.h"
#include "App_CommitInfo.h"
#include "app_globals.h"

#include "Io_CanTx.h"
#include "Io_CanRx.h"
#include "Io_SharedSoftwareWatchdog.h"
#include "io_stackWaterMark.h"
#include "io_watchdogConfig.h"
#include "io_lowVoltageBattery.h"
#include "io_efuse.h"
#include "hw_adc.h"
#include "hw_hardFaultHandler.h"
#include "io_can.h"
#include "io_jsoncan.h"
#include "io_time.h"
#include "io_log.h"
#include "hw_bootup.h"
#include "hw_can.h"
#include "hw_utils.h"

static void canRxQueueOverflowCallBack(uint32_t overflow_count)
{
    App_CanTx_PDM_RxOverflowCount_Set(overflow_count);
    App_CanAlerts_PDM_Warning_RxOverflow_Set(true);
}

static void canTxQueueOverflowCallBack(uint32_t overflow_count)
{
    App_CanTx_PDM_TxOverflowCount_Set(overflow_count);
    App_CanAlerts_PDM_Warning_TxOverflow_Set(true);
}

extern ADC_HandleTypeDef hadc1;
extern CAN_HandleTypeDef hcan1;
extern TIM_HandleTypeDef htim3;

static const CanConfig can_config = {
    .rx_msg_filter        = Io_CanRx_FilterMessageId,
    .tx_overflow_callback = canTxQueueOverflowCallBack,
    .rx_overflow_callback = canRxQueueOverflowCallBack,
};

struct HeartbeatMonitor *heartbeat_monitor;

static const LvBatteryConfig lv_battery_config = {
    .lt3650_charger_fault_gpio = {
        .port = N_CHRG_FAULT_GPIO_Port,
        .pin = N_CHRG_FAULT_Pin
    },
    .ltc3786_boost_fault_gpio = {
        .port = PGOOD_GPIO_Port,
        .pin = PGOOD_Pin,
    },
    .vbat_vsense_adc_channel = ADC_1_CHANNEL_10,
    .boost_vsense_adc_channel = ADC_1_CHANNEL_12,
    .acc_vsense_adc_channel = ADC_1_CHANNEL_11
};

static const EfuseConfig efuse_configs[NUM_EFUSE_CHANNELS] = {
    [EFUSE_CHANNEL_AIR] = {
        .enable_gpio = {
            .port = EN_AIR_GPIO_Port,
            .pin = EN_AIR_Pin
        },
        .stby_reset_gpio = {
            .port = FR_STBY_AIR_LVPWR_GPIO_Port,
            .pin = FR_STBY_AIR_LVPWR_Pin,
        },
        .cur_sns_adc_channel = ADC_1_CHANNEL_9,
    },
    [EFUSE_CHANNEL_LVPWR] = {
        .enable_gpio = {
            .port = EN_LVPWR_GPIO_Port,
            .pin = EN_LVPWR_Pin
        },
        .stby_reset_gpio = {
            .port = FR_STBY_AIR_LVPWR_GPIO_Port,
            .pin = FR_STBY_AIR_LVPWR_Pin,
        },
        .cur_sns_adc_channel = ADC_1_CHANNEL_8,
    },
    [EFUSE_CHANNEL_EMETER] = {
        .enable_gpio = {
            .port = EN_EMETER_GPIO_Port,
            .pin = EN_EMETER_Pin
        },
        .stby_reset_gpio = {
            .port = FR_STBY_EMETER_AUX_GPIO_Port,
            .pin = FR_STBY_EMETER_AUX_Pin,
        },
        .cur_sns_adc_channel = ADC_1_CHANNEL_15,
    },
    [EFUSE_CHANNEL_AUX] = {
        .enable_gpio = {
            .port = EN_AUX_GPIO_Port,
            .pin = EN_AUX_Pin
        },
        .stby_reset_gpio = {
            .port = FR_STBY_EMETER_AUX_GPIO_Port,
            .pin = FR_STBY_EMETER_AUX_Pin,
        },
        .cur_sns_adc_channel = ADC_1_CHANNEL_14,
    },
    [EFUSE_CHANNEL_DRS] = {
        .enable_gpio = {
            .port = EN_DRS_GPIO_Port,
            .pin = EN_DRS_Pin
        },
        .stby_reset_gpio = {
            .port = FR_STBY_DRS_FAN_GPIO_Port,
            .pin = FR_STBY_DRS_FAN_Pin,
        },
        .cur_sns_adc_channel = ADC_1_CHANNEL_7,
    },
    [EFUSE_CHANNEL_FAN] = {
        .enable_gpio = {
            .port = EN_FAN_GPIO_Port,
            .pin = EN_FAN_Pin
        },
        .stby_reset_gpio = {
            .port = FR_STBY_DRS_FAN_GPIO_Port,
            .pin = FR_STBY_DRS_FAN_Pin,
        },
        .cur_sns_adc_channel = ADC_1_CHANNEL_6,
    },
    [EFUSE_CHANNEL_DI_LHS] = {
        .enable_gpio = {
            .port = EN_DI_LHS_GPIO_Port,
            .pin = EN_DI_LHS_Pin
        },
        .stby_reset_gpio = {
            .port = FR_STBY_DIS_GPIO_Port,
            .pin = FR_STBY_DIS_Pin,
        },
        .cur_sns_adc_channel = ADC_1_CHANNEL_5,
    },
    [EFUSE_CHANNEL_DI_RHS] = {
        .enable_gpio = {
            .port = EN_DI_RHS_GPIO_Port,
            .pin = EN_DI_RHS_Pin
        },
        .stby_reset_gpio = {
            .port = FR_STBY_DIS_GPIO_Port,
            .pin = FR_STBY_DIS_Pin,
        },
        .cur_sns_adc_channel = ADC_1_CHANNEL_4,
    }
};

// config to forward can functions to shared heartbeat
// PDM rellies on BMS
bool heartbeatMonitorChecklist[HEARTBEAT_BOARD_COUNT] = { [BMS_HEARTBEAT_BOARD] = true,
                                                          [DCM_HEARTBEAT_BOARD] = false,
                                                          [PDM_HEARTBEAT_BOARD] = false,
                                                          [FSM_HEARTBEAT_BOARD] = false,
                                                          [DIM_HEARTBEAT_BOARD] = false };

// heartbeatGetters - get heartbeat signals from other boards
bool (*heartbeatGetters[HEARTBEAT_BOARD_COUNT])() = { [BMS_HEARTBEAT_BOARD] = &App_CanRx_BMS_Heartbeat_Get,
                                                      [DCM_HEARTBEAT_BOARD] = NULL,
                                                      [PDM_HEARTBEAT_BOARD] = NULL,
                                                      [FSM_HEARTBEAT_BOARD] = NULL,
                                                      [DIM_HEARTBEAT_BOARD] = NULL };

// heartbeatUpdaters - update local CAN table with heartbeat status
void (*heartbeatUpdaters[HEARTBEAT_BOARD_COUNT])(bool) = { [BMS_HEARTBEAT_BOARD] = &App_CanRx_BMS_Heartbeat_Update,
                                                           [DCM_HEARTBEAT_BOARD] = NULL,
                                                           [PDM_HEARTBEAT_BOARD] = NULL,
                                                           [FSM_HEARTBEAT_BOARD] = NULL,
                                                           [DIM_HEARTBEAT_BOARD] = NULL };

// heartbeatUpdaters - update local CAN table with heartbeat status
void (*heartbeatFaultSetters[HEARTBEAT_BOARD_COUNT])(bool) = {
    [BMS_HEARTBEAT_BOARD] = &App_CanAlerts_PDM_Fault_MissingBMSHeartbeat_Set,
    [DCM_HEARTBEAT_BOARD] = NULL,
    [PDM_HEARTBEAT_BOARD] = NULL,
    [FSM_HEARTBEAT_BOARD] = NULL,
    [DIM_HEARTBEAT_BOARD] = NULL
};

// heartbeatFaultGetters - gets fault statuses over CAN
bool (*heartbeatFaultGetters[HEARTBEAT_BOARD_COUNT])() = {
    [BMS_HEARTBEAT_BOARD] = &App_CanAlerts_PDM_Fault_MissingBMSHeartbeat_Get,
    [DCM_HEARTBEAT_BOARD] = NULL,
    [PDM_HEARTBEAT_BOARD] = NULL,
    [FSM_HEARTBEAT_BOARD] = NULL,
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

    // Configure and initialize SEGGER SystemView.
    SEGGER_SYSVIEW_Conf();
    LOG_INFO("PDM reset!");

    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)hw_adc_getRawValuesBuffer(), hadc1.Init.NbrOfConversion);
    HAL_TIM_Base_Start(&htim3);

    hw_hardFaultHandler_init();
    hw_can_init(&hcan1);

    Io_SharedSoftwareWatchdog_Init(io_watchdogConfig_refresh, io_watchdogConfig_timeoutCallback);
    Io_CanTx_Init(io_jsoncan_pushTxMsgToQueue);
    Io_CanTx_EnableMode(CAN_MODE_DEFAULT, true);
    io_can_init(&can_config);

    io_lowVoltageBattery_init(&lv_battery_config);
    io_efuse_init(efuse_configs);

    App_CanTx_Init();
    App_CanRx_Init();

    app_stateMachine_init(app_initState_get());

    heartbeat_monitor = App_SharedHeartbeatMonitor_Create(
        io_time_getCurrentMs, HEARTBEAT_MONITOR_TIMEOUT_PERIOD_MS, heartbeatMonitorChecklist, heartbeatGetters,
        heartbeatUpdaters, &App_CanTx_PDM_Heartbeat_Set, heartbeatFaultSetters, heartbeatFaultGetters);
    globals->heartbeat_monitor = heartbeat_monitor;

    io_efuse_setChannel(EFUSE_CHANNEL_AIR, true);
    io_efuse_setChannel(EFUSE_CHANNEL_LVPWR, true);
    io_efuse_setChannel(EFUSE_CHANNEL_EMETER, true);
    io_efuse_setChannel(EFUSE_CHANNEL_AUX, true);
    io_efuse_setChannel(EFUSE_CHANNEL_DRS, true);
    io_efuse_setChannel(EFUSE_CHANNEL_FAN, true);
    io_efuse_setChannel(EFUSE_CHANNEL_DI_LHS, true);
    io_efuse_setChannel(EFUSE_CHANNEL_DI_RHS, true);

    // broadcast commit info
    App_CanTx_PDM_Hash_Set(GIT_COMMIT_HASH);
    App_CanTx_PDM_Clean_Set(GIT_COMMIT_CLEAN);
}

void tasks_run1Hz(void)
{
    static const TickType_t  period_ms = 1000U;
    SoftwareWatchdogHandle_t watchdog  = Io_SharedSoftwareWatchdog_AllocateWatchdog();
    Io_SharedSoftwareWatchdog_InitWatchdog(watchdog, RTOS_TASK_1HZ, period_ms);

    static uint32_t start_ticks = 0;
    start_ticks                 = osKernelGetTickCount();

    /* Infinite loop */
    for (;;)
    {
        io_stackWaterMark_check();
        app_stateMachine_tick1Hz();

        const bool debug_mode_enabled = App_CanRx_Debug_EnableDebugMode_Get();
        Io_CanTx_EnableMode(CAN_MODE_DEBUG, debug_mode_enabled);
        Io_CanTx_Enqueue1HzMsgs();

        // Watchdog check-in must be the last function called before putting the
        // task to sleep.
        Io_SharedSoftwareWatchdog_CheckInWatchdog(watchdog);

        start_ticks += period_ms;
        osDelayUntil(start_ticks);
    }
}

void tasks_run100Hz(void)
{
    static const TickType_t  period_ms = 10U;
    SoftwareWatchdogHandle_t watchdog  = Io_SharedSoftwareWatchdog_AllocateWatchdog();
    Io_SharedSoftwareWatchdog_InitWatchdog(watchdog, RTOS_TASK_100HZ, period_ms);

    static uint32_t start_ticks = 0;
    start_ticks                 = osKernelGetTickCount();

    /* Infinite loop */
    for (;;)
    {
        const uint32_t start_time_ms = osKernelGetTickCount();

        app_stateMachine_tick100Hz();
        Io_CanTx_Enqueue100HzMsgs();

        // Watchdog check-in must be the last function called before putting the
        // task to sleep.
        Io_SharedSoftwareWatchdog_CheckInWatchdog(watchdog);

        start_ticks += period_ms;
        osDelayUntil(start_ticks);
    }
}

void tasks_run1kHz(void)
{
    static const TickType_t  period_ms = 1U;
    SoftwareWatchdogHandle_t watchdog  = Io_SharedSoftwareWatchdog_AllocateWatchdog();
    Io_SharedSoftwareWatchdog_InitWatchdog(watchdog, RTOS_TASK_1KHZ, period_ms);

    static uint32_t start_ticks = 0;
    start_ticks                 = osKernelGetTickCount();

    for (;;)
    {
        const uint32_t start_time_ms = osKernelGetTickCount();

        Io_SharedSoftwareWatchdog_CheckForTimeouts();
        const uint32_t task_start_ms = TICK_TO_MS(osKernelGetTickCount());

        Io_CanTx_EnqueueOtherPeriodicMsgs(task_start_ms);

        // Watchdog check-in must be the last function called before putting the
        // task to sleep. Prevent check in if the elapsed period is greater or
        // equal to the period ms
        if ((TICK_TO_MS(osKernelGetTickCount()) - task_start_ms) <= period_ms)
        {
            Io_SharedSoftwareWatchdog_CheckInWatchdog(watchdog);
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
