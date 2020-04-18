#include "Io_HeartbeatMonitor.h"

uint32_t Io_HeartbeatMonitor_GetCurrentMs(void)
{
    return xTaskGetTickCount() * portTICK_RATE_MS);
};