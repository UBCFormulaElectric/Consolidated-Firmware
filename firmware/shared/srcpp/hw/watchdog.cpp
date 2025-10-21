#include "hw/watchdog.hpp"
#include "io/log.hpp"
#include "cmsis_os.h"

#include <array>

namespace hw::watchdog::monitor
{
#define MAX_WATCHDOG_INSTANCES 10
std::array<WatchdogInstance *, MAX_WATCHDOG_INSTANCES> watchdogs{ nullptr };
bool                                                   timeout_detected = false;

void registerWatchdogInstance(WatchdogInstance *watchdog_instance)
{
    for (WatchdogInstance *&instance : watchdogs)
    {
        if (instance == nullptr)
        {
            instance = watchdog_instance;
            return;
        }
    }
    LOG_ERROR("Failed to register watchdog instance. Maximum number of watchdog instances reached.");
}

/**
 * Check if any software watchdog has expired.
 * @note  If no software watchdog has expired, the hardware watchdog is
 *        refreshed. If any software watchdog has expired, the callback function
 *        is called and the hardware watchdog will no longer be refreshed.
 * @note  This function must be called periodically. It is good practice to call
 *        it from the system tick handler.
 */
void checkForTimeouts()
{
    // If a timeout is detected, let the hardware watchdog timeout reset the
    // system. We don't reboot immediately because we need some time to log
    // information for further debugging.
    for (WatchdogInstance *watchdog_instance : watchdogs)
    {
        // Only check for timeout if the watchdog has been initialized
        if (!watchdog_instance->initialized)
            continue;

        if (osKernelGetTickCount() >= watchdog_instance->deadline)
        {
            // Check if the check-in status is set
            if (watchdog_instance->check_in_status)
            {
                // Clear the check-in status
                watchdog_instance->check_in_status = false;

                // Update deadline
                watchdog_instance->deadline += watchdog_instance->period;

                hw::watchdogConfig::refresh_hardware_watchdog();
            }
            else
            {
                hw::watchdogConfig::timeout_callback(watchdog_instance);
                timeout_detected = true;
                break;
            }
        }
    }
}
} // namespace hw::watchdog::monitor