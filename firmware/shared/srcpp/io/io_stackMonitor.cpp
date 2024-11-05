#include <cassert>
#include "io_stackMonitor.h"

namespace io
{
StackMonitor::StackMonitor(osThreadId_t i_handle, const float i_watermark_threshold)
  : handle(i_handle), watermark_threshold(i_watermark_threshold)
{
    assert(0.0f < watermark_threshold && watermark_threshold < 1.0f);
}

float StackMonitor::stack_usage() const
{
    return static_cast<float>(osThreadGetStackSpace(handle)) / static_cast<float>(osThreadGetStackSize(handle));
}
bool StackMonitor::is_stack_ok() const
{
    return stack_usage() > watermark_threshold;
}
} // namespace io