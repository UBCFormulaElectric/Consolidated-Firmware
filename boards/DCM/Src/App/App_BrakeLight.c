#include <stdlib.h>
#include <assert.h>

#include "App_BrakeLight.h"

struct BrakeLight
{
    void (*turn_on_brake_light)(void);
    void (*turn_off_brake_light)(void);
    bool is_turned_on;
};

struct BrakeLight *
    App_BrakeLight_Create(void (*const turn_on_brake_light)(void), void (*const turn_off_brake_light)(void))
{
    assert(turn_on_brake_light != NULL);
    assert(turn_off_brake_light != NULL);

    struct BrakeLight *brake_light = malloc(sizeof(struct BrakeLight));

    assert(brake_light != NULL);

    brake_light->turn_on_brake_light  = turn_on_brake_light;
    brake_light->turn_off_brake_light = turn_off_brake_light;

    brake_light->turn_off_brake_light();
    brake_light->is_turned_on = false;

    return brake_light;
}

void App_BrakeLight_Destroy(struct BrakeLight *const brake_light)
{
    free(brake_light);
}

void App_BrakeLight_SetLightStatus(struct BrakeLight *const brake_light, bool is_brake_actuated, bool is_regen_active)
{
    if (is_brake_actuated || is_regen_active)
    {
        brake_light->turn_on_brake_light();
        brake_light->is_turned_on = true;
    }
    else
    {
        brake_light->turn_off_brake_light();
        brake_light->is_turned_on = false;
    }
}

bool App_BrakeLight_IsTurnedOn(const struct BrakeLight *const brake_light)
{
    return brake_light->is_turned_on;
}
