#include <stddef.h>

#include "App_SharedAssert.h"

// The current firmware architecture only requires creating a single
// world for each board.
#define MAX_NUM_OF_WORLDS 1

struct DcmWorld
{
    struct CanTxInterface *can_tx_interface;
};

struct DcmWorld *App_DcmWorld_Create(struct CanTxInterface *can_tx_interface)
{
    static struct DcmWorld worlds[MAX_NUM_OF_WORLDS];
    static size_t       alloc_index = 0;

    shared_assert(alloc_index < MAX_NUM_OF_WORLDS);

    struct DcmWorld *world     = &worlds[alloc_index++];
    world->can_tx_interface = can_tx_interface;

    return world;
}

struct CanTxInterface *App_DcmWorld_GetCanTx(struct DcmWorld *world)
{
    return world->can_tx_interface;
}
