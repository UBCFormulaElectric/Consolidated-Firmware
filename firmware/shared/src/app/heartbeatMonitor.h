#pragma once
#include <stdbool.h>
#include "app/heartbeatBoard.h"

typedef struct
{
    HeartbeatBoard *boards;
    uint8_t         board_count;
    bool            block_faults;
} HeartbeatMonitor;

void app_heartbeatMonitor_init(const HeartbeatMonitor *hbm);

void app_heartbeatMonitor_checkIn(const HeartbeatMonitor *hbm);

void app_heartbeatMonitor_broadcastFaults(const HeartbeatMonitor *hbm);

bool app_heartbeatMonitor_isSendingMissingHeartbeatFault(const HeartbeatMonitor *hbm);

#ifdef TARGET_TEST
void app_heartbeatMonitor_clearFaults(const HeartbeatMonitor *hbm);
void app_heartbeatMonitor_blockFaults(HeartbeatMonitor *hbm, bool block_faults_init);
#endif