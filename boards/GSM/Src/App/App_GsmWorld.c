#include <stdlib.h>
#include <assert.h>

#include "App_GsmWorld.h"

struct GsmWorld
{
    struct GsmCanTxInterface *can_tx_interface;
    struct GsmCanRxInterface *can_rx_interface;
};

struct GsmWorld *
    App_GsmWorld_Create(struct GsmCanTxInterface *can_tx_interface, struct GsmCanRxInterface *can_rx_interface)
{
    struct GsmWorld *world = (struct GsmWorld *)malloc(sizeof(struct GsmWorld));
    assert(world != NULL);

    world->can_tx_interface = can_tx_interface;
    world->can_rx_interface = can_rx_interface;

    return world;
}

void App_GsmWorld_Destroy(struct GsmWorld *world)
{
    free(world);
}

struct GsmCanTxInterface *App_GsmWorld_GetCanTx(const struct GsmWorld *const world)
{
    return world->can_tx_interface;
}

struct GsmCanRxInterface *App_GsmWorld_GetCanRx(const struct GsmWorld *const world)
{
    return world->can_rx_interface;
}
