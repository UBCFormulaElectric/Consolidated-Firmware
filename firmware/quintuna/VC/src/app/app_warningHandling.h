#pragma once
#include "stdbool.h"

typedef enum
{
    CAN_ISSUES = 3587,
    DC_BUS_ISSUES,
} Inverter_Fault_Info;

// board warnings
bool app_warningHandling_boardWarningCheck(void);

// inverters
bool app_warningHandling_inverterStatus(void);
void app_warningHandling_inverterReset(void);

// bspd
void app_softwareBspd_init(void);
bool app_warningHandling_checkSoftwareBspd(float papps_pedal_percentage);
