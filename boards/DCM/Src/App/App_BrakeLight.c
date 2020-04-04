#include <stdlib.h>

#include "App_BrakeLight.h"
#include "App_SharedAssert.h"

struct BrakeLight
{
    bool (*is_brake_actuated)(void);
    bool (*is_regen_active)(void);
    void (*turn_on_brake_light)(void);
    void (*turn_off_brake_light)(void);
    enum BrakeLight_Status status;
};

struct BrakeLight *App_BrakeLight_Create(
    bool (*is_brake_actuated)(void),
    bool (*is_regen_active)(void),
    void (*turn_on_brake_light)(void),
    void (*turn_off_brake_light)(void))
{
    struct BrakeLight *brake_light = malloc(sizeof(struct BrakeLight));

    shared_assert(brake_light != NULL);

    brake_light->is_brake_actuated    = is_brake_actuated;
    brake_light->is_regen_active      = is_regen_active;
    brake_light->turn_on_brake_light  = turn_on_brake_light;
    brake_light->turn_off_brake_light = turn_off_brake_light;
    brake_light->status               = BRAKE_LIGHT_OFF;

    return brake_light;
}

void App_BrakeLight_Tick(struct BrakeLight *brake_light)
{
    if (brake_light->is_brake_actuated() == true ||
        brake_light->is_regen_active() == true)
    {
        brake_light->turn_on_brake_light();
        brake_light->status = BRAKE_LIGHT_ON;
    }
    else
    {
        brake_light->turn_off_brake_light();
        brake_light->status = BRAKE_LIGHT_OFF;
    }
}

enum BrakeLight_Status App_BrakeLight_GetStatus(struct BrakeLight *brake_light)
{
    return brake_light->status;
}

void App_BrakeLight_Destroy(struct BrakeLight *brake_light)
{
    free(brake_light);
}
