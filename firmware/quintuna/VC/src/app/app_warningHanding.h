#pragma once 
#include "stdbool.h"

typedef enum
{
    NO_WARNINGS,
    MISSING_HEARTBEAT, 
    INVERTER_FAULT
} warningType;

warningType app_warningHandling_globalWarningCheck(void);

bool app_warningHandling_boardWarningCheck(void);

bool app_warningHandling_inverterStatus(void);
