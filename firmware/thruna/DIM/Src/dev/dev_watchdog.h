#pragma once

#include "Io_SharedSoftwareWatchdog.h"

void dev_watchdog_feedWatchdog(void);
void dev_watchdog_timeoutCallback(SoftwareWatchdogHandle_t sw_watchdog_handle);
