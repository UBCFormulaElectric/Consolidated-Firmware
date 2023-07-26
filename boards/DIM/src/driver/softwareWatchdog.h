#pragma once

#include "Io_SharedSoftwareWatchdog.h"

void softwareWatchdog_init(IWDG_HandleTypeDef watchdog_handle);
void softwareWatchdog_refreshWatchdog(void);
void softwareWatchdog_timeoutCallback(SoftwareWatchdogHandle_t sw_watchdog_handle);
