#include <stddef.h>
#include <stdlib.h>
#include <assert.h>

#include "App_DcmWorld.h"

struct DcmWorld
{
    struct DcmCanTxInterface *can_tx_interface;
    struct DcmCanRxInterface *can_rx_interface;
};

struct DcmWorld *App_DcmWorld_Create(
    struct DcmCanTxInterface *const can_tx_interface,
    struct DcmCanRxInterface *const can_rx_interface)
{
    struct DcmWorld *world = (struct DcmWorld *)malloc(sizeof(struct DcmWorld));
    assert(world != NULL);

    world->can_tx_interface = can_tx_interface;
    world->can_rx_interface = can_rx_interface;

    return world;
}

void App_DcmWorld_Destroy(struct DcmWorld *world)
{
    assert(world != NULL);
    free(world);
}

struct DcmCanTxInterface *
    App_DcmWorld_GetCanTx(const struct DcmWorld *const world)
{
    assert(world != NULL);
    return world->can_tx_interface;
}

struct DcmCanRxInterface *
    App_DcmWorld_GetCanRx(const struct DcmWorld *const world)
{
    assert(world != NULL);
    return world->can_rx_interface;
}
