#pragma once

#include <stdint.h>
#include "App_SharedHeartbeatMonitor.h"

uint32_t Io_HeartbeatMonitor_GetCurrentMs(void);
void     Io_HeartbeatMonitor_TimeoutCallback(
        enum HeartbeatOneHot heartbeats_to_check,
        enum HeartbeatOneHot heartbeats_checked_in);
