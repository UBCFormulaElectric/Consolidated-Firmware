#pragma once

#include "hw_watchdog.h"

void hw_watchdogConfig_refresh(void);
void hw_watchdogConfig_timeoutCallback(SoftwareWatchdogHandle_t sw_watchdog_handle);
