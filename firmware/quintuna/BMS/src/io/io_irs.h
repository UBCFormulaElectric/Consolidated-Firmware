#pragma once

typedef enum
{
    CONTACTORS_OPEN,
    CONTACTORS_CLOSED
} ContactorsState;

/**
 * Check if the IR- is closed
 * @returns the state of the IR- relay.
 */
ContactorsState io_irs_negativeState(void);

/**
 * Sets the IR- relay to the desired state
 */
void io_irs_setPositive(ContactorsState state);

/**
 * Check if the IR+ is closed
 * @returns the state of the IR+ relay.
 */
ContactorsState io_irs_positiveState(void);

/**
 * Set the precharge relay to the desired state
 */
void io_irs_setPrecharge(ContactorsState state);

/**
 * Check if the precharge relay is closed
 * @returns the state of the precharge relay.
 */
ContactorsState io_irs_prechargeState(void);
