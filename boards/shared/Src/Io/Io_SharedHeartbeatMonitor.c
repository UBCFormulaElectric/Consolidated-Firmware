#include <cmsis_os.h>
#include "Io_SharedHeartbeatMonitor.h"

uint32_t Io_SharedHeartbeatMonitor_GetCurrentMs(void)
{
    return osKernelSysTick() * portTICK_RATE_MS;
}
