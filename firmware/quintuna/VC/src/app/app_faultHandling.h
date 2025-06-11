#pragma once
// function to get all board faults
#include <stdbool.h>

#define APPS_BRAKE_DISAGREEMENT_TIME_TO_FAULT (10U)
#define APPS_BRAKE_DISAGREEMENT_TIME_TO_CLEAR (10U)

bool app_faultHandling_air_minus_closed(void);
