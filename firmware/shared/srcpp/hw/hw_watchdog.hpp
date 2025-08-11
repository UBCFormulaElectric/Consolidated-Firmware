#pragma once
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

#include <cstdint>
#include <cassert>

namespace hw::watchdog
{
struct WatchdogInstance
{
    // Is this watchdog ready to be used?
    bool initialized;
    // The tick period of the task being monitored.
    uint32_t period;
    // The current deadline of the task being monitored.
    uint32_t deadline;
    // Has the task being monitored checked in for the current period?
    bool check_in_status;
    // ID to identify the task this watchdog monitors (for debugging only).
    uint8_t task_id;

    explicit WatchdogInstance(const uint8_t in_task_id, const uint32_t period_in_ticks)
      : initialized(true),
        period(period_in_ticks),
        deadline(period_in_ticks),
        check_in_status(true),
        task_id(in_task_id)
    {
    }

    /**
     * Every periodic task monitored by a software watchdog must call this at the
     * end of each period.
     * @param watchdog: Handle to the software watchdog
     */
    void checkIn()
    {
        assert(initialized == true);
        check_in_status = true;
    }
};
} // namespace hw::watchdog

/**
 * NOTE: The following functions must be implemented.
 * If you are getting linking issues with these functions make sure they are defined in hw/hw_wachdogs.cpp
 * TODO find a more elegant way of doing this?? I think this is pretty good
 */
namespace hw::watchdogConfig
{
/**
 * Function to refresh the hardware watchdog
 */
extern void refresh_hardware_watchdog();

/**
 * Callback function used to perform additional operations prior to the reset of the microcontroller.
 * For example, a message may be written to a log file.
 */
extern void timeout_callback(hw::watchdog::WatchdogInstance *watchdog);
} // namespace hw::watchdogConfig

namespace hw::watchdog::monitor
{
/**
 * Register a software watchdog instance to the monitor.
 * @param watchdog_instance - Handle to the software watchdog
 */
void registerWatchdogInstance(WatchdogInstance *watchdog_instance);

/**
 * Check if any software watchdog has expired.
 * @note  If no software watchdog has expired, the hardware watchdog is
 *        refreshed. If any software watchdog has expired, the callback function
 *        is called and the hardware watchdog will no longer be refreshed.
 * @note  This function must be called periodically. It is good practice to call
 *        it from the system tick handler.
 */
void checkForTimeouts();
} // namespace hw::watchdog::monitor