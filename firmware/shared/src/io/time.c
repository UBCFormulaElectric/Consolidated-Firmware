#include "io/time.h"
#include <cmsis_os.h>

uint32_t io_time_getCurrentMs(void)
{
    return osKernelGetTickCount() * portTICK_RATE_MS;
}

void io_time_delay(const uint32_t ms)
{
    osDelay(ms);
}
