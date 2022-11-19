#include <assert.h>
#include <malloc.h>
#include <stdbool.h>
#include "../../Inc/App/App_LoadSwitch.h"
#include "configs/App_CurrentLimits.h"

struct LoadSwitch
{
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

    load_switch->get_AUX1_current           = get_AUX1_current;
    load_switch->get_AUX2_current           = get_AUX2_current;
    load_switch->get_LEFT_INVERTER_current  = get_LEFT_INVERTER_current;
    load_switch->get_RIGHT_INVERTER_current = get_RIGHT_INVERTER_current;
    load_switch->get_ENERGY_METER_current   = get_ENERGY_METER_current;
    load_switch->get_CAN_current            = get_CAN_current;
    load_switch->get_AIR_SHUTDOWN_current   = get_AIR_SHUTDOWN_current;

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

int App_LoadSwitch_InRangeCheck(float value, float min_value, float max_value)
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

bool App_LoadSwitch_AreCurrentsInRange(struct LoadSwitch *load_switch)
{
    if (App_LoadSwitch_InRangeCheck(load_switch->get_AUX1_current(), AUX1_MIN_CURRENT, AUX1_MAX_CURRENT) == 0 &&
        App_LoadSwitch_InRangeCheck(load_switch->get_AUX2_current(), AUX2_MIN_CURRENT, AUX2_MAX_CURRENT) == 0 &&
        App_LoadSwitch_InRangeCheck(
            load_switch->get_LEFT_INVERTER_current(), LEFT_INVERTER_MIN_CURRENT, LEFT_INVERTER_MAX_CURRENT) == 0 &&
        App_LoadSwitch_InRangeCheck(
            load_switch->get_RIGHT_INVERTER_current(), RIGHT_INVERTER_MIN_CURRENT, RIGHT_INVERTER_MAX_CURRENT) == 0 &&
        App_LoadSwitch_InRangeCheck(
            load_switch->get_ENERGY_METER_current(), ENERGY_METER_MIN_CURRENT, ENERGY_METER_MAX_CURRENT) == 0 &&
        App_LoadSwitch_InRangeCheck(load_switch->get_CAN_current(), CAN_MIN_CURRENT, CAN_MAX_CURRENT) == 0 &&
        App_LoadSwitch_InRangeCheck(
            load_switch->get_AIR_SHUTDOWN_current(), AIR_SHUTDOWN_MIN_CURRENT, AIR_SHUTDOWN_MAX_CURRENT) == 0)
        return true;
    return false;
}
