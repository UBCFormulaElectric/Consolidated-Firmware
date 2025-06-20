#include "tasks.h"
#include "app_stateMachine.h"
#include "hw_watchdog.h"
#include "states/app_balancingState.h"
#include "hw_bootup.h"
#include "io_ltc6813.h"
#include "jobs.h"
#include "io_bootHandler.h"
#include "io_canMsg.h"

#include "app_canTx.h"
#include "app_segments.h"
#include "app_utils.h"
#include "app_jsoncan.h"
#include "app_canAlerts.h"
#include "app_powerLimit.h"
#include "app_jsoncan.h"

#include "hw_gpios.h"
#include "io_log.h"
#include "io_canQueue.h"

// hw
#include "hw_usb.h"
#include "hw_cans.h"
#include "hw_adcs.h"
#include "hw_pwms.h"
#include "hw_hardFaultHandler.h"

// chimera
#include "hw_chimeraConfig_v2.h"
#include "hw_chimera_v2.h"
#include "hw_resetReason.h"
#include <io_canRx.h>
#include <io_canTx.h>

#include "semphr.h"
#include <FreeRTOS.h>
#include <app_canRx.h>
#include <cmsis_os2.h>
#include <portmacro.h>

CanTxQueue can_tx_queue;

static void jsoncan_transmit_func(const JsonCanMsg *tx_msg)
{
    const CanMsg msg = app_jsoncan_copyToCanMsg(tx_msg);
    io_canQueue_pushTx(&can_tx_queue, &msg);
}

static void charger_transmit_func(const JsonCanMsg *msg)
{
    // LOG_INFO("Send charger message: %d", msg->std_id);
}

// Define this guy to use CAN2 for talking to the Elcon.
// #define CHARGER_CAN

StaticSemaphore_t init_complete_locks_buf;
SemaphoreHandle_t init_complete_locks;

// isospi_bus_access_lock guards access to the ISOSPI bus, to guarantee an LTC transaction doesn't get interrupted by
// another task. It's just a regular semaphore (no priority inheritance) since it depends on hardware.

// ltc_app_data_lock guards access to any LTC app data that may be written to by the LTC tasks and read from other tick
// functions. It's a mutex (yes priority inheritance) since it's guarding shared data and doesn't depend on hardware.

StaticSemaphore_t isospi_bus_access_lock_buf;
SemaphoreHandle_t isospi_bus_access_lock;
StaticSemaphore_t ltc_app_data_lock_buf;
SemaphoreHandle_t ltc_app_data_lock;

void tasks_runChimera(void)
{
    hw_chimera_v2_task(&chimera_v2_config);
}

void tasks_preInit(void)
{
    hw_hardFaultHandler_init();
    hw_bootup_enableInterruptsForApp();
}

void tasks_init(void)
{
    // Configure and initialize SEGGER SystemView.
    // NOTE: Needs to be done after clock config!
    SEGGER_SYSVIEW_Conf();
    LOG_INFO("BMS Reset");

    __HAL_DBGMCU_FREEZE_IWDG1();

    isospi_bus_access_lock = xSemaphoreCreateBinaryStatic(&isospi_bus_access_lock_buf);
    ltc_app_data_lock      = xSemaphoreCreateMutexStatic(&ltc_app_data_lock_buf);
    assert(isospi_bus_access_lock != NULL);
    assert(ltc_app_data_lock != NULL);
    xSemaphoreGive(isospi_bus_access_lock);
    xSemaphoreGive(ltc_app_data_lock);

    LOG_IF_ERR(hw_usb_init());
    hw_adcs_chipsInit();
    hw_pwms_init();

// TODO: Start CAN1/CAN2 based on if we're charging at runtime.
#ifdef CHARGER_CAN
    hw_can_init(&can2);
#else
    hw_can_init(&can1);
#endif

    // Shutdown loop power comes from a load switch on the BMS.
    hw_gpio_writePin(&shdn_en_pin, true);

    const ResetReason reset_reason = hw_resetReason_get();
    app_canTx_BMS_ResetReason_set((CanResetReason)reset_reason);

    // Check for watchdog timeout on a previous boot cycle and populate CAN alert.
    if (reset_reason == RESET_REASON_WATCHDOG)
    {
        LOG_WARN("Detected watchdog timeout on the previous boot cycle!");
        app_canAlerts_BMS_Info_WatchdogTimeout_set(true);
    }

    BootRequest boot_request = hw_bootup_getBootRequest();
    if (boot_request.context != BOOT_CONTEXT_NONE)
    {
        // Check for stack overflow on a previous boot cycle and populate CAN alert.
        if (boot_request.context == BOOT_CONTEXT_STACK_OVERFLOW)
        {
            LOG_WARN("Detected stack overflow on the previous boot cycle!");
            app_canAlerts_BMS_Info_StackOverflow_set(true);
            app_canTx_BMS_StackOverflowTask_set(boot_request.context_value);
        }
        else if (boot_request.context == BOOT_CONTEXT_WATCHDOG_TIMEOUT)
        {
            // If the software driver detected a watchdog timeout the context should be set.
            app_canTx_BMS_WatchdogTimeoutTask_set(boot_request.context_value);
        }

        // Clear stack overflow bootup.
        boot_request.context       = BOOT_CONTEXT_NONE;
        boot_request.context_value = 0;
        hw_bootup_setBootRequest(boot_request);
    }

    io_canQueue_initRx();
    io_canQueue_initTx(&can_tx_queue);
    io_canTx_init(jsoncan_transmit_func, charger_transmit_func);
    io_canTx_enableMode_can1(CAN1_MODE_DEFAULT, true);
    io_canTx_enableMode_charger(CHARGER_MODE_DEFAULT, true);

    jobs_init();

    io_canTx_BMS_Bootup_sendAperiodic();
}

