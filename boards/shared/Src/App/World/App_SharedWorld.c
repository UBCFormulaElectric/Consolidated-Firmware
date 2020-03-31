#include <stddef.h>

#include "App_SharedAssert.h"
#include "World/App_SharedWorld.h"

// The current firmware architecture only requires creating a single
// world for each board.
#define MAX_NUM_OF_WORLDS 1

struct World
{
    struct CanTxInterface *can_tx_interface;
    struct CanRxInterface *can_rx_interface;
};

struct World *App_SharedWorld_Create(
    struct CanTxInterface *can_tx_interface,
    struct CanRxInterface *can_rx_interface)
{
    static struct World worlds[MAX_NUM_OF_WORLDS];
    static size_t       alloc_index = 0;

    shared_assert(alloc_index < MAX_NUM_OF_WORLDS);

    struct World *world     = &worlds[alloc_index++];
    world->can_tx_interface = can_tx_interface;
    world->can_rx_interface = can_rx_interface;

    return world;
}

struct CanTxInterface *App_SharedWorld_GetCanTx(struct World *world)
{
    return world->can_tx_interface;
}

struct CanRxInterface *App_SharedWorld_GetCanRx(struct World *world)
{
    return world->can_rx_interface;
}
