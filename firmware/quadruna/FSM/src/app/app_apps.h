#pragma once

#include <stdbool.h>

#define PAPPS_SAPPS_DISAGREEMENT_TIME_TO_FAULT (10U)
#define PAPPS_SAPPS_DISAGREEMENT_TIME_TO_CLEAR (10U)
#define APPS_OCSC_TIME_TO_FAULT (10U)
#define APPS_OCSC_TIME_TO_CLEAR (10U)

/**
 * Initialize apps internal signals.
 */
void app_apps_init(void);

/**
 * Broadcast all apps alerts/faults/messages.
 */
void app_apps_broadcast(void);
