#pragma once

#include <stdbool.h>

#define APPS_BRAKE_DISAGREEMENT_TIME_TO_FAULT (10U)
#define APPS_BRAKE_DISAGREEMENT_TIME_TO_CLEAR (10U)

/**
 * Initialize apps internal signals.
 */
void app_faultCheck_init(void);

/**
 * Check if any baord has faulted
 */
bool app_faultCheck_checkBoards();

/**
 * Check either right or left inverters have faulted
 */
bool app_faultCheck_checkInverters();

/**
 * Check if brakes and apps are active at same time.
 */
bool app_faultCheck_checkSoftwareBspd(float papps_pedal_percentage, float sapps_pedal_percentage);