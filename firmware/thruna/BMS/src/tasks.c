#include "tasks.h"
#include "main.h"
#include "cmsis_os.h"

#include "App_CanTx.h"
#include "App_CanRx.h"
#include "App_CanAlerts.h"
#include "App_CommitInfo.h"
#include "app_timer.h"
#include "app_thermistors.h"
#include "app_accumulator.h"
#include "app_soc.h"
#include "app_globals.h"
#include "states/app_allStates.h"
#include "states/app_initState.h"
#include "states/app_inverterOnState.h"
#include "app_stateMachine.h"
#include "configs/App_HeartbeatMonitorConfig.h"

#include "Io_SharedSoftwareWatchdog.h"
#include "Io_SharedSpi.h"
#include "Io_CanTx.h"
#include "Io_CanRx.h"
#include "io_jsoncan.h"
#include "io_time.h"
#include "io_can.h"
#include "io_airs.h"
#include "io_charger.h"
#include "io_eeprom.h"
#include "io_faultLatch.h"
#include "io_imd.h"
#include "ltc6813/io_ltc6813Shared.h"
#include "io_stackWaterMark.h"
#include "io_thermistors.h"
#include "io_tractiveSystem.h"
#include "io_watchdogConfig.h"
#include "io_log.h"

#include "hw_can.h"
#include "hw_adc.h"
#include "hw_hardFaultHandler.h"
#include "hw_bootup.h"
#include "hw_utils.h"

static void canRxQueueOverflowCallBack(uint32_t overflow_count)
{
    App_CanTx_BMS_RxOverflowCount_Set(overflow_count);
    App_CanAlerts_BMS_Warning_RxOverflow_Set(true);
}

static void canTxQueueOverflowCallBack(uint32_t overflow_count)
{
    App_CanTx_BMS_TxOverflowCount_Set(overflow_count);
    App_CanAlerts_BMS_Warning_TxOverflow_Set(true);
}

extern ADC_HandleTypeDef  hadc1;
extern CAN_HandleTypeDef  hcan1;
extern IWDG_HandleTypeDef hiwdg;
extern SPI_HandleTypeDef  hspi1;
extern TIM_HandleTypeDef  htim1;
extern TIM_HandleTypeDef  htim13;

static const CanConfig can_config = {
    .rx_msg_filter        = Io_CanRx_FilterMessageId,
    .tx_overflow_callback = canTxQueueOverflowCallBack,
    .rx_overflow_callback = canRxQueueOverflowCallBack,
};

struct HeartbeatMonitor *hb_monitor;

static const Charger charger_config  = { .enable_gpio = {
                                       .port = CHRG_EN_3V3_GPIO_Port,
                                       .pin  = CHRG_EN_3V3_Pin,
                                   },
                                   .connected_gpio = {
                                       .port = CHRG_STATE_3V3_GPIO_Port,
                                       .pin  = CHRG_STATE_3V3_Pin,
                                   },
                                   .faulted_gpio = {
                                       .port = CHRG_FLT_3V3_GPIO_Port,
                                       .pin  = CHRG_FLT_3V3_Pin,
                                   }};

static const ThermistorsConfig thermistors_config = {.mux_0_gpio = {
                                       .port = AUX_TSENSE_MUX0_GPIO_Port,
                                       .pin  = AUX_TSENSE_MUX0_Pin,
                                   },
                                   .mux_1_gpio = {
                                       .port = AUX_TSENSE_MUX1_GPIO_Port,
                                       .pin  = AUX_TSENSE_MUX1_Pin,
                                   },
                                   .mux_2_gpio = {
                                       .port = AUX_TSENSE_MUX2_GPIO_Port,
                                       .pin  = AUX_TSENSE_MUX2_Pin,
                                   },
                                   .mux_3_gpio = {
                                       .port = AUX_TSENSE_MUX3_GPIO_Port,
                                       .pin  = AUX_TSENSE_MUX3_Pin,
                                   },
                                   .thermistor_adc_channel = ADC1_CHANNEL_14
                                   };

static const AirsConfig airs_config = { .air_p_gpio = {
                                       .port = AIR_P_EN_GPIO_Port,
                                       .pin  = AIR_P_EN_Pin,
                                   },
                                   .air_n_gpio = {
                                       .port = AIR_N_STATUS_GPIO_Port,
                                       .pin  = AIR_N_STATUS_Pin,
                                   },
                                   .precharge_gpio = {
                                       .port = PRECHARGE_EN_GPIO_Port,
                                       .pin  = PRECHARGE_EN_Pin,
                                   },
                                   .loop_isense_channel = ADC1_CHANNEL_3
};

