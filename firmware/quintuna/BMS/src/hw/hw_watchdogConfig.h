#pragma once

#include "hw_watchdog.h"
#include "main.h"

void hw_watchdogConfig_refresh(void);
void hw_watchdogConfig_timeoutCallback(WatchdogHandle *watchdog);
