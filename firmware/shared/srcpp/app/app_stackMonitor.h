#pragma once
#include <array>
#include <cstdint>
#include "io_stackMonitor.h"

struct abc
{
    io::StackMonitor monitor;
    float            watermark_threshold;
    void (*broadcast_level)(uint32_t);
    void (*broadcast_ok)(bool);
};

namespace app
{
template <size_t T> class StackMonitor
{
    const std::array<abc, T> stack_monitors;

  public:
    explicit StackMonitor(const std::array<abc, T> m_swms) : stack_monitors(m_swms)
    {
        for (const auto &swm : stack_monitors)
        {
            assert(0.0f < swm.watermark_threshold && swm.watermark_threshold < 1.0f);
        }
    }

    void check()
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