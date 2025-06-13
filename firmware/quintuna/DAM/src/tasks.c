#include "tasks.h"
#include "hw_bootup.h"
#include "hw_watchdog.h"
#include "jobs.h"
#include "main.h"
#include "cmsis_os.h"

#include "app_canTx.h"
#include "app_utils.h"
#include "app_canAlerts.h"

#include "io_log.h"
#include "io_canQueue.h"
#include "io_canLogging.h"
#include "io_fileSystem.h"
#include "io_buzzer.h"
#include "io_telemMessage.h"
#include "io_telemRx.h"
#include "io_time.h"

#include "hw_hardFaultHandler.h"
#include "hw_cans.h"
#include "hw_usb.h"
#include "hw_gpios.h"
#include "hw_crc.h"
#include "hw_resetReason.h"

#include <cmsis_os2.h>
#include <hw_chimera_v2.h>
#include <shared.pb.h>
#include <hw_chimeraConfig_v2.h>

// Note: Need to declare this here (not at the top of main.h) since the name hcrc shadows other local variables that
// include main.h (and the compiler doesn't like that for some reason).
extern CRC_HandleTypeDef hcrc;

IoRtcTime boot_time;
char      boot_time_string[27]; // YYYY-MM-DDTHH:MM:SS

void tasks_preInit(void)
{
    hw_hardFaultHandler_init();
    hw_bootup_enableInterruptsForApp();
}

void tasks_preInitWatchdog(void)
{
    ExitCode status = io_rtc_readTime(&boot_time);
    sprintf(
        boot_time_string, "20%02d-%02d-%02dT%02d-%02d-%02d", boot_time.year, boot_time.month, boot_time.day,
        boot_time.hours, boot_time.minutes, boot_time.seconds);
    io_canLogging_init(boot_time_string);
}

void tasks_init(void)
{
    // Configure and initialize SEGGER SystemView.
    // NOTE: Needs to be done after clock config!
    SEGGER_SYSVIEW_Conf();
    LOG_INFO("DAM reset!");

    __HAL_DBGMCU_FREEZE_IWDG1();

    hw_can_init(&can1);
    ASSERT_EXIT_OK(hw_usb_init());
    hw_crc_init(&hcrc);

    const ResetReason reset_reason = hw_resetReason_get();
    app_canTx_DAM_ResetReason_set((CanResetReason)reset_reason);

    // Check for watchdog timeout on a previous boot cycle and populate CAN alert.
    if (reset_reason == RESET_REASON_WATCHDOG)
    {
        LOG_WARN("Detected watchdog timeout on the previous boot cycle!");
        app_canAlerts_DAM_Info_WatchdogTimeout_set(true);
    }

    BootRequest boot_request = hw_bootup_getBootRequest();
    if (boot_request.context != BOOT_CONTEXT_NONE)
    {
        // Check for stack overflow on a previous boot cycle and populate CAN alert.
        if (boot_request.context == BOOT_CONTEXT_STACK_OVERFLOW)
        {
            LOG_WARN("Detected stack overflow on the previous boot cycle!");
            app_canAlerts_DAM_Info_StackOverflow_set(true);
            app_canTx_DAM_StackOverflowTask_set(boot_request.context_value);
        }
        else if (boot_request.context == BOOT_CONTEXT_WATCHDOG_TIMEOUT)
        {
            // If the software driver detected a watchdog timeout the context should be set.
            app_canTx_DAM_WatchdogTimeoutTask_set(boot_request.context_value);
        }

        // Clear stack overflow bootup.
        boot_request.context       = BOOT_CONTEXT_NONE;
        boot_request.context_value = 0;
        hw_bootup_setBootRequest(boot_request);
    }

    jobs_init();
}

_Noreturn void tasks_runChimera(void)
{
    hw_chimera_v2_task(&chimera_v2_config);
}

_Noreturn void tasks_run1Hz(void)
{
    const uint32_t  period_ms                = 1000U;
    const uint32_t  watchdog_grace_period_ms = 50U;
    WatchdogHandle *watchdog                 = hw_watchdog_initTask(period_ms + watchdog_grace_period_ms);

    uint32_t start_ticks = osKernelGetTickCount();
    for (;;)
    {
        if (!hw_chimera_v2_enabled)
            jobs_run1Hz_tick();

        // Watchdog check-in must be the last function called before putting the task to sleep.
        hw_watchdog_checkIn(watchdog);

        start_ticks += period_ms;
        osDelayUntil(start_ticks);
    }
}

_Noreturn void tasks_run100Hz(void)
{
    const uint32_t  period_ms                = 10U;
    const uint32_t  watchdog_grace_period_ms = 2U;
    WatchdogHandle *watchdog                 = hw_watchdog_initTask(period_ms + watchdog_grace_period_ms);

    uint32_t start_ticks = osKernelGetTickCount();
    for (;;)
    {
        if (!hw_chimera_v2_enabled)
            jobs_run100Hz_tick();

        // io_telemMessage_pushMsgtoQueue(&fake_msg);

        // Watchdog check-in must be the last function called before putting the task to sleep.
        hw_watchdog_checkIn(watchdog);

        start_ticks += period_ms;
        osDelayUntil(start_ticks);
    }
}

_Noreturn void tasks_run1kHz(void)
{
    const uint32_t  period_ms                = 1U;
    const uint32_t  watchdog_grace_period_ms = 1U;
    WatchdogHandle *watchdog                 = hw_watchdog_initTask(period_ms + watchdog_grace_period_ms);

    uint32_t start_ticks = osKernelGetTickCount();
    for (;;)
    {
        hw_watchdog_checkForTimeouts();

        if (!hw_chimera_v2_enabled)
            jobs_run1kHz_tick();

        // Watchdog check-in must be the last function called before putting the task to sleep.
        hw_watchdog_checkIn(watchdog);

        start_ticks += period_ms;
        osDelayUntil(start_ticks);
    }
}

_Noreturn void tasks_runCanTx(void)
{
    for (;;)
    {
        CanMsg tx_msg = io_canQueue_popTx(&can_tx_queue);
        // LOG_IF_ERR(hw_fdcan_transmit(&can1, &tx_msg));
        // LOG_IF_ERR(hw_fdcan_transmit(&can1, &tx_msg));
        // ToDo: check if this is needed and investigate why is_fd is not a bool

        hw_fdcan_transmit(&can1, &tx_msg);
    }
}

_Noreturn void tasks_runCanRx(void)
{
    for (;;)
    {
        jobs_runCanRx_tick();
    }
}

_Noreturn void tasks_runTelem(void)
{
    for (;;)
    {
        io_telemMessage_broadcastMsgFromQueue();
    }
}

_Noreturn void tasks_runTelemRx(void)
{
    for (;;)
    {
        // set rtc time from telem rx data
        io_telemRx();
    }
}

_Noreturn void tasks_runLogging(void)
{
    static uint32_t write_count         = 0;
    static uint32_t message_batch_count = 0;

    for (;;)
    {
        if (io_canLogging_errorsRemaining() == 0)
        {
            osThreadSuspend(osThreadGetId());
        }

        io_canLogging_recordMsgFromQueue();
        message_batch_count++;
        write_count++;

        if (message_batch_count > 256)
        {
            io_canLogging_sync();
            message_batch_count = 0;
        }
    }
}
