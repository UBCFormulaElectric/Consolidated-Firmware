#include "tasks.h"
#include "jobs.h"

#include "app_canTx.h"
#include "app_canRx.h"
#include "app_utils.h"
#include "app_jsoncan.h"
#include "app_canAlerts.h"

#include "io_log.h"
#include "io_canQueue.h"
#include "io_canRx.h"
#include "io_canTx.h"
#include "io_canMsg.h"
#include "io_semaphore.h"

// hw
#include "hw_usb.h"
#include "hw_cans.h"
#include "hw_adcs.h"
#include "hw_pwms.h"
#include "hw_hardFaultHandler.h"
#include "hw_watchdog.h"
#include "hw_bootup.h"
#include "hw_gpios.h"
#include "hw_resetReason.h"

// chimera
#include "hw_chimeraConfig_v2.h"
#include "hw_chimera_v2.h"

#include <FreeRTOS.h>
#include <cmsis_os2.h>
#include <portmacro.h>

// Define this guy to use CAN2 for talking to the Elcon.
// #define CHARGER_CAN

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

    // Shutdown loop power comes from a load switch on the BMS.
    hw_gpio_writePin(&shdn_en_pin, true);

    jobs_init();

    io_canTx_BMS_Bootup_sendAperiodic(); // TODO do this in jobs_init
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
            jobs_run100Hz_tick();
        }

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
    static const TickType_t period_ms = 500U; // 2Hz
    jobs_initLTCVoltages();
    for (;;)
    {
        const uint32_t start_ticks = osKernelGetTickCount();
        jobs_runLTCVoltages();
        LOG_INFO("LTC voltage period remaining: %dms", start_ticks + period_ms - osKernelGetTickCount());
        osDelayUntil(start_ticks + period_ms);
    }
}

void tasks_runLtcTemps(void)
{
    static const TickType_t period_ms = 500U; // 2Hz
    jobs_initLTCTemps();
    for (;;)
    {
        const uint32_t start_ticks = osKernelGetTickCount();
        jobs_runLTCTemperatures();
        LOG_INFO("LTC temp period remaining: %dms", start_ticks + period_ms - osKernelGetTickCount());
        osDelayUntil(start_ticks + period_ms);
    }
}

void tasks_runLtcDiagnostics(void)
{
    static const TickType_t period_ms = 10000U; // Every 10s
    jobs_initLTCDiagnostics();
    for (;;)
    {
        const uint32_t start_ticks = osKernelGetTickCount();
        jobs_runLTCDiagnostics();
        osDelayUntil(start_ticks + period_ms);
    }
}
