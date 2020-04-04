#include <stddef.h>
#include <stdlib.h>

#include "App_SharedAssert.h"


struct DcmWorld
{
    struct DCMCanTxInterface *can_tx_interface;
    struct DCMCanRxInterface* can_rx_interface;
};

struct DcmWorld *App_DcmWorld_Create(
    struct DCMCanTxInterface *can_tx_interface,
    struct DCMCanRxInterface *can_rx_interface)
{
    struct DcmWorld *world  = (struct DcmWorld*)malloc(sizeof(struct DcmWorld));

    world->can_tx_interface = can_tx_interface;
    world->can_rx_interface = can_rx_interface;

    return world;
}

void App_DcmWorld_Destroy(struct DcmWorld* dcm_world){
    free(dcm_world);
}


struct DCMCanTxInterface *App_DcmWorld_GetCanTx(struct DcmWorld *world)
{
    return world->can_tx_interface;
}

struct DcmCanRxInterface *App_SharedWorld_GetCanRx(struct World *world);
