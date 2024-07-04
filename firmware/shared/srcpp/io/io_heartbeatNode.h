#pragma once

#include <functional>
namespace io::heartbeat
{
class node
{
    // gives if the heartbeat is checked in.
    // unmonitored heartbeats should be false.
    bool heartbeats_checked_in = false;
    // gives if the heartbeat is in a valid state.
    // a flag in status is true under three conditions
    // 1) if it has been checked in
    // 2) it was not on our list of heartbeats to check
    // 3) if the heartbeat_checked_in is true but the timeout has not been elapsed.
    bool status = false;

    // HEARTBEAT CAN SIGNALS
    // getters for heartbeats on the CAN table
    const std::function<bool(void)> &getter;
    // resetters on the local CAN table for other heartbeats
    const std::function<void(void)> &resetter;

    // FAULT SETGET
    // fault broadcasters for each board's heartbeat from this board
    const std::function<void(bool)> &fault_setter;
    // fault getters for each board's heartbeat from this board
    const std::function<bool(void)> &fault_getter;

  public:
    explicit node(
        const std::function<bool(void)> &in_getter,
        const std::function<void(void)> &in_resetter,
        const std::function<void(bool)> &in_fault_setter,
        const std::function<bool(void)> &in_fault_getter)
      : getter(in_getter), resetter(in_resetter), fault_setter(in_fault_setter), fault_getter(in_fault_getter){};
};
} // namespace io::heartbeat
