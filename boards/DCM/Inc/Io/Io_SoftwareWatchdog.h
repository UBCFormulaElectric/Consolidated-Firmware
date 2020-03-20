#pragma once

#include "Io/Io_SharedSoftwareWatchdog.h"

void Io_HardwareWatchdog_Refresh(void);
void App_SoftwareWatchdog_TimeoutCallback(
    SoftwareWatchdogHandle_t sw_watchdog_handle);
