#include <stddef.h>
#include <stdlib.h>
#include <assert.h>

#include "App_FsmWorld.h"

struct FsmWorld
{
    struct FsmCanTxInterface *can_tx_interface;
    struct FsmCanRxInterface *can_rx_interface;
};

struct FsmWorld *App_FsmWorld_Create(
    struct FsmCanTxInterface *const can_tx_interface,
    struct FsmCanRxInterface *const can_rx_interface)
{
    struct FsmWorld *world = (struct FsmWorld *)malloc(sizeof(struct FsmWorld));
    assert(world != NULL);

    world->can_tx_interface = can_tx_interface;
    world->can_rx_interface = can_rx_interface;

    return world;
}

void App_FsmWorld_Destroy(struct FsmWorld *world)
{
    assert(world != NULL);
    free(world);
}

struct FsmCanTxInterface *
    App_FsmWorld_GetCanTx(const struct FsmWorld *const world)
{
    assert(world != NULL);
    return world->can_tx_interface;
}

struct FsmCanRxInterface *
    App_FsmWorld_GetCanRx(const struct FsmWorld *const world)
{
    assert(world != NULL);
    return world->can_rx_interface;
}
