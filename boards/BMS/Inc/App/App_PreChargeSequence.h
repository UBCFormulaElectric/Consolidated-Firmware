#pragma once

struct PrechargeRelay;

/**
 * Allocate and initialize a pre-charge sequence
 * @param close_relay A function that can be called to close the precharge relay
 * @param open_relay A function that can be called to open the precharge relay
 * @return The created precharge relay, whose ownership is given to the
 * caller
 */
struct PrechargeRelay *App_PrechargeRelay_Create(void (*close_relay)(void), void (*open_relay)(void));

/**
 * Deallocate the memory used by the pre-charge sequence
 * @param precharge_relay The precharge relay to deallocate
 */
void App_PrechargeRelay_Destroy(struct PrechargeRelay *precharge_relay);

/**
 * Close the precharge relay
 * @param precharge_relay The precharge relay to close
 */
void App_PrechargeRelay_Close(const struct PrechargeRelay *const precharge_relay);

/**
 * Open the given precharge relay
 * @param precharge_relay The precharge relay to open
 */
void App_PrechargeRelay_Open(const struct PrechargeRelay *const precharge_relay);
