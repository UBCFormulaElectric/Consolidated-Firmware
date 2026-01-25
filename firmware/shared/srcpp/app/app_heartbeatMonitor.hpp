#pragma once

#include "io_heartbeatNode.hpp"
#include "app_timer.hpp"
#include <cassert>
#include <array>

namespace app::heartbeat
{
// constexpr unsigned int HEARTBEAT_MONITOR_TIMEOUT_PERIOD_MS = 200U;
template <size_t NODE_COUNT> class monitor
{
    const std::array<io::heartbeat::node *const, NODE_COUNT> heartbeat_nodes{};
    void (*const checkin_self)(bool);
    bool block_faults = false;

    void init()
    {
        assert(checkin_self != NULL);
        for (io::heartbeat::node *const node : heartbeat_nodes)
        {
            node->fault_setter(true);
        }
    }

  public:
    explicit monitor(
        void (*const in_checkin_self)(bool),
        const std::array<io::heartbeat::node *const, NODE_COUNT> in_heartbeat_nodes)
      : heartbeat_nodes(std::move(in_heartbeat_nodes)), checkin_self(in_checkin_self)
    {
        init();
    };
    explicit monitor(
        void (*const in_checkin_self)(bool),
        const std::array<io::heartbeat::node *const, NODE_COUNT> in_heartbeat_nodes,
        bool                                                     in_block_faults)
      : block_faults(in_block_faults), heartbeat_nodes(std::move(in_heartbeat_nodes)), checkin_self(in_checkin_self)
    {
        init();
    };

    /**
     * Populates heartbeats_checked_in
     */
    void checkIn() const
    {
        // check in current heartbeat
        checkin_self(true);

        for (io::heartbeat::node *const node : heartbeat_nodes)
        {
            // check in, and reset board on local CAN table
            const bool              board_status_good = node->getter();
            const Timer::TimerState state             = node->timer.runIfCondition(!board_status_good);

            node->heartbeats_checked_in = board_status_good;
            node->status                = board_status_good || state == Timer::TimerState::RUNNING;
            node->resetter(false);
        }
    }

    /**
     * Gets state to broadcast via can, and can callbacks to use to broadcast
     */
    void broadcastFaults() const
    {
        if (block_faults)
            return;

        for (const io::heartbeat::node *const node : heartbeat_nodes)
        {
            const bool board_has_fault = !node->status;
            node->fault_setter(board_has_fault);
        }
    }

    /**
     * @return Whether the heartbeat monitor for the current board has detected any fault
     */
    [[nodiscard]] bool isSendingMissingHeartbeatFault() const
    {
        if (block_faults)
        {
            return false;
        }

        for (io::heartbeat::node *const node : heartbeat_nodes)
        {
            if (node->fault_getter())
            {
                return true;
            }
        }
        return false;
    }

    /**
     * Blocks faults from being reported in app_heartbeatMonitor_isSendingMissingHeartbeatFault
     * @param block_faults Whether to block faults
     */
    void setBlockFaults(bool new_block_faults) { block_faults = new_block_faults; }

    /**
     * Resets faults as to report as false, useful for test environments
     */
    void clearFaults() const {}
};
} // namespace app::heartbeat