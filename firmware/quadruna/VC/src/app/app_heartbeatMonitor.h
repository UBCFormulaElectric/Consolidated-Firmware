#pragma once
#include <stdbool.h>

void app_heartbeatMonitorConfig_init(bool block_faults);

void app_heartbeatMonitorConfig_checkin(void);

void app_heartbeatMonitorConfig_broadcastFaults(void);

bool app_heartBeatMonitorConfig_isSendingMissingHeartbeatFault(void);
