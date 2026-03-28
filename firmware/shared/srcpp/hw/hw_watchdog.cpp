#include "hw_watchdog.hpp"

void hw::watchdog::monitor::registerWatchdogInstance()
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
void hw::watchdog::monitor::checkForTimeouts()
        {
            // If a timeout is detected, let the hardware watchdog timeout reset the
            // system. We don't reboot immediately because we need some time to log
            // information for further debugging.
            for (WatchdogInstance *instance : watchdogs)
            {
                if (instance == nullptr)
                    continue;
                
                // Only check for timeout if the watchdog has been initialized
                if (!instance->initialized)
                    continue;

                if (osKernelGetTickCount() >= instance->deadline)
                {
                    // Check if the check-in status is set
                    if (instance->check_in_status)
                    {
                        // Clear the check-in status
                        instance->check_in_status = false;

                        // Update deadline
                        instance->deadline += instance->period;

                        refresh_hardware_watchdog();
                    }
                    else
                    {
                        timeout_detected = true;
                        if (timeout_callback != nullptr)
                        {
                            timeout_callback(instance);
                            break;
                        }
                    }
                }
            }
        }

 // namespace hw::watchdog
