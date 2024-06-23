#include "app_heartbeatMonitorConfig.h"
#include "app_heartbeatMonitor.h"

// CAN
#include "app_canRx.h"
#include "app_canTx.h"
#include "app_canAlerts.h"

static HeartbeatMonitorBoard bms_hbmonitor = { .getter       = app_canRx_BMS_Heartbeat_get,
                                               .resetter     = app_canRx_BMS_Heartbeat_update,
                                               .fault_setter = app_canAlerts_VC_Fault_MissingBMSHeartbeat_set,
                                               .fault_getter = app_canAlerts_VC_Fault_MissingBMSHeartbeat_get,
                                               .timeout_ms   = 200 };

static HeartbeatMonitorBoard rsm_hbmonitor = { .getter       = app_canRx_RSM_Heartbeat_get,
                                               .resetter     = app_canRx_RSM_Heartbeat_update,
                                               .fault_setter = app_canAlerts_VC_Fault_MissingRSMHeartbeat_set,
                                               .fault_getter = app_canAlerts_VC_Fault_MissingRSMHeartbeat_get,
                                               .timeout_ms   = 200 };

static HeartbeatMonitorBoard fsm_hbmonitor = { .getter       = app_canRx_FSM_Heartbeat_get,
                                               .resetter     = app_canRx_FSM_Heartbeat_update,
                                               .fault_setter = app_canAlerts_VC_Fault_MissingFSMHeartbeat_set,
                                               .fault_getter = app_canAlerts_VC_Fault_MissingFSMHeartbeat_get,
                                               .timeout_ms   = 200 };

static HeartbeatMonitorBoard crit_hbmonitor = { .getter       = app_canRx_CRIT_Heartbeat_get,
                                                .resetter     = app_canRx_CRIT_Heartbeat_update,
                                                .fault_setter = app_canAlerts_VC_Fault_MissingCRITHeartbeat_set,
                                                .fault_getter = app_canAlerts_VC_Fault_MissingCRITHeartbeat_get,
                                                .timeout_ms   = 200 };

static bool block_faults = false;

void app_heartbeatMonitorConfig_init(bool block_faults_init)
{
    block_faults = block_faults_init;
    app_heartbeatMonitor_init(&bms_hbmonitor);
    app_heartbeatMonitor_init(&rsm_hbmonitor);
    app_heartbeatMonitor_init(&fsm_hbmonitor);
    app_heartbeatMonitor_init(&crit_hbmonitor);
}

void app_heartbeatMonitorConfig_checkin(void)
{
    app_canTx_VC_Heartbeat_set(true);
    app_heartbeatMonitor_checkIn(&bms_hbmonitor);
    app_heartbeatMonitor_checkIn(&rsm_hbmonitor);
    app_heartbeatMonitor_checkIn(&fsm_hbmonitor);
    app_heartbeatMonitor_checkIn(&crit_hbmonitor);
}

void app_heartbeatMonitorConfig_broadcastFaults(void)
{
    if (block_faults)
        return;
    app_heartbeatMonitor_broadcastFaults(&bms_hbmonitor);
    app_heartbeatMonitor_broadcastFaults(&rsm_hbmonitor);
    app_heartbeatMonitor_broadcastFaults(&fsm_hbmonitor);
    app_heartbeatMonitor_broadcastFaults(&crit_hbmonitor);
}

bool app_heartBeatMonitorConfig_isSendingMissingHeartbeatFault(void)
{
    return app_heartbeatMonitor_isSendingMissingHeartbeatFault(&bms_hbmonitor) ||
           app_heartbeatMonitor_isSendingMissingHeartbeatFault(&rsm_hbmonitor) ||
           app_heartbeatMonitor_isSendingMissingHeartbeatFault(&fsm_hbmonitor) ||
           app_heartbeatMonitor_isSendingMissingHeartbeatFault(&crit_hbmonitor);
}
