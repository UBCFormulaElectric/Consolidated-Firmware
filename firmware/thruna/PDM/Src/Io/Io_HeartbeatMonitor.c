#include <cmsis_os.h>

uint32_t Io_HeartbeatMonitor_GetCurrentMs(void)
{
    return osKernelGetTickCount() * portTICK_RATE_MS;
}
