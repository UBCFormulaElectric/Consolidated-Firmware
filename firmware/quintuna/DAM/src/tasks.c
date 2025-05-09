#include "tasks.h"
#include "jobs.h"
#include "main.h"
#include "cmsis_os.h"

#include "app_canTx.h"

#include "io_log.h"
#include "io_canQueue.h"
#include "io_canLogging.h"
#include "io_fileSystem.h"
#include "io_buzzer.h"
#include "io_telemMessage.h"
#include "io_time.h"

#include "hw_hardFaultHandler.h"
#include "hw_cans.h"
#include "hw_usb.h"
#include "hw_gpios.h"
#include "hw_crc.h"

#include <hw_chimera_v2.h>
#include <shared.pb.h>
#include <hw_chimeraConfig_v2.h>
#include "hw_resetReason.h"

extern CRC_HandleTypeDef hcrc;

void tasks_preInit(void)
{
    // After booting, re-enable interrupts and ensure the core is using the application's vector table.
    // hw_bootup_enableInterruptsForApp();
}

void tasks_preInitWatchdog(void)
{
    // if (io_fileSystem_init() == FILE_OK)
    //     io_canLogging_init();
}

void tasks_init(void)
{
    SEGGER_SYSVIEW_Conf();
    LOG_INFO("DAM reset!");

    hw_hardFaultHandler_init();
    hw_can_init(&can1);
    hw_usb_init();
    hw_crc_init(&hcrc);
    // hw_watchdog_init(hw_watchdogConfig_refresh, hw_watchdogConfig_timeoutCallback);

    // hw_gpio_writePin(&tsim_red_en_pin, true);
    // hw_gpio_writePin(&ntsim_green_en_pin, false);

    jobs_init();
    // hw_gpio_writePin(&tsim_red_en_pin, true);
    // hw_gpio_writePin(&ntsim_green_en_pin, false);

    io_telemMessage_init();

    app_canTx_DAM_ResetReason_set((CanResetReason)hw_resetReason_get());
}

_Noreturn void tasks_runChimera(void)
{
    hw_chimera_v2_task(&chimera_v2_config);
}

_Noreturn void tasks_run1Hz(void)
{
    static const TickType_t period_ms = 1000U;
    // WatchdogHandle         *watchdog  = hw_watchdog_allocateWatchdog();
    // hw_watchdog_initWatchdog(watchdog, RTOS_TASK_1HZ, period_ms);

    uint32_t start_ticks = osKernelGetTickCount();
    for (;;)
    {
        if (!hw_chimera_v2_enabled)
            jobs_run1Hz_tick();

        // Watchdog check-in must be the last function called before putting the
        // task to sleep.
        // hw_watchdog_checkIn(watchdog);
        CanMsg fake_msg = {
            .std_id    = 0x123,
            .dlc       = 8,
            .data      = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07 },
            .timestamp = io_time_getCurrentMs(),
        };
        io_telemMessage_pushMsgtoQueue(&fake_msg);

        start_ticks += period_ms;
        osDelayUntil(start_ticks);
    }
}

_Noreturn void tasks_run100Hz(void)
{
    // WatchdogHandle         *watchdog  = hw_watchdog_allocateWatchdog();
    // hw_watchdog_initWatchdog(watchdog, RTOS_TASK_100HZ, period_ms);

    static const TickType_t period_ms   = 10;
    uint32_t                start_ticks = osKernelGetTickCount();
    for (;;)
    {
        if (!hw_chimera_v2_enabled)
            jobs_run100Hz_tick();

        // Watchdog check-in must be the last function called before putting the
        // task to sleep.
        // hw_watchdog_checkIn(watchdog);

        // CanMsg fake_msg = {
        //     .std_id    = 0x124,
        //     .dlc       = 8,
        //     .data      = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07 },
        //     .timestamp = io_time_getCurrentMs(),
        // };
        // io_telemMessage_pushMsgtoQueue(&fake_msg);

        start_ticks += period_ms;
        osDelayUntil(start_ticks);
    }
}

_Noreturn void tasks_run1kHz(void)
{
    // WatchdogHandle         *watchdog  = hw_watchdog_allocateWatchdog();
    // hw_watchdog_initWatchdog(watchdog, RTOS_TASK_1KHZ, period_ms);

    static const TickType_t period_ms   = 1U;
    uint32_t                start_ticks = osKernelGetTickCount();
    for (;;)
    {
        // const uint32_t task_start_ms = io_time_getCurrentMs();

        // hw_watchdog_checkForTimeouts();
        if (!hw_chimera_v2_enabled)
            jobs_run1kHz_tick();

        // // Watchdog check-in must be the last function called before putting the
        // // task to sleep. Prevent check in if the elapsed period is greater or
        // // equal to the period ms
        // if (io_tBime_getCurrentMs() - task_start_ms <= period_ms)
        //     hw_watchdog_checkIn(watchdog);

        // CanMsg fake_msg = {
        //     .std_id = 0x125,
        //     .dlc    = 8,
        //     .data   = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07},
        //     .timestamp = io_time_getCurrentMs(),
        // };
        // io_telemMessage_pushMsgtoQueue(&fake_msg);

        start_ticks += period_ms;
        osDelayUntil(start_ticks);
    }
}

_Noreturn void tasks_runCanTx(void)
{
    for (;;)
    {
        CanMsg tx_msg = io_canQueue_popTx();
        hw_fdcan_transmit(&can1, &tx_msg);
        hw_fdcan_transmit(&can1, &tx_msg);
        // ToDo: check if this is needed and investigate why is_fd is not a bool
        //  if (tx_msg.is_fd)
        //  {
        //      hw_fdcan_transmit(&can1, &tx_msg);
        //  }
        //  else
        //  {
        //      hw_can_transmit(&can1, &tx_msg);
        //  }
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
    // osDelay(osWaitForever);
    for (;;)
    {
        io_telemMessage_broadcastMsgFromQueue();
    }
}

_Noreturn void tasks_runLogging(void)
{
    osDelay(osWaitForever);
    // if (!io_fileSystem_ready())
    // {
    //     // queue shouldn't populate, so this is just an extra precaution
    //     osThreadSuspend(osThreadGetId());
    // }

    // static uint32_t write_count         = 0;
    // static uint32_t message_batch_count = 0;
    for (;;)
    {
        // io_canLogging_recordMsgFromQueue();
        // message_batch_count++;
        // write_count++;
        // if (message_batch_count > 256)
        // {
        //     io_canLogging_sync();
        //     message_batch_count = 0;
        // }
    }
}
