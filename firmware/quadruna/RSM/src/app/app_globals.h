#pragma once

#include "App_SharedHeartbeatMonitor.h"

typedef struct
{
    struct HeartbeatMonitor *heartbeat_monitor;
} Globals;

extern Globals *const globals;