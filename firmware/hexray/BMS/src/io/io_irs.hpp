extern "C"
{
#include "app_canUtils.h"
}

namespace io::irs
{
/**
 * Check if the IR- is closed
 * @returns the state of the IR- relay.
 */
ContactorState negativeState(void);

/**
 * Sets the IR- relay to the desired state
 */
void setPositive(ContactorState state);

/**
 * Check if the IR+ is closed
 * @returns the state of the IR+ relay.
 */
ContactorState positiveState(void);

/**
 * Set the precharge relay to the desired state
 */
void setPrecharge(ContactorState state);

/**
 * Check if the precharge relay is closed
 * @returns the state of the precharge relay.
 */
ContactorState prechargeState(void);
} // namespace io::irs