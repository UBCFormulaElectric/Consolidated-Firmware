#pragma once

#include "app_canUtils.hpp"

namespace io::irs
{
    
/**
 * Check if the IR- is closed
 * @returns the state of the IR- relay.
 */
app::can_utils::ContactorState negativeState(void);

}