void tasks_run1Hz(void)
{
    const uint32_t  period_ms                = 1000U;
    const uint32_t  watchdog_grace_period_ms = 50U;
    WatchdogHandle *watchdog                 = hw_watchdog_initTask(period_ms + watchdog_grace_period_ms);

    uint32_t start_ticks = osKernelGetTickCount();
    for (;;)
    {
        if (!hw_chimera_v2_enabled)
        {
            jobs_run1Hz_tick();
        }

        // Watchdog check-in must be the last function called before putting the task to sleep.
        hw_watchdog_checkIn(watchdog);

        start_ticks += period_ms;
        osDelayUntil(start_ticks);
    }
}

void tasks_run100Hz(void)
{
    const uint32_t  period_ms                = 10U;
    const uint32_t  watchdog_grace_period_ms = 2U;
    WatchdogHandle *watchdog                 = hw_watchdog_initTask(period_ms + watchdog_grace_period_ms);

    uint32_t start_ticks = osKernelGetTickCount();
    for (;;)
    {
        if (!hw_chimera_v2_enabled)
        {
            const bool debug_mode_enabled = app_canRx_Debug_EnableDebugMode_get();
            io_canTx_enableMode_can1(CAN1_MODE_DEBUG, debug_mode_enabled);

            // Need to wrap the state machine tick in the LTC app mutex so don't use jobs.c for 100Hz.
            xSemaphoreTake(ltc_app_data_lock, portMAX_DELAY);
            app_stateMachine_tick100Hz();
            xSemaphoreGive(ltc_app_data_lock);

            app_stateMachine_tickTransitionState();
            io_canTx_enqueue100HzMsgs();
        }

        app_powerLimit_broadcast(); // Current and power limiting CAN messages

        // Watchdog check-in must be the last function called before putting the task to sleep.
        hw_watchdog_checkIn(watchdog);

        start_ticks += period_ms;
        osDelayUntil(start_ticks);
    }
}

void tasks_run1kHz(void)
{
    const uint32_t  period_ms                = 1U;
    const uint32_t  watchdog_grace_period_ms = 1U;
    WatchdogHandle *watchdog                 = hw_watchdog_initTask(period_ms + watchdog_grace_period_ms);

    uint32_t start_ticks = osKernelGetTickCount();
    for (;;)
    {
        hw_watchdog_checkForTimeouts();

        jobs_run1kHz_tick();

        // Watchdog check-in must be the last function called before putting the task to sleep.
        hw_watchdog_checkIn(watchdog);

        start_ticks += period_ms;
        osDelayUntil(start_ticks);
    }
}

void tasks_runCanTx(void)
{
    for (;;)
    {
        CanMsg tx_msg = io_canQueue_popTx(&can_tx_queue);

#ifdef CHARGER_CAN
        // Elcon only supports regular CAN but we have some debug messages that are >8 bytes long. Use FDCAN for those
        // (they won't get seen by the charger, but they'll show up on CANoe).
        // TODO: Bit-rate-switching wasn't working for me when the BMS was connected to the charger, so the FD
        // peripheral is configured without BRS. Figure out why it wasn't working?
        if (tx_msg.dlc > 8)
        {
            LOG_IF_ERR(hw_fdcan_transmit(&can2, &tx_msg));
        }
        else
        {
            LOG_IF_ERR(hw_can_transmit(&can2, &tx_msg));
        }
#else
        LOG_IF_ERR(hw_fdcan_transmit(&can1, &tx_msg));
#endif
    }
}

void tasks_runCanRx(void)
{
    for (;;)
    {
        const CanMsg rx_msg       = io_canQueue_popRx();
        JsonCanMsg   json_can_msg = app_jsoncan_copyFromCanMsg(&rx_msg);
        io_canRx_updateRxTableWithMessage(&json_can_msg);
    }
}

// TODO: Add watchdogs to the LTC commands

