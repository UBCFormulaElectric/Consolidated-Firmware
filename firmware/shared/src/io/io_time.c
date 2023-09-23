#include "io_time.h"
#include <cmsis_os.h>

uint32_t io_time_getCurrentMs(void)
{
    return osKernelSysTick() * portTICK_RATE_MS;
}
