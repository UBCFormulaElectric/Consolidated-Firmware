#pragma once

#include "Io_SharedSoftwareWatchdog.h"

void io_watchdogConfig_refresh(void);
void io_watchdogConfig_timeoutCallback(SoftwareWatchdogHandle_t sw_watchdog_handle);