void tasks_runLtcVoltages(void)
{
    static const TickType_t period_ms = 1000U; // 1Hz

    xSemaphoreTake(isospi_bus_access_lock, portMAX_DELAY);
    {
        // Write LTC configs.
        io_ltc6813_wakeup();
        LOG_IF_ERR(app_segments_configSync());
    }
    xSemaphoreGive(isospi_bus_access_lock);

    for (;;)
    {
        const uint32_t start_ticks = osKernelGetTickCount();

        const bool balancing_enabled = app_stateMachine_getCurrentState() == app_balancingState_get();

        xSemaphoreTake(isospi_bus_access_lock, portMAX_DELAY);
        {
            io_ltc6813_wakeup();
            LOG_IF_ERR(app_segments_configSync());

            // Mute/unmute balancing.
            if (balancing_enabled)
            {
                LOG_IF_ERR(io_ltc6813_sendBalanceCommand());
            }
            else
            {
                io_ltc6813_sendStopBalanceCommand();
            }

            LOG_IF_ERR(app_segments_runVoltageConversion());
        }
        xSemaphoreGive(isospi_bus_access_lock);

        xSemaphoreTake(ltc_app_data_lock, portMAX_DELAY);
        {
            app_segments_broadcastCellVoltages();
            app_segments_broadcastVoltageStats();
            app_segments_balancingTick(balancing_enabled);
        }
        xSemaphoreGive(ltc_app_data_lock);

        LOG_INFO("LTC voltage period remaining: %dms", start_ticks + period_ms - osKernelGetTickCount());
        osDelayUntil(start_ticks + period_ms);
    }
}

void tasks_runLtcTemps(void)
{
    static const TickType_t period_ms = 1000U; // 1Hz

    xSemaphoreTake(isospi_bus_access_lock, portMAX_DELAY);
    {
        // Write LTC configs.
        io_ltc6813_wakeup();
        LOG_IF_ERR(app_segments_configSync());
    }
    xSemaphoreGive(isospi_bus_access_lock);

    for (;;)
    {
        const uint32_t start_ticks = osKernelGetTickCount();

        xSemaphoreTake(isospi_bus_access_lock, portMAX_DELAY);
        {
            io_ltc6813_wakeup();
            LOG_IF_ERR(app_segments_runAuxConversion());
        }
        xSemaphoreGive(isospi_bus_access_lock);

        xSemaphoreTake(ltc_app_data_lock, portMAX_DELAY);
        {
            app_segments_broadcastTempsVRef();
            app_segments_broadcastTempStats();
        }
        xSemaphoreGive(ltc_app_data_lock);

        LOG_INFO("LTC temp period remaining: %dms", start_ticks + period_ms - osKernelGetTickCount());
        osDelayUntil(start_ticks + period_ms);
    }
}

void tasks_runLtcDiagnostics(void)
{
    static const TickType_t period_ms = 10000U; // Every 10s

    // Run all self tests at init.
    xSemaphoreTake(isospi_bus_access_lock, portMAX_DELAY);
    {
        // Write LTC configs.
        io_ltc6813_wakeup();
        LOG_IF_ERR(app_segments_configSync());

        LOG_IF_ERR(app_segments_runAdcAccuracyTest());
        LOG_IF_ERR(app_segments_runVoltageSelfTest());
        LOG_IF_ERR(app_segments_runAuxSelfTest());
        LOG_IF_ERR(app_segments_runStatusSelfTest());
        LOG_IF_ERR(app_segments_runOpenWireCheck());
    }
    xSemaphoreGive(isospi_bus_access_lock);

    xSemaphoreTake(ltc_app_data_lock, portMAX_DELAY);
    {
        app_segments_broadcastAdcAccuracyTest();
        app_segments_broadcastVoltageSelfTest();
        app_segments_broadcastAuxSelfTest();
        app_segments_broadcastStatusSelfTest();
        app_segments_broadcastOpenWireCheck();
    }
    xSemaphoreGive(ltc_app_data_lock);

    for (;;)
    {
        const uint32_t start_ticks = osKernelGetTickCount();

        xSemaphoreTake(isospi_bus_access_lock, portMAX_DELAY);
        {
            io_ltc6813_wakeup();

            // Open wire check is the only one we need to perform as per rules.
            LOG_IF_ERR(app_segments_runStatusConversion());
            LOG_IF_ERR(app_segments_runOpenWireCheck());

            if (app_canRx_Debug_EnableDebugMode_get())
            {
                LOG_IF_ERR(app_segments_runAdcAccuracyTest());
                LOG_IF_ERR(app_segments_runVoltageSelfTest());
                LOG_IF_ERR(app_segments_runAuxSelfTest());
                LOG_IF_ERR(app_segments_runStatusSelfTest());
            }
        }
        xSemaphoreGive(isospi_bus_access_lock);

        xSemaphoreTake(ltc_app_data_lock, portMAX_DELAY);
        {
            app_segments_broadcastStatus();
            app_segments_broadcastOpenWireCheck();

            if (app_canRx_Debug_EnableDebugMode_get())
            {
                app_segments_broadcastAdcAccuracyTest();
                app_segments_broadcastVoltageSelfTest();
                app_segments_broadcastAuxSelfTest();
                app_segments_broadcastStatusSelfTest();
            }
        }
        xSemaphoreGive(ltc_app_data_lock);

        osDelayUntil(start_ticks + period_ms);
    }
}
