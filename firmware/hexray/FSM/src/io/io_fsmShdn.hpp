#pragma once

namespace io::fsmShdn
{
/**
 * @return True or Talse when cockpit has triggered a shutdown
 */
bool COCKPIT_SHDN_OK_get(void);

/**
 * @return True or Talse when brakes over travel has triggered a shutdown
 */
bool BOTS_SHDN_OK_get(void);

/**
 * @return True or Talse when fl interrupt has triggered a shutdown
 */
bool FL_SHDN_OK_get(void);

/**
 * @return True or Talse when fr interrupt has triggered a shutdown
 */
bool FR_SHDN_OK_get(void);
} // namespace io::fsmShdn
