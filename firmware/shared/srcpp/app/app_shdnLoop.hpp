#pragma once

#include <array>
#include "io_shdnLoopNode.hpp"

namespace app::shdn
{
template <std::size_t NODES_LEN> class shdnLoop
{
    const std::array<const io::shdn::node *const, NODES_LEN> nodes;

  public:
    explicit constexpr shdnLoop(const std::array<const io::shdn::node *const, NODES_LEN> in_nodes) : nodes(in_nodes) {}
    // Broadcast shutdown node statuses over CAN.
    void broadcast() const
    {
        for (const io::shdn::node *const &node : nodes)
            node->broadcast();
    }
};
} // namespace app::shdn
