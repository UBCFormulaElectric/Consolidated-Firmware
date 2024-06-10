#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "app_heartbeatBoardsEnum.h"

#define HEARTBEAT_MONITOR_TIMEOUT_PERIOD_MS 200U

typedef struct
{
    // CONFIG SETTINGS

    // self_checkin for own heartbeat
    void (*self_checkin)(bool);
    // Override to block heartbeat faults during tests.
    bool block_faults;

    // determines if the heartbeat monitor should check in on a board
    bool is_watching_heartbeat_for[HEARTBEAT_BOARD_COUNT];

    // gives if the heartbeat is checked in.
    // unmonitored heartbeats should be false.
    bool heartbeats_checked_in[HEARTBEAT_BOARD_COUNT];
    // gives if the heartbeat is in a valid state.
    // a flag in status is true under three conditions
    // 1) if it has been checked in
    // 2) it was not on our list of heartbeats to check
    // 3) if the heartbeat_checked_in is true but the timeout has not been elapsed.
    bool status[HEARTBEAT_BOARD_COUNT];

    // HEARTBEAT CAN SIGNALS

    // getters for heartbeats on the CAN table
    bool (*getters[HEARTBEAT_BOARD_COUNT])(void);
    // resetters on the local CAN table for other heartbeats
    // WARNING: only pass false into this function. idk how to make a closure, so we can't make this only return false
    void (*resetters[HEARTBEAT_BOARD_COUNT])(bool);

    // FAULT SETGET

    // fault broadcasters for each board's heartbeat from this board
    void (*fault_setters[HEARTBEAT_BOARD_COUNT])(bool);
    // fault getters for each board's heartbeat from this board
    bool (*fault_getters[HEARTBEAT_BOARD_COUNT])(void);
} HeartbeatMonitor;

void app_heartbeatMonitor_init(
    const bool boards_to_check[HEARTBEAT_BOARD_COUNT],
    bool (*const getters[HEARTBEAT_BOARD_COUNT])(),
    void (*const updaters[HEARTBEAT_BOARD_COUNT])(bool),
    void (*self_checkiner)(bool),
    void (*const fault_setters[HEARTBEAT_BOARD_COUNT])(bool),
    bool (*const fault_getters[HEARTBEAT_BOARD_COUNT])());

/**
 * Populates heartbeats_checked_in
 */
void app_heartbeatMonitor_checkIn(void);

/**
 * Gets state to broadcast via can, and can callbacks to use to broadcast
 */
void app_heartbeatMonitor_broadcastFaults(void);

/**
 * @return Whether the heartbeat monitor for the current board has detected any fault
 */
bool app_heartbeatMonitor_isSendingMissingHeartbeatFault(void);

/**
 * Blocks faults from being reported in app_heartbeatMonitor_isSendingMissingHeartbeatFault
 * @param block_faults Whether to block faults
 */
void app_heartbeatMonitor_blockFaults(bool block_faults);

/**
 * Resets faults as to report as false, useful for test environments
 */
void app_heartbeatMonitor_clearFaults(void);

#ifdef TARGET_TEST
/**
 * Get reference to heartbeat monitor, for testing.
 */
HeartbeatMonitor *app_heartbeatMonitor_get(void);
#endif
