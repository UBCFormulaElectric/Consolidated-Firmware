#pragma once

#include "Io_SharedSoftwareWatchdog.h"

void Io_HardwareWatchdog_Refresh(void);
void Io_SoftwareWatchdog_TimeoutCallback(
    SoftwareWatchdogHandle_t sw_watchdog_handle);
