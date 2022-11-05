
#include <stdlib.h>
#include <assert.h>
#include "../../Inc/App/App_LoadSwitch.h"

struct LoadSwitch {
    float (*get_CS_AIR_current)(void);
    float (*get_CS_LVPWR_current)(void);
    float (*get_CS_AUX1_current)(void);
    float (*get_CS_AUX2_current)(void);
    float (*get_CS_FRONTLHS_current)(void);
    float (*get_CS_FRONTRHS_current)(void);
    float (*get_CS_REARLHS_current)(void);
    float (*get_CS_REARRHS_current)(void);
};

struct LoadSwitch *App_LoadSwitch_Create(
    float (*get_CS_AIR_current)(void),
    float (*get_CS_LVPWR_current)(void),
    float (*get_CS_AUX1_current)(void),
    float (*get_CS_AUX2_current)(void),
    float (*get_CS_FRONTLHS_current)(void),
    float (*get_CS_FRONTRHS_current)(void),
    float (*get_CS_REARLHS_current)(void),
    float (*get_CS_REARRHS_current)(void))

{
    struct LoadSwitch *load_switch = malloc(sizeof(struct LoadSwitch));
    assert(load_switch != NULL);

    load_switch->get_CS_AIR_current     = get_CS_AIR_current;
    load_switch->get_CS_LVPWR_current   = get_CS_LVPWR_current;
    load_switch->get_CS_AUX1_current    = get_CS_AUX1_current;
    load_switch->get_CS_AUX2_current    = get_CS_AUX2_current;
    load_switch->get_CS_FRONTLHS_current= get_CS_FRONTLHS_current;
    load_switch->get_CS_FRONTRHS_current= get_CS_FRONTRHS_current;
    load_switch->get_CS_REARLHS_current = get_CS_REARLHS_current;
    load_switch->get_CS_REARRHS_current = get_CS_REARRHS_current;


    return load_switch;
}

void App_LoadSwitch_Destroy(struct LoadSwitch *load_switch)
{
    free(load_switch);
}

float App_LoadSwitch_Get_CS_AIR_Current(struct LoadSwitch *load_switch)
{
    return load_switch->get_CS_AIR_current();
}

float App_LoadSwitch_Get_CS_LVPWR_Current(struct LoadSwitch *load_switch)
{
    return load_switch->get_CS_LVPWR_current();
}

float App_LoadSwitch_Get_CS_AUX1_Current(struct LoadSwitch *load_switch)
{
    return load_switch->get_CS_AUX1_current();
}

float App_LoadSwitch_Get_CS_AUX2_Current(struct LoadSwitch *load_switch)
{
    return load_switch->get_CS_AUX2_current();
}

float App_LoadSwitch_Get_CS_FRONTLHS_Current(struct LoadSwitch *load_switch)
{
    return load_switch->get_CS_FRONTLHS_current();
}

float App_LoadSwitch_Get_CS_FRONTRHS_Current(struct LoadSwitch *load_switch)
{
    return load_switch->get_CS_FRONTRHS_current();
}

float App_LoadSwitch_Get_CS_REARLHS_Current(struct LoadSwitch *load_switch)
{
    return load_switch->get_CS_REARLHS_current();
}

float App_LoadSwitch_Get_CS_REARRHS_Current(struct LoadSwitch *load_switch)
{
    return load_switch->get_CS_REARRHS_current();
}




