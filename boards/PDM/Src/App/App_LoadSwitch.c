#include <assert.h>
#include <malloc.h>
#include "../../Inc/App/App_LoadSwitch.h"

struct LoadSwitch {
    float (*get_AUX1_current)(void);
    float (*get_AUX2_current)(void);
    float (*get_LEFT_INVERTER_current)(void);
    float (*get_RIGHT_INVERTER_current)(void);
    float (*get_ENERGY_METER_current)(void);
    float (*get_CAN_current)(void);
    float (*get_AIR_SHUTDOWN_current)(void);
};

struct LoadSwitch *App_LoadSwitch_Create(
    float (*get_AUX1_current)(void),
    float (*get_AUX2_current)(void),
    float (*get_LEFT_INVERTER_current)(void),
    float (*get_RIGHT_INVERTER_current)(void),
    float (*get_ENERGY_METER_current)(void),
    float (*get_CAN_current)(void),
    float (*get_AIR_SHUTDOWN_current)(void))
{
    struct LoadSwitch *load_switch = malloc(sizeof(struct LoadSwitch));
    assert(load_switch != NULL);

    load_switch->get_AUX1_current               = get_AUX1_current;
    load_switch->get_AUX2_current               = get_AUX2_current;
    load_switch->get_LEFT_INVERTER_current      = get_LEFT_INVERTER_current;
    load_switch->get_RIGHT_INVERTER_current     = get_RIGHT_INVERTER_current;
    load_switch->get_ENERGY_METER_current       = get_ENERGY_METER_current;
    load_switch->get_CAN_current                = get_CAN_current;
    load_switch->get_AIR_SHUTDOWN_current       = get_AIR_SHUTDOWN_current;

    return load_switch;
}

void App_LoadSwitch_Destroy(struct LoadSwitch *load_switch)
{
    free(load_switch);
}

float App_LoadSwitch_Get_AUX1_Current(struct LoadSwitch *load_switch)
{
    return load_switch->get_AUX1_current();
}

float App_LoadSwitch_Get_AUX2_Current(struct LoadSwitch *load_switch)
{
    return load_switch->get_AUX2_current();
}

float App_LoadSwitch_Get_LEFT_INVERTER_Current(struct LoadSwitch *load_switch)
{
    return load_switch->get_LEFT_INVERTER_current();
}

float App_LoadSwitch_Get_RIGHT_INVERTER_Current(struct LoadSwitch *load_switch)
{
    return load_switch->get_RIGHT_INVERTER_current();
}

float App_LoadSwitch_Get_ENERGY_METER_Current(struct LoadSwitch *load_switch)
{
    return load_switch->get_ENERGY_METER_current();
}

float App_LoadSwitch_Get_CAN_Current(struct LoadSwitch *load_switch)
{
    return load_switch->get_CAN_current();
}

float App_LoadSwitch_Get_AIR_SHUTDOWN_Current(struct LoadSwitch *load_switch)
{
    return load_switch->get_AIR_SHUTDOWN_current();
}

