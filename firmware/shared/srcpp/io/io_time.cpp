#include "io_time.h"
#include <cmsis_os.h>

namespace io::time
{
uint32_t getCurrentMs()
{
    return osKernelGetTickCount() * portTICK_RATE_MS;
}
} // namespace io::time