static const TractiveSystemConfig ts_config = { .ts_vsense_p_channel        = ADC1_CHANNEL_10,
                                                .ts_vsense_n_channel        = ADC1_CHANNEL_11,
                                                .ts_isense_high_res_channel = ADC1_CHANNEL_8,
                                                .ts_isense_low_res_channel  = ADC1_CHANNEL_9

};

static const FaultLatch bms_ok_latch = {
    .current_status_gpio = {
        .port = BMS_OK_3V3_GPIO_Port,
        .pin = BMS_OK_3V3_Pin,
    },
    .latch_status_gpio = {
        .port = BMS_LATCH_GPIO_Port,
        .pin = BMS_LATCH_Pin,
    },
    .read_only = false,
};

static const FaultLatch imd_ok_latch = {
    .current_status_gpio = {
        .port = IMD_OK_3V3_GPIO_Port,
        .pin = IMD_OK_3V3_Pin,
    },
    .latch_status_gpio = {
        .port = IMD_LATCH_GPIO_Port,
        .pin = IMD_LATCH_Pin,
    },
    .read_only = true,
};

static const FaultLatch bspd_ok_latch = {
    .current_status_gpio = {
        .port = BSPD_OK_3V3_GPIO_Port,
        .pin = BSPD_OK_3V3_Pin,
    },
    .latch_status_gpio = {
        .port = BSPD_LATCH_GPIO_Port,
        .pin = BSPD_LATCH_Pin,
    },
    .read_only = true,
};

static const GlobalsConfig globals_config = {
    .bms_ok_latch  = &bms_ok_latch,
    .imd_ok_latch  = &imd_ok_latch,
    .bspd_ok_latch = &bspd_ok_latch,
};

struct Imd *             imd;
struct HeartbeatMonitor *heartbeat_monitor;
struct RgbLedSequence *  rgb_led_sequence;
struct Charger *         charger;
struct OkStatus *        bms_ok;
struct OkStatus *        imd_ok;
struct OkStatus *        bspd_ok;
struct SocStats *        soc_stats;
struct Accumulator *     accumulator;
struct CellMonitors *    cell_monitors;
struct Airs *            airs;
struct PrechargeRelay *  precharge_relay;
struct TractiveSystem *  ts;
struct Clock *           clock;
struct Eeprom *          eeprom;

// config to forward can functions to shared heartbeat
// BMS rellies on DCM, PDM, and FSM
bool heartbeatMonitorChecklist[HEARTBEAT_BOARD_COUNT] = { [BMS_HEARTBEAT_BOARD] = false,
                                                          [DCM_HEARTBEAT_BOARD] = true,
                                                          [PDM_HEARTBEAT_BOARD] = true,
                                                          [FSM_HEARTBEAT_BOARD] = true,
                                                          [DIM_HEARTBEAT_BOARD] = false };
// heartbeatGetters - get heartbeat signals from other boards
bool (*heartbeatGetters[HEARTBEAT_BOARD_COUNT])() = { [BMS_HEARTBEAT_BOARD] = NULL,
                                                      [DCM_HEARTBEAT_BOARD] = &App_CanRx_DCM_Heartbeat_Get,
                                                      [PDM_HEARTBEAT_BOARD] = &App_CanRx_PDM_Heartbeat_Get,
                                                      [FSM_HEARTBEAT_BOARD] = &App_CanRx_FSM_Heartbeat_Get,
                                                      [DIM_HEARTBEAT_BOARD] = NULL };

// heartbeatUpdaters - update local CAN table with heartbeat status
void (*heartbeatUpdaters[HEARTBEAT_BOARD_COUNT])(bool) = { [BMS_HEARTBEAT_BOARD] = NULL,
                                                           [DCM_HEARTBEAT_BOARD] = &App_CanRx_DCM_Heartbeat_Update,
                                                           [PDM_HEARTBEAT_BOARD] = &App_CanRx_PDM_Heartbeat_Update,
                                                           [FSM_HEARTBEAT_BOARD] = &App_CanRx_FSM_Heartbeat_Update,
                                                           [DIM_HEARTBEAT_BOARD] = NULL };

