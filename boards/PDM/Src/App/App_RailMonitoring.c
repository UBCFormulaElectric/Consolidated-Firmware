#include <assert.h>
#include <malloc.h>
#include <stdbool.h>
#include "/home/formulae/Documents/Consolidated-Firmware/boards/PDM/Inc/App/configs/App_VoltageLimits.h"
#include "../../Inc/App/App_RailMonitoring.h"

struct RailMonitoring
{
    float (*get_VBAT_voltage)(void);
    float (*get__24V_ACC_voltage)(void);
    float (*get__22V_AUX_voltage)(void);
};

struct RailMonitoring *App_RailMonitoring_Create(
    float (*get_VBAT_voltage)(void),
    float (*get__24V_ACC_voltage)(void),
    float (*get__22V_AUX_voltage)(void))
{
    struct RailMonitoring *rail_monitor = malloc(sizeof(struct RailMonitoring));
    assert(rail_monitor != NULL);

    rail_monitor->get_VBAT_voltage     = get_VBAT_voltage;
    rail_monitor->get__24V_ACC_voltage = get__24V_ACC_voltage;
    rail_monitor->get__22V_AUX_voltage = get__22V_AUX_voltage;

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

float App_RailMonitoring_Get__22V_AUX_Voltage(struct RailMonitoring *rail_monitor)
{
    return rail_monitor->get__24V_ACC_voltage();
}

bool App_RailMonitoring_VBAT_VoltageLowCheck(struct RailMonitoring *rail_monitor)
{
    return rail_monitor->get_VBAT_voltage() < VBAT_MIN_VOLTAGE;
}

bool App_RailMonitoring_VBAT_VoltageHighCheck(struct RailMonitoring *rail_monitor)
{
    return rail_monitor->get_VBAT_voltage() > VBAT_MAX_VOLTAGE;
}

bool App_RailMonitoring_VBAT_VoltageCriticalCheck(struct RailMonitoring *rail_monitor)
{
    return rail_monitor->get_VBAT_voltage() > VBAT_CRITICAL_VOLTAGE;
}

bool App_RailMonitoring__24V_ACC_VoltageLowCheck(struct RailMonitoring *rail_monitor)
{
    return rail_monitor->get__24V_ACC_voltage() < _24V_ACC_MIN_VOLTAGE;
}

bool App_RailMonitoring__24V_ACC_VoltageHighCheck(struct RailMonitoring *rail_monitor)
{
    return rail_monitor->get__24V_ACC_voltage() > _24V_ACC_MAX_VOLTAGE;
}

bool App_RailMonitoring__24V_ACC_VoltageCriticalCheck(struct RailMonitoring *rail_monitor)
{
    return rail_monitor->get__24V_ACC_voltage() > _24V_ACC_CRITICAL_VOLTAGE;
}

bool App_RailMonitoring__22V_AUX_VoltageLowCheck(struct RailMonitoring *rail_monitor)
{
    return rail_monitor->get__22V_AUX_voltage() < _22V_AUX_MIN_VOLTAGE;
}

bool App_RailMonitoring__22V_AUX_VoltageHighCheck(struct RailMonitoring *rail_monitor)
{
    return rail_monitor->get__22V_AUX_voltage() > _22V_AUX_MAX_VOLTAGE;
}

bool App_RailMonitoring__22V_AUX_VoltageCriticalCheck(struct RailMonitoring *rail_monitor)
{
    return rail_monitor->get__22V_AUX_voltage() > _22V_AUX_CRITICAL_VOLTAGE;
}
