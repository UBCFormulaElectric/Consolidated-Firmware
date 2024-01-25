#pragma once

#include <stdbool.h>
#include <stdint.h>

enum HeartbeatBoards
{
    BMS_HEARTBEAT_BOARD,
    DCM_HEARTBEAT_BOARD,
    PDM_HEARTBEAT_BOARD,
    FSM_HEARTBEAT_BOARD,
    DIM_HEARTBEAT_BOARD,
    HEARTBEAT_BOARD_COUNT
};

struct HeartbeatMonitor
{
    uint32_t (*get_current_ms)(void);
    uint32_t timeout_period_ms;
    uint32_t previous_timeout_ms;
    bool     heartbeats_checked_in[HEARTBEAT_BOARD_COUNT];
    bool     heartbeats_to_check[HEARTBEAT_BOARD_COUNT];
    bool     status[HEARTBEAT_BOARD_COUNT];

    // getters for other heartbeats
    bool (*getters[HEARTBEAT_BOARD_COUNT])();

    // updaters on the local CAN table for other heartbeats
    void (*updaters[HEARTBEAT_BOARD_COUNT])(bool);

    // setter for own heartbeat
    void (*setter)(bool);

    // setters for faults
    void (*fault_setters[HEARTBEAT_BOARD_COUNT])(bool);

    // getters for faults
    bool (*fault_getters[HEARTBEAT_BOARD_COUNT])();
};

struct HeartbeatMonitor* App_SharedHeartbeatMonitor_Create(
    uint32_t (*get_current_ms)(void),
    uint32_t timeout_period_ms,
    bool     boards_to_check[HEARTBEAT_BOARD_COUNT],
    bool     (*heartbeat_getters[HEARTBEAT_BOARD_COUNT])(),
    void     (*heartbeat_updaters[HEARTBEAT_BOARD_COUNT])(bool),
    void     (*heartbeat_setter)(bool),
    void     (*heartbeat_fault_setters[HEARTBEAT_BOARD_COUNT])(bool),
    bool     (*heartbeat_fault_getters[HEARTBEAT_BOARD_COUNT])());

void App_SharedHeartbeatMonitor_Destroy(struct HeartbeatMonitor* heartbeat_monitor);

void App_SharedHeartbeatMonitor_Tick(struct HeartbeatMonitor* heartbeat_monitor);

void App_SharedHeartbeatMonitor_BroadcastFaults(struct HeartbeatMonitor* const heartbeat_monitor);

void App_SharedHeartbeatMonitor_CheckIn(struct HeartbeatMonitor* const heartbeat_monitor);

bool App_SharedHeartbeatMonitor_CheckFaults(struct HeartbeatMonitor* const heartbeat_monitor);