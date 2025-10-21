#pragma once
#include <array>
#include <cstdint>
#include "io/stackMonitor.hpp"

namespace app
{
struct stack_monitor_config
{
    io::StackMonitor monitor;
    float            watermark_threshold;
    void (*broadcast_level)(uint32_t);
    void (*broadcast_ok)(bool);
};
template <size_t T> class StackMonitor
{
    const std::array<stack_monitor_config, T> stack_monitors;

  public:
    explicit StackMonitor(const std::array<stack_monitor_config, T> m_swms) : stack_monitors(m_swms)
    {
        for (const stack_monitor_config &swm : stack_monitors)
        {
            assert(0.0f < swm.watermark_threshold && swm.watermark_threshold < 1.0f);
            assert(swm.broadcast_level != nullptr);
            assert(swm.broadcast_ok != nullptr);
        }
    }

    void check() const
    {
        for (const auto &swm : stack_monitors)
        {
            const float usage = swm.monitor.stack_usage();
            swm.broadcast_level(static_cast<int>(usage) * 100);
            swm.broadcast_ok(usage <= swm.watermark_threshold);
        }
    };
};
} // namespace app