#include <assert.h>
#include <malloc.h>
#include "../../Inc/App/App_RailMonitoring.h"

struct RailMonitoring {
    float (*get_VBAT_voltage)(void);
    float (*get__24V_ACC_voltage)(void);
    float (*get__24V_AUX_voltage)(void);
};

struct RailMonitoring *App_RailMonitoring_Create(
    float (*get_VBAT_voltage)(void),
    float (*get__24V_ACC_voltage)(void),
    float (*get__24V_AUX_voltage)(void))
{
    struct RailMonitoring  *rail_monitor = malloc(sizeof(struct RailMonitoring));
    assert(rail_monitor != NULL);

    rail_monitor->get_VBAT_voltage          = get_VBAT_voltage;
    rail_monitor->get__24V_ACC_voltage      = get__24V_ACC_voltage;
    rail_monitor->get__24V_AUX_voltage      = get__24V_AUX_voltage;

    return rail_monitor;
}

void App_RailMonitoring_Destroy(struct RailMonitoring *rail_monitor)
{
    free(rail_monitor);
}

float App_RailMonitoring_Get_VBAT_Voltage(struct RailMonitoring *rail_monitor)
{
    return rail_monitor->get_VBAT_voltage();
}

float App_RailMonitoring_Get__24V_ACC_Voltage(struct RailMonitoring *rail_monitor)
{
    return rail_monitor->get__24V_ACC_voltage();
}

float App_RailMonitoring_Get__24V_AUX_Voltage(struct RailMonitoring *rail_monitor)
{
    return rail_monitor->get__24V_AUX_voltage();
}
