#include "app_heartbeatMonitor.h"

void app_heartbeatMonitor_checkIn(const HeartbeatMonitor *hbm)
{
    hbm->self_heartbeater(true);
    for (int i = 0; i < hbm->board_count; i++)
        app_heartbeatMonitorBoard_checkIn(&hbm->boards[i]);
}

void app_heartbeatMonitor_broadcastFaults(const HeartbeatMonitor *hbm)
{
    if (hbm->block_faults)
        return;
    for (int i = 0; i < hbm->board_count; i++)
        app_heartbeatMonitorBoard_broadcastFaults(&hbm->boards[i]);
}

bool app_heartbeatMonitor_isSendingMissingHeartbeatFault(const HeartbeatMonitor *hbm)
{
    for (int i = 0; i < hbm->board_count; i++)
        if (app_heartbeatMonitorBoard_isSendingMissingHeartbeatFault(&hbm->boards[i]))
            return true;
    return false;
}

#ifdef TARGET_TEST
void app_heartbeatMonitor_clearFaults(void)
{
    for (int i = 0; i < hbm->board_count; i++)
        app_heartbeatMOnitorBoard_clearFaults(&hbm->boards[i]);
}
void app_heartbeatMonitor_blockFaults(bool block_faults_init)
{
    block_faults = block_faults_init;
}
#endif
