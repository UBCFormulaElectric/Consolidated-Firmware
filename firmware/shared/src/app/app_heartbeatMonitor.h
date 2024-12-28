#pragma once
#include <stdbool.h>
#include "app_heartbeatMonitorBoard.h"

typedef struct
{
  HeartbeatMonitorBoard * boards;
  uint8_t board_count;
  bool block_faults;
  void (*self_heartbeater)(bool);
} HeartbeatMonitor;

void app_heartbeatMonitor_checkIn(const HeartbeatMonitor * hbm);

void app_heartbeatMonitor_broadcastFaults(const HeartbeatMonitor* hbm);

bool app_heartbeatMonitor_isSendingMissingHeartbeatFault(const HeartbeatMonitor* hbm);

#ifdef TARGET_TEST
void app_heartbeatMonitor_clearFaults(const HeartbeatMonitor* hbm);
void app_heartbeatMonitor_blockFaults(bool block_faults);
#endif