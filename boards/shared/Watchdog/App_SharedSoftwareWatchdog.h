/**
 * @brief Monitor task deadlines by assigning a software watchdog to each
 *        periodic task.
 * @note The current implementation doesn't really support monitoring
 *       "non-periodic" tasks (e.g. CAN TX).
 */
#pragma once

#include <stdbool.h>

// Application specific configuration options
#include "App_SharedSoftwareWatchdogConfig.h"

// Check all the required application specific macros have been defined in
// <App_SharedSoftwareWatchdogConfig.h>.
#ifndef MAX_NUM_OF_SOFTWARE_WATCHDOG
#error Missing definition: MAX_NUM_OF_SOFTWARE_WATCHDOG must be defined in App_SharedSoftwareWatchdogConfig.h
#endif

// Anonymous type by which software watchdogs are referenced.
typedef void *SoftwareWatchdogHandle_t;

// Platform independent typedef for OS ticks. The C file will be implemented
// using platform-specific OS ticks.
typedef uint32_t Tick_t;

/**
 * Initialize the watchdog library.
 * @param refresh_hardware_watchdog: Function to refresh the hardware watchdog
 * @param timeout_callback: Callback function used to perform additional
 *        operations prior to the reset of the microcontroller. For example, a
 *        message may be written to a log file.
 */
void App_SharedSoftwareWatchdog_Init(
    void (*refresh_hardware_watchdog)(),
    void (*timeout_callback)(SoftwareWatchdogHandle_t));

/**
 * Allocate memory for a software watchdog (if there's space left).
 * @return Handle to the allocated software watchdog
 */
SoftwareWatchdogHandle_t App_SharedSoftwareWatchdog_AllocateWatchdog(void);

/**
 * Initialize a software watchdog. Once a software watchdog is initialized, it
 * is ready to monitor a periodic task.
 * @param sw_watchdog_handle: Handle to the software watchdog
 * @param name: Name of the software watchdog
 * @param period_in_ticks: Period of the task in OS ticks
 */
void App_SharedSoftwareWatchdog_InitWatchdog(
    SoftwareWatchdogHandle_t sw_watchdog_handle,
    char *                   name,
    Tick_t                   period_in_ticks);

/**
 * Every periodic task monitored by a software watchdog must call this at the
 * end of each period.
 * @param sw_watchdog_handle: Handle to the software watchdog
 */
void App_SharedSoftwareWatchdog_CheckInWatchdog(
    SoftwareWatchdogHandle_t sw_watchdog_handle);

/**
 * Check if any software watchdog has expired.
 * @note  If no software watchdog has expired, the hardware watchdog is
 *        refreshed. If any software watchdog has expired, the callback function
 *        is called and the hardware watchdog will no longer be refreshed.
 * @note  This function must be called periodically. It is good practice to call
 *        it from the system tick handler.
 */
void App_SharedSoftwareWatchdog_CheckForTimeouts(void);

/**
 * Get the name of a software watchdog.
 * @param sw_watchdog_handle: Handle to the software watchdog
 * @return Name of the software watchdog
 */
const char *App_SharedSoftwareWatchdog_GetName(
    SoftwareWatchdogHandle_t sw_watchdog_handle);
