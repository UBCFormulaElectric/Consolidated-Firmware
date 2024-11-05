#include "io_stackMonitor.h"

namespace io
{
float StackMonitor::stack_usage() const
{
    return static_cast<float>(osThreadGetStackSpace(handle)) / static_cast<float>(osThreadGetStackSize(handle));
}
} // namespace io