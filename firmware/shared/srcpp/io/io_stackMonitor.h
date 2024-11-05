#pragma once
#include "cmsis_os.h"

namespace io {
  class StackMonitor
  {
    osThreadId_t handle; // We can't use TaskHandle as a compile-time constant,
                          // so instead we use a pointer to it.
    float    watermark_threshold;
  public:
    explicit StackMonitor(osThreadId_t i_handle, const float i_watermark_threshold)
      : handle(i_handle), watermark_threshold(i_watermark_threshold){};
    [[nodiscard]] int stack_usage() const;
  };
}