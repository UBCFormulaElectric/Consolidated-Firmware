#include <cmsis_os.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "Io_SharedSoftwareWatchdog.h"

// Convert our anonymous handle to a software watchdog pointer
#define prvGetWatchdogFromHandle(handle) (SoftwareWatchdog_t *)(handle)

// Table of hardware-agnostic software watchdog
typedef struct SoftwareWatchdogTable
{
    // Software watchdogs
    SoftwareWatchdog_t watchdogs[MAX_NUM_OF_SOFTWARE_WATCHDOG];
    // Index to keep track of how many software watchdogs have been allocated
    size_t allocation_index;
} SoftwareWatchdogTable_t;

static SoftwareWatchdogTable_t sw_watchdog_table;

// Hook functions that the user must define in order to use this library
static void (*Io_RefreshHardwareWatchdog)();
static void (*App_TimeoutCallback)(WatchdogHandle);

void io_watchdog_init(void (*refresh_hardware_watchdog)(), void (*timeout_callback)(WatchdogHandle))
{
    assert(refresh_hardware_watchdog != NULL);
    assert(timeout_callback != NULL);

    memset(&sw_watchdog_table, 0, sizeof(sw_watchdog_table));

    Io_RefreshHardwareWatchdog = refresh_hardware_watchdog;
    App_TimeoutCallback        = timeout_callback;
}

WatchdogHandle io_watchdog_allocateWatchdog(void)
{
    assert(sw_watchdog_table.allocation_index < MAX_NUM_OF_SOFTWARE_WATCHDOG);

    return (WatchdogHandle)&sw_watchdog_table.watchdogs[sw_watchdog_table.allocation_index++];
}

void io_watchdog_initWatchdog(WatchdogHandle watchdog, uint8_t task_id, Tick_t period_in_ticks)
{
    assert(watchdog != NULL);

    SoftwareWatchdog_t *sw_watchdog = prvGetWatchdogFromHandle(watchdog);

    sw_watchdog->period          = period_in_ticks;
    sw_watchdog->deadline        = period_in_ticks;
    sw_watchdog->check_in_status = false;
    sw_watchdog->initialized     = true;
    sw_watchdog->task_id         = task_id;
}

void io_watchdog_checkIn(WatchdogHandle watchdog)
{
    assert(watchdog != NULL);

    SoftwareWatchdog_t *sw_watchdog = prvGetWatchdogFromHandle(watchdog);

    assert(sw_watchdog->initialized == true);

    sw_watchdog->check_in_status = true;
}

void io_watchdog_checkForTimeouts(void)
{
    static bool timeout_detected = false;

    // If a timeout is detected, let the hardware watchdog timeout reset the
    // system. We don't reboot immediately because we need some time to log
    // information for further debugging.
    for (int i = 0; i < MAX_NUM_OF_SOFTWARE_WATCHDOG && !timeout_detected; i++)
    {
        // Only check for timeout if the watchdog has been initialized
        if (sw_watchdog_table.watchdogs[i].initialized == false)
            continue;

        if (osKernelGetTickCount() >= sw_watchdog_table.watchdogs[i].deadline)
        {
            // Check if the check-in status is set
            if (sw_watchdog_table.watchdogs[i].check_in_status == true)
            {
                // Clear the check-in status
                sw_watchdog_table.watchdogs[i].check_in_status = false;

                // Update deadline
                sw_watchdog_table.watchdogs[i].deadline += sw_watchdog_table.watchdogs[i].period;

                assert(Io_RefreshHardwareWatchdog != NULL);
                Io_RefreshHardwareWatchdog();
            }
            else
            {
                // We have no strict requirement that a timeout callback
                // function must be provided.
                if (App_TimeoutCallback != NULL)
                {
                    App_TimeoutCallback(&sw_watchdog_table.watchdogs[i]);
                }
                timeout_detected = true;
            }
        }
    }
}

uint8_t io_watchdog_getTaskId(WatchdogHandle watchdog)
{
    SoftwareWatchdog_t *sw_watchdog = prvGetWatchdogFromHandle(watchdog);
    return sw_watchdog->task_id;
}
