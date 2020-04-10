#pragma once

#include "App_CanTx.h"
#include "App_CanRx.h"

struct DimWorld;

/**
 * Allocate and initialize a world in the application layer
 * @return A pointer to the created world, whose ownership is given to the
 * caller
 */
struct DimWorld *App_DimWorld_Create();

/**
 * Destroy the given DIM world, freeing the memory associated with it
 * @param world The DIM world to destroy
 */
void App_DimWorld_Destroy(struct DimWorld *world);
