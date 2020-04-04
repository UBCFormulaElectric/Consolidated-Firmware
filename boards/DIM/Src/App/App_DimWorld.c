#include <stddef.h>
#include <stdlib.h>
#include "App_SharedAssert.h"

#include "App_DimWorld.h"

struct DimWorld
{
};

struct DimWorld *App_DimWorld_Create()
{
    struct DimWorld *world = (struct DimWorld *)malloc(sizeof(struct DimWorld));
    shared_assert(world != NULL);

    return world;
}

void App_DimWorld_Destroy(struct DimWorld *world)
{
    shared_assert(world != NULL);
    free(world);
}
