#include <FreeRTOS.h>
#include <cmsis_os.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "hw_watchdog.h"
#include "io_log.h"
#include "io_time.h"
#include "main.h"
#ifdef BOOTLOADER
#include "hw_bootup.h"
#endif

#ifdef STM32F412Rx
// extern IWDG_HandleTypeDef  hiwdg;
static IWDG_HandleTypeDef *iwdg_handle = &hiwdg;
#elif STM32H733xx
// extern IWDG_HandleTypeDef  hiwdg1;
static IWDG_HandleTypeDef *iwdg_handle = &hiwdg1;
#else
#error "Please define what MCU is used."
#endif

// Table of hardware-agnostic software watchdog
typedef struct
{
    // Software watchdogs
    WatchdogHandle watchdogs[MAX_NUM_OF_SOFTWARE_WATCHDOG];
    // Index to keep track of how many software watchdogs have been allocated
    size_t allocation_index;
} WatchdogTable;

static WatchdogTable watchdog_table;

static void refreshHardwareWatchdog(void)
{
    HAL_IWDG_Refresh(iwdg_handle);
}

WatchdogHandle *hw_watchdog_initTask(uint32_t period_ms)
{
    assert(watchdog_table.allocation_index < MAX_NUM_OF_SOFTWARE_WATCHDOG);
    WatchdogHandle *watchdog = &watchdog_table.watchdogs[watchdog_table.allocation_index++];

    watchdog->task            = xTaskGetCurrentTaskHandle();
    watchdog->period          = period_ms;
    watchdog->deadline        = io_time_getCurrentMs() + period_ms;
    watchdog->check_in_status = false;
    watchdog->initialized     = true;

    return watchdog;
}

void hw_watchdog_checkIn(WatchdogHandle *watchdog)
{
    assert(watchdog != NULL);
    assert(watchdog->initialized == true);

    // Prevent check in if we've already timed out.
    const bool passed_deadline = io_time_getCurrentMs() > watchdog->deadline;
    if (passed_deadline)
    {
        return;
    }

    watchdog->check_in_status = true;
}

void hw_watchdog_checkForTimeouts(void)
{
    static bool timeout_detected = false;

    // If a timeout is detected, let the hardware watchdog timeout reset the
    // system. We don't reboot immediately because we need some time to log
    // information for further debugging.
    for (int i = 0; i < MAX_NUM_OF_SOFTWARE_WATCHDOG && !timeout_detected; i++)
    {
        // Only check for timeout if the watchdog has been initialized
        if (watchdog_table.watchdogs[i].initialized == false)
            break;

        const bool checked_in      = watchdog_table.watchdogs[i].check_in_status == true;
        const bool passed_deadline = io_time_getCurrentMs() > watchdog_table.watchdogs[i].deadline;

        if (passed_deadline)
        {
            if (checked_in)
            {
                // Clear the check-in status.
                watchdog_table.watchdogs[i].check_in_status = false;

                // Update deadline.
                watchdog_table.watchdogs[i].deadline = io_time_getCurrentMs() + watchdog_table.watchdogs[i].period;
            }
            else
            {
                TaskStatus_t status;
                vTaskGetInfo(watchdog_table.watchdogs[i].task, &status, pdFALSE, eRunning);

                LOG_ERROR(
                    "Software watchdog detected a timeout in a task, letting hardware watchdog reset the MCU (task = "
                    "%s, "
                    "id = %d)",
                    pcTaskGetTaskName(watchdog_table.watchdogs[i].task), status.xTaskNumber);

#ifdef BOOTLOADER
                const BootRequest request = { .target        = BOOT_TARGET_APP,
                                              .context       = BOOT_CONTEXT_WATCHDOG_TIMEOUT,
                                              .context_value = status.xTaskNumber };
                hw_bootup_setBootRequest(request);
#endif
                // Stop petting the hardware watchdog.
                timeout_detected = true;
            }
        }
    }

    // If there is no timeout, pet the watchdog.
    if (!timeout_detected)
    {
        assert(refreshHardwareWatchdog != NULL);
        refreshHardwareWatchdog();
    }
}
