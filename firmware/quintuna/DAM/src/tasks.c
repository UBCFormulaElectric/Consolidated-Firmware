#include "tasks.h"
#include "hw_bootup.h"
#include "hw_watchdog.h"
#include "jobs.h"
#include "main.h"
#include "cmsis_os.h"
#include "hw_sd.h"

#include "app_canTx.h"
#include "app_utils.h"
#include "app_jsoncan.h"
#include "app_canAlerts.h"

#include "io_log.h"
#include "io_canQueue.h"
#include "io_canLogging.h"
#include "io_fileSystem.h"
#include "io_telemMessage.h"
#include "io_telemMessageQueue.h"
#include "io_telemRx.h"
#include "io_canTx.h"
#include "io_tsim.h"
#include "io_canMsg.h"

#include "hw_hardFaultHandler.h"
#include "hw_cans.h"
#include "hw_usb.h"
#include "hw_crc.h"
#include "hw_gpios.h"
#include "hw_resetReason.h"
#include "hw_uarts.h"
#include "io_time.h"

#include <cmsis_os2.h>
#include <hw_chimera_v2.h>
#include <shared.pb.h>
#include <hw_chimeraConfig_v2.h>
#include <stdio.h>

uint32_t queueElementCounter = 100;

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
    ASSERT_EXIT_OK(io_rtc_readTime(&boot_time));
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

    io_canTx_DAM_Bootup_sendAperiodic();

    hw_gpio_writePin(&telem_pwr_en_pin, true);
    io_telemMessageQueue_init();
}

_Noreturn void tasks_runChimera(void)
{
    // hw_chimera_v2_task(&chimera_v2_config);
    io_tsim_set_green();

    /* SD Card Write Test*/

    // static uint8_t dummyData[10 * 512] = {0};

    // for (int m = 0; m < 10; m++)
    // {
    //     SdCardStatus sdEraseStatus = hw_sd_erase(0, 10*1000);
    //     assert(sdEraseStatus == SD_CARD_OK);

    //     uint32_t start = io_time_getCurrentMs();
    
    //     // Write to SD card
    //     for (int i = 0; i < 1000; i++)
    //     {
    //         SdCardStatus sdWriteStatus = hw_sd_write((uint8_t *)dummyData, 1 , 10);
    //         assert(sdWriteStatus == SD_CARD_OK);
    //     }
        
    //     uint32_t end = io_time_getCurrentMs();

    //     // calculate diff and print
    //     uint32_t elapsedTimeMs = end - start;
    //     LOG_INFO("Elapsed time: %d milliseconds", elapsedTimeMs);
    // }


    /* Radio Transmit Message Test */

    const CanMsg dummyRadioMsg = 
    {
        .std_id = 0,
        .dlc = 64,
        .timestamp = 0,
        .data.data8 = {0},
        .bus = 0,
        .is_fd = false
    };

    // IoRtcTime start; 
    // ExitCode exitcode = io_rtc_setTime(&start);
    // assert(exitcode == EXIT_CODE_OK);

    //LOG_INFO("Starting to send messages at hour %d, minute %d, second %d. \n", start.hours, start.minutes, start.seconds);

    if (queueElementCounter == 0)
    {
        for (int i = 0; i < 100; i++)
        {
            queueElementCounter++;
            bool status = io_telemMessageQueue_pushTx(&dummyRadioMsg);
            assert(status);
        }

        while(queueElementCounter != 0) {}
    }

    for(;;)
    {

    }
    
    //LOG_INFO("Stopped sending messages at hour %d, minute %d, second %d. \n", start.hours, start.minutes, start.seconds);
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
        LOG_IF_ERR(hw_fdcan_transmit(&can1, &tx_msg));
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
    BaseTimeRegMsg base_time_msg = io_telemMessage_buildBaseTimeRegMsg(&boot_time);
    hw_uart_transmit(&_900k_uart, (uint8_t *)&base_time_msg, sizeof(base_time_msg));

    for (;;)
    {
        CanMsg      queue_out = io_telemMessageQueue_popTx();
        queueElementCounter--;
        uint8_t     full_msg_size;
        TelemCanMsg can_msg = io_telemMessage_buildCanMsg(&queue_out, 0, &full_msg_size);
        
        LOG_INFO("Message timestamp: %d\n",queue_out.timestamp);

        // Start timing for measuring transmission speeds
        SEGGER_SYSVIEW_MarkStart(0);
        LOG_IF_ERR(hw_uart_transmit(&_900k_uart, (uint8_t *)&can_msg, full_msg_size));
        SEGGER_SYSVIEW_MarkStop(0);
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
