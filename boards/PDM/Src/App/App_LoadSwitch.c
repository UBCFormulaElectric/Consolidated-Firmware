
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "../../Inc/App/App_LoadSwitch.h"

struct LoadSwitch {
    float (*)(void);
    float (*)(void);
};

struct LoadSwitch *App_LoadSwitch_Create(bool (*has_current_fault)(void)) {
    struct LoadSwitch *load_switch = malloc(sizeof(struct LoadSwitch));
    assert(load_switch != NULL);

    load_switch->has_current_fault = has_current_fault;

    return load_switch;
}

void App_LoadSwitch_Destroy(struct LoadSwitch *load_switch)
{
    free(load_switch);
}

bool App_LowVoltageBattery_GetCurrent(const struct LoadSwitch *load_switch)
{
    return load_switch->has_fault();
}



