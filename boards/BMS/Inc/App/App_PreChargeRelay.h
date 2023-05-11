#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <assert.h>
#include <stdlib.h>
#include "App_CanTx.h"

// clang-format off
#define NUM_OF_INVERTERS        (2U)
#define PRECHARGE_RESISTOR_OHMS (500U)
#define INV_CAPACITANCE_F       (0.280e-3f)
#define PRECHARGE_CAPACITANCE_F (INV_CAPACITANCE_F * NUM_OF_INVERTERS)
#define PRECHARGE_RC_MS         (S_TO_MS(PRECHARGE_RESISTOR_OHMS * PRECHARGE_CAPACITANCE_F))

// 2.7RC corresponds to time to reach ~93% charged
#define PRECHARGE_COMPLETION_MS          ((float)PRECHARGE_RC_MS * 2.7f)
#define PRECHARGE_COMPLETION_LOWER_BOUND ((uint32_t)(PRECHARGE_COMPLETION_MS * 0.5f))
#define PRECHARGE_COMPLETION_UPPER_BOUND ((uint32_t)(PRECHARGE_COMPLETION_MS * 3.0f))

// clang-format on

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

/**
 * Increment Precharge Fault Count
 * @param precharge_relay The precharge relay to increment counter on
 */
void App_PrechargeRelay_IncFaultCounter(struct PrechargeRelay *precharge_relay);

/**
 * Return Precharge Fault Count
 * @param precharge_relay The precharge relay to return counter value of
 */
uint8_t App_PrechargeRelay_GetFaultCounterVal(const struct PrechargeRelay *const precharge_relay);

/**
 * Reset Precharge Fault Count
 * @param precharge_relay The precharge relay to reset counter value of
 */
void App_PrechargeRelay_ResetFaultCounterVal(struct PrechargeRelay *const precharge_relay);

/**
 * Check the status of Precharge faults
 * @param can_tx CAN interface to send messages over
 * @param is_charger_connected True if charger connected, false otherwise
 * @param is_ts_rising_quickly True if tractive system voltage rising quickly, false otherwise
 * @param is_ts_rising_slowly True if tractive system voltage rising slowly, false otherwise
 * @param is_air_negative_open True if negative contactor is open, false otherwise
 * @param precharge_limit_exceeded Acts as second return val, set to True if 3 precharge faults are detected
 * @return True if faults present, false otherwise
 */
bool App_PrechargeRelay_CheckFaults(
    struct PrechargeRelay *precharge_relay,
    bool                   is_charger_connected,
    bool                   is_ts_rising_slowly,
    bool                   is_ts_rising_quickly,
    bool                   is_air_negative_open,
    bool *                 precharge_limit_exceeded);
