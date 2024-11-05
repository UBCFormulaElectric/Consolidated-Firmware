#pragma once
#include <array>

#include "io_stackMonitor.h"

namespace app
{
template <size_t T> class StackMonitor
{
  const std::array<io::StackMonitor, T> stack_monitors;
public:
  explicit StackMonitor(const std::array<io::StackMonitor, T> m_swms): stack_monitors(m_swms) {}

  void check()
  {
    for(const auto& swm: stack_monitors)
    {
      swm.stack_usage();
    }
  };
};
}