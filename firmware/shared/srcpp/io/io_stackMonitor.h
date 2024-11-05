#pragma once
#include "cmsis_os.h"

namespace io
{
class StackMonitor
{
    osThreadId_t handle; // We can't use TaskHandle as a compile-time constant,
                         // so instead we use a pointer to it.
  public:
    explicit StackMonitor::StackMonitor(osThreadId_t i_handle) : handle(i_handle) {}
    [[nodiscard]] float    stack_usage() const;
};
} // namespace io