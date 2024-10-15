#pragma once
#include "hw_watchdogs.h"

namespace io::watchdog
{
    void watchdogInit();
    void registerWatchdogInstance (static const TickType_t period_ms, int RTOS_TASK_1HZ);
    void checkIn(hw::watchdog::WatchdogInstance *watchdog);
    void checkForTimeouts();
} //watchdog namespace