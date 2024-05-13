#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "hw_watchdog.h"

// Table of hardware-agnostic software watchdog
typedef struct
{
    // Software watchdogs
    WatchdogHandle watchdogs[MAX_NUM_OF_SOFTWARE_WATCHDOG];
    // Index to keep track of how many software watchdogs have been allocated
    size_t allocation_index;
} WatchdogTable;

static WatchdogTable watchdog_table;

// Hook functions that the user must define in order to use this library
static void (*refreshHardwareWatchdog)(void);
static void (*timeoutCallback)(WatchdogHandle *);

void hw_watchdog_init(void (*refresh_hardware_watchdog)(void), void (*timeout_callback)(WatchdogHandle *))
{
    assert(refresh_hardware_watchdog != NULL);
    assert(timeout_callback != NULL);

    memset(&watchdog_table, 0, sizeof(watchdog_table));

    refreshHardwareWatchdog = refresh_hardware_watchdog;
    timeoutCallback         = timeout_callback;
}

WatchdogHandle *hw_watchdog_allocateWatchdog(void)
{
    assert(watchdog_table.allocation_index < MAX_NUM_OF_SOFTWARE_WATCHDOG);

    return (WatchdogHandle *)&watchdog_table.watchdogs[watchdog_table.allocation_index++];
}

void hw_watchdog_initWatchdog(WatchdogHandle *watchdog, uint8_t task_id, Tick_t period_in_ticks)
{
    watchdog->period          = period_in_ticks;
    watchdog->deadline        = period_in_ticks;
    watchdog->check_in_status = false;
    watchdog->initialized     = true;
    watchdog->task_id         = task_id;
}

void hw_watchdog_checkIn(WatchdogHandle *watchdog)
{
    assert(watchdog != NULL);
    assert(watchdog->initialized == true);

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
            continue;

        if (osKernelGetTickCount() >= watchdog_table.watchdogs[i].deadline)
        {
            // Check if the check-in status is set
            if (watchdog_table.watchdogs[i].check_in_status == true)
            {
                // Clear the check-in status
                watchdog_table.watchdogs[i].check_in_status = false;

                // Update deadline
                watchdog_table.watchdogs[i].deadline += watchdog_table.watchdogs[i].period;

                assert(refreshHardwareWatchdog != NULL);
                refreshHardwareWatchdog();
            }
            else
            {
                // We have no strict requirement that a timeout callback
                // function must be provided.
                if (timeoutCallback != NULL)
                {
                    timeoutCallback(&watchdog_table.watchdogs[i]);
                }
                timeout_detected = true;
            }
        }
    }
}

uint8_t hw_watchdog_getTaskId(WatchdogHandle *watchdog)
{
    return watchdog->task_id;
}
