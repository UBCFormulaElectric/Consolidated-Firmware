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
    return rail_monitor->get__22V_AUX_voltage();
}

int App_RailMonitoring_InRangeCheck(float value, float min_value, float max_value)
{
    int status;
    if (value < min_value)
        status = 1;

    else if (value > max_value)
        status = 2;

    else
        status = 0;

    return status;
}

bool *App_RailMonitoring_VBAT_VoltageCheck(struct RailMonitoring *rail_monitor)
{
    if (App_RailMonitoring_InRangeCheck(rail_monitor->get_VBAT_voltage(), VBAT_MIN_VOLTAGE, VBAT_MAX_VOLTAGE) == 0)
        return (bool*) true;
    return (bool*) false;
}

bool *App_RailMonitoring__24V_ACC_VoltageCheck(struct RailMonitoring *rail_monitor)
{
    if (App_RailMonitoring_InRangeCheck(rail_monitor->get__24V_ACC_voltage(), VBAT_MIN_VOLTAGE, VBAT_MAX_VOLTAGE) == 0)
        return (bool*) true;
    return (bool*) false;
}

bool *App_RailMonitoring__22V_AUX_VoltageCheck(struct RailMonitoring *rail_monitor)
{
    if (App_RailMonitoring_InRangeCheck(rail_monitor->get__22V_AUX_voltage(), VBAT_MIN_VOLTAGE, VBAT_MAX_VOLTAGE) == 0)
        return (bool*) true;
    return (bool*) false;
}

/*
bool App_RailMonitoring_AreVoltagesInRange(struct RailMonitoring *rail_monitor)
{
    if (App_RailMonitoring_VBAT_VoltageCheck(rail_monitor) &&
        App_RailMonitoring__24V_ACC_VoltageCheck(rail_monitor) &&
        App_RailMonitoring__22V_AUX_VoltageCheck(rail_monitor))
        return true;
    return false;
}
*/
