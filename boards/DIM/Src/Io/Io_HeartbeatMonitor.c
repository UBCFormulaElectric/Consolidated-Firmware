#include <cmsis_os.h>

#include "Io_HeartbeatMonitor.h"

uint32_t Io_HeartbeatMonitor_GetCurrentMs(void)
{
    return osKernelSysTick() * portTICK_RATE_MS;
}