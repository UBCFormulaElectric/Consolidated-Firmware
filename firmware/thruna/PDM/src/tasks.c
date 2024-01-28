#include "tasks.h"
#include "main.h"
#include "cmsis_os.h"

#include "app_canTx.h"
#include "app_canRx.h"
#include "app_canAlerts.h"
#include "app_utils.h"
#include "app_units.h"
#include "app_stateMachine.h"
#include "app_heartbeatMonitor.h"
#include "states/app_initState.h"
#include "app_commitInfo.h"

#include "io_canTx.h"
#include "io_canRx.h"
#include "io_lowVoltageBattery.h"
#include "io_efuse.h"
#include "io_can.h"
#include "io_jsoncan.h"
#include "io_time.h"
#include "io_log.h"

#include "hw_adc.h"
#include "hw_hardFaultHandler.h"
#include "hw_bootup.h"
#include "hw_can.h"
#include "hw_utils.h"
#include "hw_watchdog.h"
#include "hw_watchdogConfig.h"
#include "hw_stackWaterMark.h"
#include "hw_stackWaterMarkConfig.h"

static void canRxQueueOverflowCallBack(uint32_t overflow_count)
{
    app_canTx_PDM_RxOverflowCount_set(overflow_count);
    app_canAlerts_PDM_Warning_RxOverflow_set(true);
}

static void canTxQueueOverflowCallBack(uint32_t overflow_count)
{
    app_canTx_PDM_TxOverflowCount_set(overflow_count);
    app_canAlerts_PDM_Warning_TxOverflow_set(true);
}

extern ADC_HandleTypeDef hadc1;
extern CAN_HandleTypeDef hcan1;
extern TIM_HandleTypeDef htim3;

static const CanConfig can_config = {
    .rx_msg_filter        = io_canRx_filterMessageId,
    .tx_overflow_callback = canTxQueueOverflowCallBack,
    .rx_overflow_callback = canRxQueueOverflowCallBack,
};

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
bool (*heartbeatGetters[HEARTBEAT_BOARD_COUNT])() = { [BMS_HEARTBEAT_BOARD] = &app_canRx_BMS_Heartbeat_get,
                                                      [DCM_HEARTBEAT_BOARD] = NULL,
                                                      [PDM_HEARTBEAT_BOARD] = NULL,
                                                      [FSM_HEARTBEAT_BOARD] = NULL,
                                                      [DIM_HEARTBEAT_BOARD] = NULL };

// heartbeatUpdaters - update local CAN table with heartbeat status
void (*heartbeatUpdaters[HEARTBEAT_BOARD_COUNT])(bool) = { [BMS_HEARTBEAT_BOARD] = &app_canRx_BMS_Heartbeat_update,
                                                           [DCM_HEARTBEAT_BOARD] = NULL,
                                                           [PDM_HEARTBEAT_BOARD] = NULL,
                                                           [FSM_HEARTBEAT_BOARD] = NULL,
                                                           [DIM_HEARTBEAT_BOARD] = NULL };

// heartbeatUpdaters - update local CAN table with heartbeat status
void (*heartbeatFaultSetters[HEARTBEAT_BOARD_COUNT])(bool) = {
    [BMS_HEARTBEAT_BOARD] = &app_canAlerts_PDM_Fault_MissingBMSHeartbeat_set,
    [DCM_HEARTBEAT_BOARD] = NULL,
    [PDM_HEARTBEAT_BOARD] = NULL,
    [FSM_HEARTBEAT_BOARD] = NULL,
    [DIM_HEARTBEAT_BOARD] = NULL
};

// heartbeatFaultGetters - gets fault statuses over CAN
bool (*heartbeatFaultGetters[HEARTBEAT_BOARD_COUNT])() = {
    [BMS_HEARTBEAT_BOARD] = &app_canAlerts_PDM_Fault_MissingBMSHeartbeat_get,
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
    hw_watchdog_init(hw_watchdogConfig_refresh, hw_watchdogConfig_timeoutCallback);

    io_canTx_init(io_jsoncan_pushTxMsgToQueue);
    io_canTx_enableMode(CAN_MODE_DEFAULT, true);
    io_can_init(&can_config);

    io_lowVoltageBattery_init(&lv_battery_config);
    io_efuse_init(efuse_configs);

    app_canTx_init();
    app_canRx_init();

    app_heartbeatMonitor_init(
        HEARTBEAT_MONITOR_TIMEOUT_PERIOD_MS, heartbeatMonitorChecklist, heartbeatGetters, heartbeatUpdaters,
        &app_canTx_PDM_Heartbeat_set, heartbeatFaultSetters, heartbeatFaultGetters);
    app_stateMachine_init(app_initState_get());

    io_efuse_setChannel(EFUSE_CHANNEL_AIR, true);
    io_efuse_setChannel(EFUSE_CHANNEL_LVPWR, true);
    io_efuse_setChannel(EFUSE_CHANNEL_EMETER, true);
    io_efuse_setChannel(EFUSE_CHANNEL_AUX, true);
    io_efuse_setChannel(EFUSE_CHANNEL_DRS, true);
    io_efuse_setChannel(EFUSE_CHANNEL_FAN, true);
    io_efuse_setChannel(EFUSE_CHANNEL_DI_LHS, true);
    io_efuse_setChannel(EFUSE_CHANNEL_DI_RHS, true);

    // broadcast commit info
    app_canTx_PDM_Hash_set(GIT_COMMIT_HASH);
    app_canTx_PDM_Clean_set(GIT_COMMIT_CLEAN);
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
    static const TickType_t period_ms = 10U;
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
