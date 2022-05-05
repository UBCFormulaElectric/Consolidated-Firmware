#pragma once

#include <stdint.h>

// One-hot encoded heartbeat enum
enum HeartbeatOneHot
{
    BMS_HEARTBEAT_ONE_HOT = (1U << 0U),
    DCM_HEARTBEAT_ONE_HOT = (1U << 1U),
    PDM_HEARTBEAT_ONE_HOT = (1U << 2U),
    FSM_HEARTBEAT_ONE_HOT = (1U << 3U),
    DIM_HEARTBEAT_ONE_HOT = (1U << 4U),
};

struct HeartbeatMonitor *App_SharedHeartbeatMonitor_Create(
    uint32_t (*get_current_ms)(void),
    uint32_t             timeout_period_ms,
    enum HeartbeatOneHot boards_to_check,
    void (*timeout_callback)(enum HeartbeatOneHot, enum HeartbeatOneHot));

void App_SharedHeartbeatMonitor_Destroy(struct HeartbeatMonitor *heartbeat_monitor);

void App_SharedHeartbeatMonitor_Tick(struct HeartbeatMonitor *heartbeat_monitor);

void App_SharedHeartbeatMonitor_CheckIn(struct HeartbeatMonitor *heartbeat_monitor, enum HeartbeatOneHot heartbeat);
