#include <cmsis_os.h>
#include "Io_SharedHeartbeatMonitor.h"

uint32_t Io_SharedHeartbeatMonitor_GetCurrentMs(void)
{
    return osKernelGetTickCount() * portTICK_RATE_MS;
}