// heartbeatFaultSetters - broadcast heartbeat faults over CAN
void (*heartbeatFaultSetters[HEARTBEAT_BOARD_COUNT])(bool) = {
    [BMS_HEARTBEAT_BOARD] = NULL,
    [DCM_HEARTBEAT_BOARD] = &App_CanAlerts_BMS_Fault_MissingDCMHeartbeat_Set,
    [PDM_HEARTBEAT_BOARD] = &App_CanAlerts_BMS_Fault_MissingPDMHeartbeat_Set,
    [FSM_HEARTBEAT_BOARD] = &App_CanAlerts_BMS_Fault_MissingFSMHeartbeat_Set,
    [DIM_HEARTBEAT_BOARD] = NULL
};

// heartbeatFaultGetters - gets fault statuses over CAN
bool (*heartbeatFaultGetters[HEARTBEAT_BOARD_COUNT])() = {
    [BMS_HEARTBEAT_BOARD] = NULL,
    [DCM_HEARTBEAT_BOARD] = &App_CanAlerts_BMS_Fault_MissingDCMHeartbeat_Get,
    [PDM_HEARTBEAT_BOARD] = &App_CanAlerts_BMS_Fault_MissingPDMHeartbeat_Get,
    [FSM_HEARTBEAT_BOARD] = &App_CanAlerts_BMS_Fault_MissingFSMHeartbeat_Get,
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
    LOG_INFO("BMS reset!");

    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)hw_adc_getRawValuesBuffer(), hadc1.Init.NbrOfConversion);
    HAL_TIM_Base_Start(&htim13);

    hw_hardFaultHandler_init();
    hw_can_init(&hcan1);

    Io_SharedSoftwareWatchdog_Init(io_watchdogConfig_refresh, io_watchdogConfig_timeoutCallback);
    Io_CanTx_Init(io_jsoncan_pushTxMsgToQueue);
    Io_CanTx_EnableMode(CAN_MODE_DEFAULT, true);
    io_can_init(&can_config);

    io_tractiveSystem_init(&ts_config);
    io_thermistors_init(&thermistors_config);
    io_ltc6813Shared_init(&hspi1);
    io_airs_init(&airs_config);
    io_imd_init();
    io_charger_init(&charger_config);

    App_CanTx_Init();
    App_CanRx_Init();

    app_inverterOnState_init();
    app_accumulator_init();
    app_thermistors_init();
    app_soc_init();
    app_stateMachine_init(app_initState_get());
    app_globals_init(&globals_config);

    heartbeat_monitor = App_SharedHeartbeatMonitor_Create(
        io_time_getCurrentMs, HEARTBEAT_MONITOR_TIMEOUT_PERIOD_MS, heartbeatMonitorChecklist, heartbeatGetters,
        heartbeatUpdaters, &App_CanTx_BMS_Heartbeat_Set, heartbeatFaultSetters, heartbeatFaultGetters);
    globals->hb_monitor = heartbeat_monitor;

    // broadcast commit info
    App_CanTx_BMS_Hash_Set(GIT_COMMIT_HASH);
    App_CanTx_BMS_Clean_Set(GIT_COMMIT_CLEAN);
}

void tasks_run1Hz(void)
{
    static const TickType_t  period_ms = 1000U;
    SoftwareWatchdogHandle_t watchdog  = Io_SharedSoftwareWatchdog_AllocateWatchdog();
    Io_SharedSoftwareWatchdog_InitWatchdog(watchdog, RTOS_TASK_1HZ, period_ms);

    static uint32_t start_ticks = 0;
    start_ticks                 = osKernelGetTickCount();

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
    static const TickType_t  period_ms = 10;
    SoftwareWatchdogHandle_t watchdog  = Io_SharedSoftwareWatchdog_AllocateWatchdog();
    Io_SharedSoftwareWatchdog_InitWatchdog(watchdog, RTOS_TASK_100HZ, period_ms);

    static uint32_t start_ticks = 0;
    start_ticks                 = osKernelGetTickCount();

    for (;;)
    {
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
    static const TickType_t  period_ms = 1;
    SoftwareWatchdogHandle_t watchdog  = Io_SharedSoftwareWatchdog_AllocateWatchdog();
    Io_SharedSoftwareWatchdog_InitWatchdog(watchdog, RTOS_TASK_1KHZ, period_ms);

    static uint32_t start_ticks = 0;
    start_ticks                 = osKernelGetTickCount();

    for (;;)
    {
        // ADC wasn't reading any voltages when triggered by TIM3 like on other boards
        // But worked fine when starting the conversion via software as below
        // TODO: Figure out why
        HAL_ADC_Start_DMA(&hadc1, (uint32_t *)hw_adc_getRawValuesBuffer(), hadc1.Init.NbrOfConversion);

        // Check in for timeouts for all RTOS tasks
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