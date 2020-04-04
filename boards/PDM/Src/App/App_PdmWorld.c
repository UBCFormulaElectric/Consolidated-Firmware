#include <stddef.h>
#include <stdlib.h>
#include "App_SharedAssert.h"

#include "App_PdmWorld.h"

struct PdmWorld
{
    struct PDMCanTxInterface *can_tx_interface;
    struct PDMCanRxInterface *can_rx_interface;
};

struct PdmWorld *App_PdmWorld_Create(
    struct PDMCanTxInterface *can_tx_interface,
    struct PDMCanRxInterface *can_rx_interface)
{
    struct PdmWorld *world = (struct PdmWorld *)malloc(sizeof(struct PdmWorld));
    shared_assert(world != NULL);

    world->can_tx_interface = can_tx_interface;
    world->can_rx_interface = can_rx_interface;

    return world;
}

void App_PdmWorld_Destroy(struct PdmWorld *world)
{
    shared_assert(world != NULL);
    free(world);
}

struct PDMCanTxInterface *App_PdmWorld_GetCanTx(struct PdmWorld *world)
{
    shared_assert(world != NULL);
    return world->can_tx_interface;
}

struct PDMCanRxInterface *App_SharedWorld_GetCanRx(struct PdmWorld *world)
{
    shared_assert(world != NULL);
    return world->can_rx_interface;
}
