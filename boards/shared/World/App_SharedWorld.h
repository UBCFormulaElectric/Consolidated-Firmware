#pragma once

#include "auto_generated/App_CanTx.h"

struct World;
struct CanTxInterface;

struct World *App_SharedWorld_Create(struct CanTxInterface *can_tx_interface);

struct CanTxInterface *App_SharedWorld_GetCanTx(struct World *world);
