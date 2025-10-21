#include "app/heartbeatMonitor.h"

void app_heartbeatMonitor_init(const HeartbeatMonitor *hbm)
{
    for (int i = 0; i < hbm->board_count; i++)
    {
        app_heartbeatBoard_init(&hbm->boards[i]);
    }
}

void app_heartbeatMonitor_checkIn(const HeartbeatMonitor *hbm)
{
    for (int i = 0; i < hbm->board_count; i++)
        app_heartbeatBoard_checkIn(&hbm->boards[i]);
}

void app_heartbeatMonitor_broadcastFaults(const HeartbeatMonitor *hbm)
{
    if (hbm->block_faults)
        return;
    for (int i = 0; i < hbm->board_count; i++)
        app_heartbeatBoard_broadcastFaults(&hbm->boards[i]);
}

bool app_heartbeatMonitor_isSendingMissingHeartbeatFault(const HeartbeatMonitor *hbm)
{
    for (int i = 0; i < hbm->board_count; i++)
        if (app_heartbeatBoard_isSendingMissingHeartbeatFault(&hbm->boards[i]))
            return true;
    return false;
}

#ifdef TARGET_TEST
void app_heartbeatMonitor_clearFaults(const HeartbeatMonitor *hbm)
{
    for (int i = 0; i < hbm->board_count; i++)
        app_heartbeatBoard_clearFaults(&hbm->boards[i]);
}
void app_heartbeatMonitor_blockFaults(HeartbeatMonitor *hbm, bool block_faults_init)
{
    hbm->block_faults = block_faults_init;
}
#endif
