#include <assert.h>
#include <malloc.h>
#include "../../Inc/App/App_CurrentMonitoring.h"

struct CurrentMonitoring {
    float (*get_AUX1_current)(void);
    float (*get_AUX2_current)(void);
    float (*get_LEFT_INVERTER_current)(void);
    float (*get_RIGHT_INVERTER_current)(void);
    float (*get_ENERGY_METER_current)(void);
    float (*get_CAN_current)(void);
    float (*get_AIR_SHUTDOWN_current)(void);
};

struct CurrentMonitoring *App_CurrentMonitoring_Create(
    float (*get_AUX1_current)(void),
    float (*get_AUX2_current)(void),
    float (*get_LEFT_INVERTER_current)(void),
    float (*get_RIGHT_INVERTER_current)(void),
    float (*get_ENERGY_METER_current)(void),
    float (*get_CAN_current)(void),
    float (*get_AIR_SHUTDOWN_current)(void))
{
    struct CurrentMonitoring *current_monitor = malloc(sizeof(struct CurrentMonitoring));
    assert(current_monitor != NULL);

    current_monitor->get_AUX1_current               = get_AUX1_current;
    current_monitor->get_AUX2_current               = get_AUX2_current;
    current_monitor->get_LEFT_INVERTER_current      = get_LEFT_INVERTER_current;
    current_monitor->get_RIGHT_INVERTER_current     = get_RIGHT_INVERTER_current;
    current_monitor->get_ENERGY_METER_current       = get_ENERGY_METER_current;
    current_monitor->get_CAN_current                = get_CAN_current;
    current_monitor->get_AIR_SHUTDOWN_current       = get_AIR_SHUTDOWN_current;

    return current_monitor;
}

void App_CurrentMonitoring_Destroy(struct CurrentMonitoring *current_monitor)
{
    free(current_monitor);
}

float App_CurrentMonitoring_Get_AUX1_Current(struct CurrentMonitoring *current_monitor)
{
    return current_monitor->get_AUX1_current();
}

float App_CurrentMonitoring_Get_AUX2_Current(struct CurrentMonitoring *current_monitor)
{
    return current_monitor->get_AUX2_current();
}

float App_CurrentMonitoring_Get_LEFT_INVERTER_Current(struct CurrentMonitoring *current_monitor)
{
    return current_monitor->get_LEFT_INVERTER_current();
}

float App_CurrentMonitoring_Get_RIGHT_INVERTER_Current(struct CurrentMonitoring *current_monitor)
{
    return current_monitor->get_RIGHT_INVERTER_current();
}

float App_CurrentMonitoring_Get_ENERGY_METER_Current(struct CurrentMonitoring *current_monitor)
{
    return current_monitor->get_ENERGY_METER_current();
}

float App_CurrentMonitoring_Get_CAN_Current(struct CurrentMonitoring *current_monitor)
{
    return current_monitor->get_CAN_current();
}

float App_CurrentMonitoring_Get_AIR_SHUTDOWN_Current(struct CurrentMonitoring *current_monitor)
{
    return current_monitor->get_AIR_SHUTDOWN_current();
}

