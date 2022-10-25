
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "../../Inc/App/App_LoadSwitch.h"

struct LoadSwitch {
    float (*get_CS1_current)(void);
    float (*get_CS2_current)(void);
};

struct LoadSwitch *App_LoadSwitch_Create(float (*get_CS1_current)(void), float (*get_CS2_current)(void)) {
    struct LoadSwitch *load_switch = malloc(sizeof(struct LoadSwitch));

    assert(load_switch != NULL);

    load_switch->get_CS1_current = get_CS1_current;
    load_switch->get_CS2_current = get_CS2_current;


    return load_switch;
}

void App_LoadSwitch_Destroy(struct LoadSwitch *load_switch)
{
    free(load_switch);
}

float App_LowVoltageBattery_GetCS1Voltage(struct LoadSwitch *load_switch)
{
    return load_switch->get_CS1_current();
}
float App_LowVoltageBattery_GetCS2Voltage(struct LoadSwitch *load_switch)
{
    return load_switch->get_CS2_current();
}



