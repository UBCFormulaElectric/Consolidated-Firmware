#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "app_timer.h"

typedef struct
{
    // gives if the heartbeat is checked in.
    // unmonitored heartbeats should be false.
    bool heartbeat_checked_in;
    // gives if the heartbeat is in a valid state.
    // a flag in status is true under three conditions
    // 1) if it has been checked in
    // 2) it was not on our list of heartbeats to check
    // 3) if the heartbeat_checked_in is true but the timeout has not been elapsed.
    bool         status;
    TimerChannel timer;

    // PUBLIC ELEMENTS
    const uint32_t timeout_ms;

    // HEARTBEAT CAN SIGNALS
    // getters for heartbeats on the CAN table
    bool (*const getter)(void);
    // resetters on the local CAN table for other heartbeats
    // WARNING: only pass false into this function. idk how to make a closure, so we can't make this only return false
    void (*const resetter)(bool);

    // FAULT SETGET
    // fault broadcasters for each board's heartbeat from this board
    void (*const fault_setter)(bool);
    // fault getters for each board's heartbeat from this board
    bool (*const fault_getter)(void);
} HeartbeatBoard;

void app_heartbeatBoard_init(HeartbeatBoard *hb);

/**
 * Populates heartbeats_checked_in
 */
void app_heartbeatBoard_checkIn(HeartbeatBoard *hb);

/**
 * Gets state to broadcast via can, and can callbacks to use to broadcast
 */
void app_heartbeatBoard_broadcastFaults(const HeartbeatBoard *hb);

/**
 * @return Whether the heartbeat monitor for the current board has detected any fault
 */
bool app_heartbeatBoard_isSendingMissingHeartbeatFault(const HeartbeatBoard *hb);

#ifdef TARGET_TEST
/**
 * Resets faults as to report as false, useful for test environments
 */
void app_heartbeatBoard_clearFaults(const HeartbeatBoard *hb);
#endif
