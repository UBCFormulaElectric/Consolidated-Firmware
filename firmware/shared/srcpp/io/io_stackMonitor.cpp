#include <cassert>
#include "io_stackMonitor.h"

namespace io
{
int StackMonitor::stack_usage() const
{
    assert(0.0f < watermark_threshold && watermark_threshold < 1.0f);
    const float stack_high_watermark_percent = 1.0f -
        static_cast<float>(osThreadGetStackSpace(handle)) / static_cast<float>(osThreadGetStackSize(handle));
    // TODO determine stack size
    return stack_high_watermark_percent > watermark_threshold;
}


}