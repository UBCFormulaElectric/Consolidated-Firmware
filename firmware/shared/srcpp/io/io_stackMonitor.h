#pragma once
#include "cmsis_os.h"

namespace io
{
class StackMonitor
{
    osThreadId_t handle; // We can't use TaskHandle as a compile-time constant,
                         // so instead we use a pointer to it.
    const float stack_size;
  public:
    explicit StackMonitor(osThreadId_t i_handle, const uint32_t i_stack_size) : handle(i_handle), stack_size(static_cast<float>(i_stack_size)) {}

    /**
    * Returns stack usage for in percentage between 0-1
    */
    [[nodiscard]] float stack_usage() const;
};
} // namespace io