#pragma once

#include "app_canUtils.h"

typedef enum
{
    IRS_OPEN   = CONTACTOR_STATE_CLOSED,
    IRS_CLOSED = CONTACTOR_STATE_OPEN,
} IRsState;

/**
 * Check if the IR- is closed
 * @returns the state of the IR- relay.
 */
IRsState io_irs_negativeState(void);

/**
 * Sets the IR- relay to the desired state
 */
void io_irs_setPositive(IRsState state);

/**
 * Check if the IR+ is closed
 * @returns the state of the IR+ relay.
 */
IRsState io_irs_positiveState(void);

/**
 * Set the precharge relay to the desired state
 */
void io_irs_setPrecharge(IRsState state);

/**
 * Check if the precharge relay is closed
 * @returns the state of the precharge relay.
 */
IRsState io_irs_prechargeState(void);
