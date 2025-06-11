#pragma once
#include "stdbool.h"

typedef enum
{
    NO_WARNINGS,
    BOARD_WARNING_DETECTED,
    INVERTER_FAULT
} warningType;

warningType app_warningHandling_globalWarningCheck(void);

bool app_warningHandling_inverterStatus();
