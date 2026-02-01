#pragma once

namespace io
{
/**
 * Checks to see if right shutdown pin is high.
 * @return True if high, false if low.
 */
bool r_shdn_pin_is_high();

/**
 * Checks to see if left shutdown pin is high.
 * @return True if high, false if low.
 */
bool l_shdn_pin_is_high();

} // namespace io
