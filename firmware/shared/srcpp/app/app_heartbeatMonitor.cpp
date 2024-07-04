#include "app_heartbeatMonitor.h"

namespace app::heartbeat
{
template <size_t N> void monitor<N>::checkIn() const {}

template <size_t N> void monitor<N>::broadcastFaults() const {}

template <size_t N> bool monitor<N>::isSendingMissingHeartbeatFault() const
{
    return false;
}

template <size_t N> void monitor<N>::setBlockFaults(bool new_block_faults)
{
    block_faults = new_block_faults;
}

template <size_t N> void monitor<N>::clearFaults() const {}
} // namespace app::heartbeat
