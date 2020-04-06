#include <stddef.h>
#include <stdlib.h>
#include "App_SharedAssert.h"

#include "App_FsmWorld.h"

struct FsmWorld
{
    struct FSMCanTxInterface *can_tx_interface;
    struct FSMCanRxInterface *can_rx_interface;
};

struct FsmWorld *App_FsmWorld_Create(
    struct FSMCanTxInterface *const can_tx_interface,
    struct FSMCanRxInterface *const can_rx_interface)
{
    struct FsmWorld *world = (struct FsmWorld *)malloc(sizeof(struct FsmWorld));
    shared_assert(world != NULL);

    world->can_tx_interface = can_tx_interface;
    world->can_rx_interface = can_rx_interface;

    return world;
}

void App_FsmWorld_Destroy(struct FsmWorld *world)
{
    shared_assert(world != NULL);
    free(world);
}

struct FSMCanTxInterface *App_FsmWorld_GetCanTx(const struct FsmWorld * const world)
{
    shared_assert(world != NULL);
    return world->can_tx_interface;
}

struct FSMCanRxInterface *App_FsmWorld_GetCanRx(const struct FsmWorld * const world)
{
    shared_assert(world != NULL);
    return world->can_rx_interface;
}
