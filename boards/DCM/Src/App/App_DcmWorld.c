#include <stddef.h>
#include <stdlib.h>
#include "App_SharedAssert.h"

#include "App_DcmWorld.h"

struct DcmWorld
{
    struct DCMCanTxInterface *can_tx_interface;
    struct DCMCanRxInterface *can_rx_interface;
};

struct DcmWorld *App_DcmWorld_Create(
    struct DCMCanTxInterface *const can_tx_interface,
    struct DCMCanRxInterface *const can_rx_interface)
{
    struct DcmWorld *world = (struct DcmWorld *)malloc(sizeof(struct DcmWorld));
    shared_assert(world != NULL);

    world->can_tx_interface = can_tx_interface;
    world->can_rx_interface = can_rx_interface;

    return world;
}

void App_DcmWorld_Destroy(struct DcmWorld *world)
{
    shared_assert(world != NULL);
    free(world);
}

struct DCMCanTxInterface *App_DcmWorld_GetCanTx(const struct DcmWorld * const world)
{
    shared_assert(world != NULL);
    return world->can_tx_interface;
}

struct DCMCanRxInterface *App_DcmWorld_GetCanRx(const struct DcmWorld * const world)
{
    shared_assert(world != NULL);
    return world->can_rx_interface;
}
