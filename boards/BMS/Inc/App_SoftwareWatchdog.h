#pragma once

#include "App_SharedSoftwareWatchdog.h"

void Io_HardwareWatchdog_Refresh(void);
void App_SoftwareWatchdog_TimeoutCallback(
    SoftwareWatchdogHandle_t sw_watchdog_handle);
