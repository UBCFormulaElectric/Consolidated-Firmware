#pragma once
#include <stdbool.h>

void app_heartbeatMonitorConfig_init(bool block_faults);

void app_heartbeatMonitorConfig_checkin(void);
