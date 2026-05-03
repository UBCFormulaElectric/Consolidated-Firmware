#pragma once

#include "io_heartbeatNode.hpp"
#include "app_timer.hpp"
#include <cassert>
#include <array>
#include <cstddef>
#include <functional>

namespace app::heartbeat
{
// constexpr unsigned int HEARTBEAT_MONITOR_TIMEOUT_PERIOD_MS = 200U;
template <size_t NODE_COUNT> class monitor
{
    const std::array<std::reference_wrapper<io::heartbeat::node>, NODE_COUNT> &heartbeat_nodes;
    void (*const checkin_self)(bool);
    bool block_faults;

    void init() const
    {
        assert(checkin_self != nullptr);
        for (const auto node : heartbeat_nodes)
        {
            node.get().fault_setter(true);
        }
    }

  public:
    constexpr monitor(
        void (*const in_checkin_self)(bool),
        const std::array<std::reference_wrapper<io::heartbeat::node>, NODE_COUNT> &in_heartbeat_nodes,
        const bool                                                                 in_block_faults = false)
      : heartbeat_nodes(in_heartbeat_nodes), checkin_self(in_checkin_self), block_faults(in_block_faults)
    {
        init();
    }

    /**
     * Populates heartbeats_checked_in
     */
    void checkIn() const
    {
        // check in current heartbeat
        checkin_self(true);

        for (auto node : heartbeat_nodes)
        {
            // check in, and reset board on local CAN table
            const bool              board_status_good = node.get().getter();
            const Timer::TimerState state             = node.get().timer.runIfCondition(!board_status_good);

            node.get().heartbeats_checked_in = board_status_good;
            node.get().status                = board_status_good || state == Timer::TimerState::RUNNING;
            node.get().resetter(false);
        }
    }

    /**
     * Gets state to broadcast via can, and can callbacks to use to broadcast
     */
    void broadcastFaults() const
    {
        if (block_faults)
            return;

        for (const auto node : heartbeat_nodes)
        {
            const bool board_has_fault = !node.get().status;
            node.get().fault_setter(board_has_fault);
        }
    }

    /**
     * Blocks faults from being reported in app_heartbeatMonitor_isSendingMissingHeartbeatFault
     * @param new_block_faults Whether to block faults
     */
    void setBlockFaults(const bool new_block_faults) { block_faults = new_block_faults; }

#ifdef TARGET_TEST
    /**
     * Resets faults as to report as false, useful for test environments
     */
    void clearFaults() const {}
#endif
};
} // namespace app::heartbeat