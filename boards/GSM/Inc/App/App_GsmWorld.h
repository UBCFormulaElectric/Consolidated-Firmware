#pragma once

#include "App_CanAlerts.h"
#include "App_CanRx.h"
#include "App_CanTx.h"
#include "App_SharedClock.h"

struct GsmWorld;

/**
 * Allocate and initialize a world in the application layer by registering
 * interfaces to it
 * @note This function does __not__ take ownership of any of the interfaces
 *       passed into it, which means the every interface must be kept alive for
 *       the lifetime of the created world
 * @return A pointer to the created world, whose ownership is given to the
 * caller
 */
struct GsmWorld *App_GsmWorld_Create(struct Clock *clock);

/**
 * Deallocate the memory used by the given world
 * @param world The world to deallocate
 */
void App_GsmWorld_Destroy(struct GsmWorld *world);
