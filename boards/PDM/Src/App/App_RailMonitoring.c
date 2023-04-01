#include <assert.h>
#include <malloc.h>
#include <stdbool.h>
#include "configs/App_VoltageLimits.h"
#include "App/App_RailMonitoring.h"

struct RailMonitoring
{
    float (*get_vbat_voltage)(void);
    float (*get_24v_acc_voltage)(void);
    float (*get_22v_aux_voltage)(void);
};

struct RailMonitoring *App_RailMonitoring_Create(
    float (*get_vbat_voltage)(void),
    float (*get_24v_acc_voltage)(void),
    float (*get_22v_aux_voltage)(void))
{
    struct RailMonitoring *rail_monitor = malloc(sizeof(struct RailMonitoring));
    assert(rail_monitor != NULL);

    rail_monitor->get_vbat_voltage    = get_vbat_voltage;
    rail_monitor->get_24v_acc_voltage = get_24v_acc_voltage;
    rail_monitor->get_22v_aux_voltage = get_22v_aux_voltage;

    return rail_monitor;
}

void App_RailMonitoring_Destroy(struct RailMonitoring *rail_monitor)
{
    free(rail_monitor);
}

float App_RailMonitoring_GetVbatVoltage(struct RailMonitoring *rail_monitor)
{
    return rail_monitor->get_vbat_voltage();
}

float App_RailMonitoring_Get24VAccumulatorVoltage(struct RailMonitoring *rail_monitor)
{
    return rail_monitor->get_24v_acc_voltage();
}

float App_RailMonitoring_Get22VAuxiliaryVoltage(struct RailMonitoring *rail_monitor)
{
    return rail_monitor->get_24v_acc_voltage();
}

bool App_RailMonitoring_VbatVoltageLowCheck(struct RailMonitoring *rail_monitor)
{
    return rail_monitor->get_vbat_voltage() < VBAT_MIN_VOLTAGE;
}

bool App_RailMonitoring_VbatVoltageHighCheck(struct RailMonitoring *rail_monitor)
{
    return rail_monitor->get_vbat_voltage() > VBAT_MAX_VOLTAGE;
}

bool App_RailMonitoring_24VAccumulatorVoltageLowCheck(struct RailMonitoring *rail_monitor)
{
    return rail_monitor->get_24v_acc_voltage() < ACC_24V_MIN_VOLTAGE;
}

bool App_RailMonitoring_24VAccumulatorVoltageHighCheck(struct RailMonitoring *rail_monitor)
{
    return rail_monitor->get_24v_acc_voltage() > ACC_24V_MAX_VOLTAGE;
}

bool App_RailMonitoring_22VAuxiliaryVoltageLowCheck(struct RailMonitoring *rail_monitor)
{
    return rail_monitor->get_22v_aux_voltage() < AUX_22V_MIN_VOLTAGE;
}

bool App_RailMonitoring_22VAuxiliaryVoltageHighCheck(struct RailMonitoring *rail_monitor)
{
    return rail_monitor->get_22v_aux_voltage() > AUX_22V_MAX_VOLTAGE;
}
