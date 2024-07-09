#pragma once

#include "io_heartbeatNode.h"

namespace app::heartbeat
{
// constexpr unsigned int HEARTBEAT_MONITOR_TIMEOUT_PERIOD_MS = 200U;
template <size_t NODE_COUNT> class monitor
{
    const std::array<const io::heartbeat::node *const, NODE_COUNT> heartbeat_nodes{};
    void (*const checkin_self)(bool);
    bool block_faults = false;

  public:
    explicit monitor(
        void (*const in_checkin_self)(bool),
        const std::array<const io::heartbeat::node *const, NODE_COUNT> in_heartbeat_nodes)
      : heartbeat_nodes(std::move(in_heartbeat_nodes)), checkin_self(in_checkin_self){};
    explicit monitor(
        void (*const in_checkin_self)(bool),
        const std::array<const io::heartbeat::node *const, NODE_COUNT> in_heartbeat_nodes,
        bool                                                           in_block_faults)
      : block_faults(in_block_faults), heartbeat_nodes(std::move(in_heartbeat_nodes)), checkin_self(in_checkin_self){};

    /**
     * Populates heartbeats_checked_in
     */
    void checkIn() const {}

    /**
     * Gets state to broadcast via can, and can callbacks to use to broadcast
     */
    void broadcastFaults() const {}

    /**
     * @return Whether the heartbeat monitor for the current board has detected any fault
     */
    [[nodiscard]] bool isSendingMissingHeartbeatFault() const { return false; }

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