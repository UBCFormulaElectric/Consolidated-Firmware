#include "tasks.h"
#include "hw_bootup.h"
#include "jobs.h"

#include "app_canTx.h"
#include "app_segments.h"
#include "app_utils.h"
#include "app_canAlerts.h"

#include "hw_bootup.h"
#include "io_log.h"
#include "io_canQueue.h"

// hw
#include "hw_usb.h"
#include "hw_cans.h"
#include "hw_adcs.h"
#include "hw_pwms.h"
#include "hw_watchdogConfig.h"
#include "hw_hardFaultHandler.h"

// chimera
#include "hw_chimeraConfig_v2.h"
#include "hw_chimera_v2.h"
#include "hw_resetReason.h"

#include "semphr.h"
#include <FreeRTOS.h>
#include <app_canRx.h>
#include <cmsis_os.h>
#include <cmsis_os2.h>
#include <portmacro.h>

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

    LOG_IF_ERR(hw_usb_init());
    hw_adcs_chipsInit();
    hw_pwms_init();
    hw_watchdog_init(hw_watchdogConfig_refresh, hw_watchdogConfig_timeoutCallback);

    // TODO: Start CAN1/CAN2 based on if we're charging
    hw_can_init(&can1);
    // hw_can_init(&can2);

    jobs_init();

    app_canTx_BMS_ResetReason_set((CanResetReason)hw_resetReason_get());

    // Check for stack overflow on a previous boot cycle and populate CAN alert.
    BootRequest boot_request = hw_bootup_getBootRequest();
    if (boot_request.context == BOOT_CONTEXT_STACK_OVERFLOW)
    {
        app_canAlerts_BMS_Info_StackOverflow_set(true);
        app_canTx_BMS_StackOverflowTask_set(boot_request.context_value);

        // Clear stack overflow bootup.
        boot_request.context       = BOOT_CONTEXT_NONE;
        boot_request.context_value = 0;
        hw_bootup_setBootRequest(boot_request);
    }

    isospi_bus_access_lock = xSemaphoreCreateBinaryStatic(&isospi_bus_access_lock_buf);
    ltc_app_data_lock      = xSemaphoreCreateMutexStatic(&ltc_app_data_lock_buf);
    assert(isospi_bus_access_lock != NULL);
    assert(ltc_app_data_lock != NULL);
    xSemaphoreGive(isospi_bus_access_lock);
    xSemaphoreGive(ltc_app_data_lock);

    // Write LTC configs.
    app_segments_writeDefaultConfig();
    io_ltc6813_wakeup();

    // TODO: This blocks forever if modules don't reply. If we can't talk to modules we're boned anyway so not the end
    // of the world but there's probably a way to make this more clear...
    LOG_IF_ERR(app_segments_configSync());

    // Run all self tests at init.
    LOG_IF_ERR(app_segments_runAdcAccuracyTest());
    LOG_IF_ERR(app_segments_runVoltageSelfTest());
    LOG_IF_ERR(app_segments_runAuxSelfTest());
    LOG_IF_ERR(app_segments_runStatusSelfTest());
    LOG_IF_ERR(app_segments_runOpenWireCheck());

    app_segments_broadcastAdcAccuracyTest();
    app_segments_broadcastVoltageSelfTest();
    app_segments_broadcastAuxSelfTest();
    app_segments_broadcastStatusSelfTest();
    app_segments_broadcastOpenWireCheck();
}

void tasks_run1Hz(void)
{
    static const TickType_t period_ms   = 1000U;
    uint32_t                start_ticks = osKernelGetTickCount();
    for (;;)
    {
        if (!hw_chimera_v2_enabled)
        {
            jobs_run1Hz_tick();
        }
        start_ticks += period_ms;
        osDelayUntil(start_ticks);
    }
}

void tasks_run100Hz(void)
{
    static const TickType_t period_ms   = 10U;
    uint32_t                start_ticks = osKernelGetTickCount();
    for (;;)
    {
        if (!hw_chimera_v2_enabled)
        {
            jobs_run100Hz_tick();
        }
        start_ticks += period_ms;
        osDelayUntil(start_ticks);
    }
}

void tasks_run1kHz(void)
{
    static const TickType_t period_ms   = 1U;
    uint32_t                start_ticks = osKernelGetTickCount();
    for (;;)
    {
        // hw_watchdog_checkForTimeouts();
        jobs_run1kHz_tick();
        start_ticks += period_ms;
        osDelayUntil(start_ticks);
    }
}

void tasks_runCanTx(void)
{
    for (;;)
    {
        CanMsg tx_msg = io_canQueue_popTx(&can_tx_queue);
        LOG_IF_ERR(hw_fdcan_transmit(&can1, &tx_msg));
    }
}

void tasks_runCanRx(void)
{
    for (;;)
    {
        jobs_runCanRx_tick();
    }
}

// TODO: Add watchdogs to the LTC commands

void tasks_runLtcVoltages(void)
{
    static const TickType_t period_ms = 100U; // 10Hz

    for (;;)
    {
        const uint32_t start_ticks = osKernelGetTickCount();

        xSemaphoreTake(isospi_bus_access_lock, portMAX_DELAY);
        {
            io_ltc6813_wakeup();
            LOG_IF_ERR(app_segments_runVoltageConversion());
        }
        xSemaphoreGive(isospi_bus_access_lock);

        xSemaphoreTake(ltc_app_data_lock, portMAX_DELAY);
        {
            app_segments_broadcastCellVoltages();
        }
        xSemaphoreGive(ltc_app_data_lock);

        LOG_INFO("LTC voltage period remaining: %dms", start_ticks + period_ms - osKernelGetTickCount());
        osDelayUntil(start_ticks + period_ms);
    }
}

void tasks_runLtcTemps(void)
{
    static const TickType_t period_ms = 1000U; // 1Hz

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
        }
        xSemaphoreGive(ltc_app_data_lock);

        LOG_INFO("LTC temp period remaining: %dms", start_ticks + period_ms - osKernelGetTickCount());
        osDelayUntil(start_ticks + period_ms);
    }
}

void tasks_runLtcDiagnostics(void)
{
    static const TickType_t period_ms = 10000U; // Every 10s

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
