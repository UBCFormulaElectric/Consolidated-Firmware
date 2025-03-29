#pragma once

#include "hw_watchdog.h"

void hw_watchdogConfig_refresh(void);
void hw_watchdogConfig_timeoutCallback(WatchdogHandle *watchdog);
