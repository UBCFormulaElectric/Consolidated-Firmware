#include "app_canUtils.hpp"

namespace io::irs
{
/**
 * Check if the IR- is closed
 * @returns the state of the IR- relay.
 */
app::can_utils::ContactorState negativeState(void);

/**
 * Sets the IR- relay to the desired state
 */
void setPositive(app::can_utils::ContactorState state);

/**
 * Check if the IR+ is closed
 * @returns the state of the IR+ relay.
 */
app::can_utils::ContactorState positiveState(void);

/**
 * Set the precharge relay to the desired state
 */
void setPrecharge(app::can_utils::ContactorState state);

/**
 * Check if the precharge relay is closed
 * @returns the state of the precharge relay.
 */
app::can_utils::ContactorState prechargeState(void);
} // namespace io::irs