#pragma once

#include "Io_SharedSoftwareWatchdog.h"

void dev_watchdogConfig_refresh(void);
void dev_watchdogConfig_timeoutCallback(SoftwareWatchdogHandle_t sw_watchdog_handle);
