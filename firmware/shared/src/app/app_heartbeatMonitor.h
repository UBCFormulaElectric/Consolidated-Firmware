#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef enum
{
    BMS_HEARTBEAT_BOARD,
    DCM_HEARTBEAT_BOARD,
    PDM_HEARTBEAT_BOARD,
    FSM_HEARTBEAT_BOARD,
    DIM_HEARTBEAT_BOARD,
    HEARTBEAT_BOARD_COUNT
} HeartbeatBoards;

void app_heartbeatMonitor_init(
    uint32_t timeout_period_ms,
    bool     boards_to_check[HEARTBEAT_BOARD_COUNT],
    bool (*heartbeat_getters[HEARTBEAT_BOARD_COUNT])(),
    void (*heartbeat_updaters[HEARTBEAT_BOARD_COUNT])(bool),
    void (*heartbeat_setter)(bool),
    void (*heartbeat_fault_setters[HEARTBEAT_BOARD_COUNT])(bool),
    bool (*heartbeat_fault_getters[HEARTBEAT_BOARD_COUNT])());

void app_heartbeatMonitor_tick(void);

void app_heartbeatMonitor_broadcastFaults(void);

void app_heartbeatMonitor_checkIn(void);

bool app_heartbeatMonitor_checkFaults(void);