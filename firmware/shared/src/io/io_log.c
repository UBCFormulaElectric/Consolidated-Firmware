#include "io_log.h"
#include "app_canAlerts.h"
#define MAX_FAULT_COUNT 50

void LOG_ALL_FAULTS(void)
{
    Fault_Warning_Info buffer[MAX_FAULT_COUNT] = { 0 };
    const uint8_t      fault_count             = app_canAlerts_FaultInfo(buffer);
    LOG_INFO("==================================");
    LOG_INFO("All Faults:");
    for (uint8_t i = 0; i < fault_count; i++)
    {
        LOG_INFO("%s", buffer[i].name);
    }
    LOG_INFO("==================================");
}
