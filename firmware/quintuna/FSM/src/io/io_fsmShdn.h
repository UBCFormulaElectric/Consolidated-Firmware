#pragma once

#include <stdbool.h>

/**
 * @return True or Talse when cockpit has triggered a shutdown
 */
bool io_fsmShdn_COCKPIT_SHDN_OK_get(void);

/**
 * @return True or Talse when brakes over travel has triggered a shutdown
 */
bool io_fsmShdn_BOTS_SHDN_OK_get(void);

/**
 * @return True or Talse when fl interrupt has triggered a shutdown
 */
bool io_fsmShdn_FL_SHDN_OK_get(void);

/**
 * @return True or Talse when fr interrupt has triggered a shutdown
 */
bool io_fsmShdn_FR_SHDN_OK_get(void);
