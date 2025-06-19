#pragma once
#include "stdbool.h"

typedef enum
{
    NO_WARNINGS,
    BOARD_WARNING_DETECTED,
    INVERTER_FAULT
} warningType;

#define APPS_BRAKE_DISAGREEMENT_TIME_TO_FAULT (10U)
#define APPS_BRAKE_DISAGREEMENT_TIME_TO_CLEAR (10U)

warningType app_warningHandling_globalWarningCheck(void);

bool app_warningHandling_inverterStatus(void);

bool app_warningHandling_checkSoftwareBspd(float papps_pedal_percentage, float sapps_pedal_percentage);

void app_softwareBspd_init(void);
