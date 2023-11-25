#pragma once

#include <stdbool.h>

// TODO relook at these values
#define AGREEMENT_TIME_TO_FAULT (100U)
#define AGREEMENT_TIME_TO_CLEAR (1000U)
#define PAPPS_OCSC_TIME_TO_FAULT (10U)
#define PAPPS_OCSC_TIME_TO_CLEAR (10U)
#define SAPPS_OCSC_TIME_TO_FAULT (10U)
#define SAPPS_OCSC_TIME_TO_CLEAR (10U)
#define APP_BRAKE_TIME_TO_FAULT (10U)
#define APP_BRAKE_TIME_TO_CLEAR (10U)

/**
 * Initialize a pair of accelerator pedals
 */
void app_apps_init(void);

/**
 * Deallocate the memory used by the given pair of accelerator pedals
 */
void app_apps_destroy(void);

/**
 * Primary Broadcast Function. Very important.
 */
void app_apps_broadcast(void);
