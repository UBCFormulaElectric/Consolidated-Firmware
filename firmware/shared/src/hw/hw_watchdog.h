/**
 * @brief Monitor task deadlines by assigning a software watchdog to each
 *        periodic task.
 * @note The current implementation doesn't really support monitoring
 *       "non-periodic" tasks (e.g. CAN TX).
 * @note Some hints for setting up the hardware watchdog in STM32CubeMX:
 *       - Use __HAL_DBGMCU_FREEZE_IWDG to stop the IWDG clock when the CPU is
 *         halted due to a breakpoint, or else the IWDG can likely trigger an
 *         undesirable system reboot.
 *       - Create User Constants in STM32CubeMX with the following values:
 *
 *             IWDG_WINDOW_DISABLE_VALUE = 4095
 *             LSI_FREQUENCY             = <LSI Clock Frequency in Hertz.
 *                                          For example, it is 40000 for
 *                                          STM32F302R8.>
 *             IWDG_PRESCALER            = 4
 *             IWDG_RESET_FREQUENCY      = 5
 *
 *         ...then set the following parameters for "Watchdog Clocking" under
 *         "Parameter Settings" for IWDG:
 *
 *            IWDG counter clock prescaler   = 4
 *            IWDG window value              = IWDG_WINDOW_DISABLE_VALUE
 *            IWDG down-counter reload value = LSI_FREQUENCY / IWDG_PRESCALER
 *                                             / IWDG_RESET_FREQUENCY
 *
 *         In case it was not clear, IWDG_RESET_FREQUENCY = 5 means that the
 *         IWDG has a frequency of 5Hz, or a period of 200ms.
 */
#pragma once

#include <FreeRTOS.h>
#include <stdbool.h>
#include <stdint.h>
#include <cmsis_os.h>

#define MAX_NUM_OF_SOFTWARE_WATCHDOG 10

typedef struct
{
    // Is this watchdog ready to be used?
    bool initialized;
    // Task ID that created this handle.
    xTaskHandle task;
    // The tick period of the task being monitored.
    uint32_t period;
    // The current deadline of the task being monitored.
    uint32_t deadline;
    // Has the task being monitored checked in for the current period?
    bool check_in_status;
} WatchdogHandle;

/**
 * Initialize a software watchdog. Once a software watchdog is initialized, it
 * is ready to monitor a periodic task.
 * @param watchdog: Handle to the software watchdog
 * @param task_id: ID to identify the task this watchdog is monitoring
 * @param period_ms: Period of the task in ms
 */
WatchdogHandle *hw_watchdog_initTask(uint32_t period_ms);

/**
 * Every periodic task monitored by a software watchdog must call this at the
 * end of each period.
 * @param watchdog: Handle to the software watchdog
 */
void hw_watchdog_checkIn(WatchdogHandle *watchdog);

/**
 * Check if any software watchdog has expired.
 * @note  If no software watchdog has expired, the hardware watchdog is
 *        refreshed. If any software watchdog has expired, the callback function
 *        is called and the hardware watchdog will no longer be refreshed.
 * @note  This function must be called periodically. It is good practice to call
 *        it from the system tick handler.
 */
void hw_watchdog_checkForTimeouts(void);
