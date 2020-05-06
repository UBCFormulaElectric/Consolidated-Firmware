#include <stdlib.h>
#include <assert.h>

#include "App_BrakeLight.h"

struct BrakeLight
{
    bool (*is_brake_actuated)(void);
    bool (*is_regen_active)(void);
    void (*turn_on_brake_light)(void);
    void (*turn_off_brake_light)(void);
    bool is_turned_on;
};

struct BrakeLight *App_BrakeLight_Create(
    bool (*const is_brake_actuated)(void),
    bool (*const is_regen_active)(void),
    void (*const turn_on_brake_light)(void),
    void (*const turn_off_brake_light)(void))
{
    assert(is_brake_actuated != NULL);
    assert(is_regen_active != NULL);
    assert(turn_on_brake_light != NULL);
    assert(turn_off_brake_light != NULL);

    struct BrakeLight *brake_light = malloc(sizeof(struct BrakeLight));

    assert(brake_light != NULL);

    brake_light->is_brake_actuated    = is_brake_actuated;
    brake_light->is_regen_active      = is_regen_active;
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

void App_BrakeLight_Tick(struct BrakeLight *const brake_light)
{
    if (brake_light->is_brake_actuated() == true ||
        brake_light->is_regen_active() == true)
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
