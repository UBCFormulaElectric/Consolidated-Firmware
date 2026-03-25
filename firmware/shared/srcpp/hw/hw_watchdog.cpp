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

void hw::watchdog::monitor::checkForTimeouts()
        {
            // If a timeout is detected, let the hardware watchdog timeout reset the
            // system. We don't reboot immediately because we need some time to log
            // information for further debugging.
            for (watchdog_instance : watchdogs)
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

                        refresh_hardware_watchdog();
                    }
                    else
                    {
                        timeout_callback(watchdog_instance);
                        timeout_detected = true;
                        break;
                    }
                }
            }
        }

 // namespace hw::watchdog
