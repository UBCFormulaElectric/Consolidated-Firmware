#include "tasks.h"
#include "hw_bootup.h"
#include "io_ltc6813.h"
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

void tasks_runChimera(void)
{
    hw_chimera_v2_task(&chimera_v2_config);
}

void tasks_preInit(void)
{
    hw_hardFaultHandler_init();
    // hw_bootup_enableInterruptsForApp();
}

void tasks_init(void)
{
    // Configure and initialize SEGGER SystemView.
    // NOTE: Needs to be done after clock config!
    SEGGER_SYSVIEW_Conf();
    LOG_INFO("BMS Reset");

    __HAL_DBGMCU_FREEZE_IWDG1();

    ASSERT_EXIT_OK(hw_usb_init());
    hw_adcs_chipsInit();
    hw_pwms_init();
    hw_can_init(&can1);
    hw_can_init(&can2);
    hw_watchdog_init(hw_watchdogConfig_refresh, hw_watchdogConfig_timeoutCallback);

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

void tasks_runLtc(void)
{
    // setup
    app_segments_writeDefaultConfig();
    LOG_IF_ERR(app_segments_configSync());

    // self tests
    LOG_IF_ERR(app_segments_voltageSelftest());
    LOG_IF_ERR(app_segments_auxSelftest());
    LOG_IF_ERR(app_segments_statusSelftest());
    // RETURN_IF_ERR(app_segments_openWireCheck()); // TODO: Test this
    LOG_IF_ERR(app_segments_ADCAccuracyTest());

    for (;;)
    {
        jobs_runLtc_tick();
    }
}
