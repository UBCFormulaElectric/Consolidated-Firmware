#pragma once
#include <stdbool.h>

void app_heartbeatMonitor_init(bool block_faults);

void app_heartbeatMonitor_checkIn(void);

void app_heartbeatMonitor_broadcastFaults(void);

bool app_heartBeatMonitor_isSendingMissingHeartbeatFault(void);
