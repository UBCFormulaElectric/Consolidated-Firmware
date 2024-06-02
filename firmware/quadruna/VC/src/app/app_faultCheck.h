#pragma once

#include <stdbool.h>

/**
 * Check if any baord has faulted
 */
bool app_boardFaultCheck();

/**
 * Check either right or left inverters have faulted
 */
bool app_inverterFaultCheck();

/**
 * Initialize apps internal signals.
 */
void app_bspd_init(void);

/**
 * Check if brakes and apps are active at same time.
 */
bool app_bspdWarningCheck(float papps_pedal_percentage, float sapps_pedal_percentage);