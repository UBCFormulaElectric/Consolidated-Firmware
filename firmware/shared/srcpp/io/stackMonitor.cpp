#include "io/stackMonitor.hpp"

namespace io
{
float StackMonitor::stack_usage() const
{
    // WARNING: osThreadGetStackSize is not implemented in FreeRTOS
    // https://github.com/ARM-software/CMSIS-FreeRTOS/blob/dd7793adcbea0c3c0f3524f86b031ab88b9e2193/DoxyGen/General/src/cmsis_freertos.txt#L312
    return static_cast<float>(osThreadGetStackSpace(*task_handle)) / stack_size;
}
} // namespace io