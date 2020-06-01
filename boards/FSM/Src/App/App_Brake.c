#include <stdlib.h>
#include <assert.h>
#include "App_Brake.h"

struct Brake
{
    bool (*is_brake_actuated)(void);
};

struct Brake *App_Brake_Create(bool (*is_brake_actuated)(void))
{
    struct Brake *brake = malloc(sizeof(struct Brake));
    assert(brake != NULL);

    brake->is_brake_actuated = is_brake_actuated;

    return brake;
}

void App_Brake_Destroy(struct Brake *brake)
{
    free(brake);
}

bool App_Brake_IsBrakeActuated(const struct Brake *brake)
{
    return brake->is_brake_actuated();
}
