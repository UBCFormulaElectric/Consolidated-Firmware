#include <stdlib.h>

#include "App_VoltageMonitor.h"
#include "App_SharedAssert.h"

struct VoltageMonitor
{
    float (*get_voltage)(void);
    float (*get_min_voltage)(void);
    float (*get_max_voltage)(void);
    enum VoltageMonitor_Status status;
};

struct VoltageMonitor *App_VoltageMonitor_Create(
    float (*get_voltage)(void),
    float (*get_min_voltage)(void),
    float (*get_max_voltage)(void))
{
    struct VoltageMonitor *voltage_monitor =
        malloc(sizeof(struct VoltageMonitor));

    shared_assert(voltage_monitor != NULL);

    voltage_monitor->get_voltage     = get_voltage;
    voltage_monitor->get_min_voltage = get_min_voltage;
    voltage_monitor->get_max_voltage = get_max_voltage;
    voltage_monitor->status          = VOLTAGEMONITOR_IN_RANGE;

    return voltage_monitor;
}

void App_VoltageMonitor_Tick(struct VoltageMonitor *voltage_monitor)
{
    const float voltage     = voltage_monitor->get_voltage();
    const float min_voltage = voltage_monitor->get_min_voltage();
    const float max_voltage = voltage_monitor->get_max_voltage();

    if (voltage < min_voltage)
    {
        voltage_monitor->status = VOLTAGEMONITOR_UNDERVOLTAGE;
    }
    else if (voltage > max_voltage)
    {
        voltage_monitor->status = VOLTAGEMONITOR_OVERVOLTAGE;
    }
    else
    {
        voltage_monitor->status = VOLTAGEMONITOR_IN_RANGE;
    }
}

enum VoltageMonitor_Status
    App_VoltageMonitor_GetStatus(struct VoltageMonitor *voltage_monitor)
{
    return voltage_monitor->status;
}

void App_VoltageMonitor_Destroy(struct VoltageMonitor *voltage_monitor)
{
    free(voltage_monitor);
}
