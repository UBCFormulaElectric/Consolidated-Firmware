#pragma once

#include "App_CanTx.h"
#include "App_CanRx.h"
#include "App_InRangeCheck.h"
#include "App_SharedHeartbeatMonitor.h"
#include "App_SharedRgbLedSequence.h"
#include "App_SharedSignal.h"
#include "App_SharedClock.h"
#include "App_AcceleratorPedals.h"
#include "App_Brake.h"
#include "App_Coolant.h"
#include "App_Steering.h"
#include "App_Wheels.h"

struct FsmWorld;
struct AcceleratorPedals;
struct Brake;
struct Coolant;
struct Steering;
struct Wheels;

/**
 * Allocate and initialize a world in the application layer by registering
 * interfaces to it
 * @note This function does __not__ take ownership of any of the interfaces
 *       passed into it, which means the every interface must be kept alive for
 *       the lifetime of the created world
 * @return A pointer to the created world, whose ownership is given to the
 * caller
 */
struct FsmWorld *App_FsmWorld_Create(
    struct HeartbeatMonitor * heartbeat_monitor,
    struct AcceleratorPedals *papps_and_sapps,
    struct Brake *            brake,
    struct Coolant *          coolant,
    struct Steering *         steering,
    struct Wheels *           wheels);

/**
 * Deallocate the memory used by the given world
 * @param world The world to deallocate
 */
void App_FsmWorld_Destroy(struct FsmWorld *world);

/**
 * Get the heartbeat monitor for the given world
 * @param world The world to get heartbeat monitor for
 * @return The heartbeat monitor for the given world
 */
struct HeartbeatMonitor *App_FsmWorld_GetHeartbeatMonitor(const struct FsmWorld *world);

/**
 * Get the secondary flow rate in-range check for the given world
 * @param world The world to get secondary flow rate in-range check for
 * @return The secondary flow rate in-range check for the given world
 */
struct InRangeCheck *App_FsmWorld_GetSecondaryFlowRateInRangeCheck(const struct FsmWorld *world);

/**
 * Get the brake for the given world
 * @param world The world to get the brake for
 * @return The brake for the given world
 */
struct Brake *App_FsmWorld_GetBrake(const struct FsmWorld *world);

/**
 * Get the coolant for the given world
 * @param world The world to get the coolant for
 * @return The coolant for the given world
 */
struct Coolant *App_FsmWorld_GetCoolant(const struct FsmWorld *const world);

/**
 * Get the steering for the given world
 * @param world The world to get the steering for
 * @return The steering for the given world
 */
struct Steering *App_FsmWorld_GetSteering(const struct FsmWorld *const world);

/**
 * Get the wheel speed for the given world
 * @param world The world to get the wheel speeds for
 * @return The wheel speeds for the given world
 */
struct Wheels *App_FsmWorld_GetWheels(const struct FsmWorld *const world);

/**
 * Get the pair of primary and secondary APPS for the given world
 * @param world The world to get the pair of primary and secondary APPS for
 * @return The primary and secondary APPS for the given world
 */
struct AcceleratorPedals *App_FsmWorld_GetPappsAndSapps(const struct FsmWorld *world);
