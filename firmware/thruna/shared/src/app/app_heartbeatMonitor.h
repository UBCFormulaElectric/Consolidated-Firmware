#pragma once

#include <stdbool.h>
#include <stdint.h>

#define HEARTBEAT_MONITOR_TIMEOUT_PERIOD_MS 300U

typedef enum
{
    BMS_HEARTBEAT_BOARD,
    DCM_HEARTBEAT_BOARD,
    PDM_HEARTBEAT_BOARD,
    FSM_HEARTBEAT_BOARD,
    DIM_HEARTBEAT_BOARD,
    HEARTBEAT_BOARD_COUNT
} HeartbeatBoards;

typedef struct
{
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

    // Override to block heartbeat faults during tests.
    bool block_faults;
} HeartbeatMonitor;

void app_heartbeatMonitor_init(
    bool boards_to_check[HEARTBEAT_BOARD_COUNT],
    bool (*heartbeat_getters[HEARTBEAT_BOARD_COUNT])(),
    void (*heartbeat_updaters[HEARTBEAT_BOARD_COUNT])(bool),
    void (*heartbeat_setter)(bool),
    void (*heartbeat_fault_setters[HEARTBEAT_BOARD_COUNT])(bool),
    bool (*heartbeat_fault_getters[HEARTBEAT_BOARD_COUNT])());

void app_heartbeatMonitor_tick(void);

void app_heartbeatMonitor_broadcastFaults(void);

void app_heartbeatMonitor_checkIn(void);

bool app_heartbeatMonitor_checkFaults(void);

void app_heartbeatMonitor_blockFaults(bool block_faults);

#ifdef TARGET_TEST
HeartbeatMonitor *app_heartbeatMonitor_get(void);
#endif