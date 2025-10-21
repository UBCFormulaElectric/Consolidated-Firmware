#include "io/time.hpp"
#include <cmsis_os.h>

namespace io::time
{
uint32_t getCurrentMs()
{
    return osKernelGetTickCount() * portTICK_RATE_MS;
}
void delay(const uint32_t ms)
{
    osDelay(ms);
}
void delayUntil(const uint32_t time)
{
    osDelayUntil(time);
}
} // namespace io::time
