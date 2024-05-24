#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "app_heartbeatBoardsEnum.h"

#define HEARTBEAT_MONITOR_TIMEOUT_PERIOD_MS 200U

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
