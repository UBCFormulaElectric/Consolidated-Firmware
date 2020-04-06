#include <stdlib.h>

#include "App_VoltageMonitor.h"
#include "App_SharedAssert.h"

struct VoltageMonitor
{
    const char *name;
    float (*get_voltage)(void);
    float (*get_min_voltage)(void);
    float (*get_max_voltage)(void);
    enum VoltageMonitor_Status status;
    void (*error_callback)(struct VoltageMonitor *);
};

struct VoltageMonitor *App_VoltageMonitor_Create(
    const char *const name,
    float (*const get_voltage)(void),
    float (*const get_min_voltage)(void),
    float (*const get_max_voltage)(void),
    void (*error_callback)(struct VoltageMonitor *))
{
    struct VoltageMonitor *voltage_monitor =
        malloc(sizeof(struct VoltageMonitor));

    shared_assert(voltage_monitor != NULL);

    voltage_monitor->name            = name;
    voltage_monitor->get_voltage     = get_voltage;
    voltage_monitor->get_min_voltage = get_min_voltage;
    voltage_monitor->get_max_voltage = get_max_voltage;
    voltage_monitor->status          = VOLTAGEMONITOR_IN_RANGE;
    voltage_monitor->error_callback  = error_callback;

    return voltage_monitor;
}

void App_VoltageMonitor_Destroy(struct VoltageMonitor *const voltage_monitor)
{
    free(voltage_monitor);
}

void App_VoltageMonitor_Tick(struct VoltageMonitor *const voltage_monitor)
{
    const float voltage     = voltage_monitor->get_voltage();
    const float min_voltage = voltage_monitor->get_min_voltage();
    const float max_voltage = voltage_monitor->get_max_voltage();

    if (voltage < min_voltage)
    {
        voltage_monitor->status = VOLTAGEMONITOR_UNDERVOLTAGE;
        voltage_monitor->error_callback(voltage_monitor);
    }
    else if (voltage > max_voltage)
    {
        voltage_monitor->status = VOLTAGEMONITOR_OVERVOLTAGE;
        voltage_monitor->error_callback(voltage_monitor);
    }
    else
    {
        voltage_monitor->status = VOLTAGEMONITOR_IN_RANGE;
    }
}

enum VoltageMonitor_Status App_VoltageMonitor_GetStatus(
    const struct VoltageMonitor *const voltage_monitor)
{
    return voltage_monitor->status;
}

const char *App_VoltageMonitor_GetName(
    const struct VoltageMonitor *const voltage_monitor)
{
    return voltage_monitor->name;
}
