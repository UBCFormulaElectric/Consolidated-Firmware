#pragma once
#include <array>
#include <cstdint>
#include "io_stackMonitor.h"


struct abc
{
  io::StackMonitor monitor;
  void (*broadcast_level)(uint32_t);
  void (*broadcast_ok)(bool);
};

namespace app
{
template <size_t T> class StackMonitor
{
  const std::array<abc, T> stack_monitors;
public:
  explicit StackMonitor(const std::array<abc, T> m_swms): stack_monitors(m_swms) {}

  void check()
  {
    for(const auto& swm: stack_monitors)
    {
      swm.broadcast_level(static_cast<int>(swm.monitor.stack_usage() * 100));
      swm.broadcast_ok(swm.monitor.is_stack_ok());
    }
  };
};
}