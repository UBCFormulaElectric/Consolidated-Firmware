#pragma once

#include "app_heartbeatMonitor.hpp"

extern const app::heartbeat::monitor<5> hb_monitor;

extern io::heartbeat::node bms_heartbeat_node;
extern io::heartbeat::node dam_heartbeat_node;
extern io::heartbeat::node fsm_heartbeat_node;
extern io::heartbeat::node rsm_heartbeat_node;
extern io::heartbeat::node vc_heartbeat_node;