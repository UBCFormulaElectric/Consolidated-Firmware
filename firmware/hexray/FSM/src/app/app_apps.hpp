#pragma once

#define DISAGREEMENT_TIME_TO_FAULT (10U)
#define DISAGREEMENT_TIME_TO_CLEAR (10U)
#define OCSC_TIME_TO_FAULT (10U)
#define OCSC_TIME_TO_CLEAR (10U)

/**
 * Initialize apps internal signals.
 */
void app_apps_init(void);

/**
 * Broadcast all apps alerts/faults/messages.
 */
void app_apps_broadcast(void);
