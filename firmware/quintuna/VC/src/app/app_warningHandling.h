#pragma once
#include "app_faultHandling.h"
#include "stdbool.h"

typedef enum
{
    NO_WARNINGS,
    BOARD_WARNING_DETECTED,
    INVERTER_FAULT
} WarningType;

typedef enum
{
    CAN_ISSUES = 3587,
    DC_BUS_ISSUES,
} Inverter_Fault_Info;

WarningType app_warningHandling_globalWarningCheck(void);

bool app_warningHandling_inverterStatus();

void app_warningHandling_inverterReset(void);

void app_softwareBspd_init(void);