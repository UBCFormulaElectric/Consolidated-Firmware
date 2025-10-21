#pragma once
#include "app/timer.hpp"

namespace io::heartbeat
{
class node
{
#define HEARTBEAT_MONITOR_TIMEOUT_PERIOD_MS 200U
  public:
    // gives if the heartbeat is checked in.
    // unmonitored heartbeats should be false.
    bool heartbeats_checked_in = false;
    // gives if the heartbeat is in a valid state.
    // a flag in status is true under three conditions
    // 1) if it has been checked in
    // 2) it was not on our list of heartbeats to check
    // 3) if the heartbeat_checked_in is true but the timeout has not been elapsed.
    bool status = false;

    app::Timer timer{ HEARTBEAT_MONITOR_TIMEOUT_PERIOD_MS };

    // HEARTBEAT CAN SIGNALS
    // getters for heartbeats on the CAN table
    bool (*const getter)();
    // resetters on the local CAN table for other heartbeats
    void (*const resetter)();

    // FAULT SETGET
    // fault getters for each board's heartbeat from this board
    bool (*const fault_getter)();
    // fault broadcasters for each board's heartbeat from this board
    void (*const fault_setter)(bool);

    explicit node(
        bool (*const in_getter)(),
        void (*const in_resetter)(),
        bool (*const in_fault_getter)(),
        void (*const in_fault_setter)(bool))
      : getter(in_getter), resetter(in_resetter), fault_getter(in_fault_getter), fault_setter(in_fault_setter){};
};
} // namespace io::heartbeat
