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
#include "io_fileSystem.h"

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
    // io_canLogging_init(boot_time_string);
}

static void test_sd_card(void)
{
    /* =================== SD Card Write Test  - Writing raw bytes =================== */
    uint32_t num_repetitions = 50;
#define NUM_BLOCKS (64)
    static uint8_t dummyData[NUM_BLOCKS * 512] = { 0 }; // 10 blocks of 512 bytes each
    uint32_t       start                       = io_time_getCurrentMs();
    for (uint32_t i = 0; i < num_repetitions; i++)
    {
        SdCardStatus sdWriteStatus = hw_sd_write(dummyData, i * NUM_BLOCKS, NUM_BLOCKS);
        assert(sdWriteStatus == SD_CARD_OK);
        osDelay(2);
    }
    uint32_t end           = io_time_getCurrentMs();
    uint32_t elapsedTimeMs = end - start - 2 * num_repetitions;
    LOG_INFO("Elapsed time: %d milliseconds\n", elapsedTimeMs);

    /* =================== SD Card Write Tests - Writing using logfs =================== */

    // char msg[] = "hello world";

    // uint32_t returned_fd;
    // FileSystemError status = io_fileSystem_open("/test2.txt", &returned_fd);
    // assert(status == FILE_OK);

    // FileSystemError status2 = io_fileSystem_write(returned_fd, &msg, 10);
    // assert(status2 == FILE_OK);

    // FileSystemError status3 = io_fileSystem_sync(returned_fd);
    // assert(status3 == FILE_OK);

    // LOG_INFO("Done.\n");

    // const uint32_t  period_ms                = 1000U;
    // const uint32_t  watchdog_grace_period_ms = 50U;
    // WatchdogHandle *watchdog= hw_watchdog_initTask(period_ms + watchdog_grace_period_ms);
    // for (;;)
    // {
    //     hw_watchdog_checkIn(watchdog);
    // }
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
    osDelay(osWaitForever);
    hw_chimera_v2_task(&chimera_v2_config);
}

_Noreturn void tasks_run1Hz(void)
{
    test_sd_card();
    osDelay(osWaitForever);
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
    osDelay(osWaitForever);
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
    osDelay(osWaitForever);
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
    osDelay(osWaitForever);
    for (;;)
    {
        CanMsg tx_msg = io_canQueue_popTx(&can_tx_queue);
        LOG_IF_ERR(hw_fdcan_transmit(&can1, &tx_msg));
    }
}

_Noreturn void tasks_runCanRx(void)
{
    osDelay(osWaitForever);
    for (;;)
    {
        jobs_runCanRx_tick();
    }
}

_Noreturn void tasks_runTelem(void)
{
    osDelay(osWaitForever);
    BaseTimeRegMsg base_time_msg = io_telemMessage_buildBaseTimeRegMsg(&boot_time);
    hw_uart_transmit(&_900k_uart, (uint8_t *)&base_time_msg, sizeof(base_time_msg));

    for (;;)
    {
        CanMsg      queue_out = io_telemMessageQueue_popTx();
        uint8_t     full_msg_size;
        TelemCanMsg can_msg = io_telemMessage_buildCanMsg(&queue_out, 0, &full_msg_size);

        LOG_INFO("Message timestamp: %d", queue_out.timestamp);

        // Start timing for measuring transmission speeds
        SEGGER_SYSVIEW_MarkStart(0);
        LOG_IF_ERR(hw_uart_transmit(&_900k_uart, (uint8_t *)&can_msg, full_msg_size));
        SEGGER_SYSVIEW_MarkStop(0);
    }
}

_Noreturn void tasks_runTelemRx(void)
{
    osDelay(osWaitForever);
    for (;;)
    {
        // set rtc time from telem rx data
        io_telemRx();
    }
}

_Noreturn void tasks_runLogging(void)
{
    osDelay(osWaitForever);
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
