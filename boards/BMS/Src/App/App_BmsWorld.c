#include <stddef.h>
#include <stdlib.h>
#include "App_SharedAssert.h"

#include "App_BmsWorld.h"

struct BmsWorld
{
    struct BMSCanTxInterface *can_tx_interface;
    struct BMSCanRxInterface *can_rx_interface;
    struct Imd *              imd;
};

struct BmsWorld *App_BmsWorld_Create(
    struct BMSCanTxInterface *can_tx_interface,
    struct BMSCanRxInterface *can_rx_interface,
    struct Imd *              imd)
{
    struct BmsWorld *world = (struct BmsWorld *)malloc(sizeof(struct BmsWorld));
    shared_assert(world != NULL);

    world->can_tx_interface = can_tx_interface;
    world->can_rx_interface = can_rx_interface;
    world->imd              = imd;

    return world;
}

void App_BmsWorld_Destroy(struct BmsWorld *world)
{
    shared_assert(world != NULL);
    free(world);
}

struct BMSCanTxInterface *App_BmsWorld_GetCanTx(struct BmsWorld *world)
{
    shared_assert(world != NULL);
    return world->can_tx_interface;
}

struct BMSCanRxInterface *App_BmsWorld_GetCanRx(struct BmsWorld *world)
{
    shared_assert(world != NULL);
    return world->can_rx_interface;
}

struct Imd *App_BmsWorld_GetImd(struct BmsWorld *world)
{
    shared_assert(world != NULL);
    return world->imd;
}
