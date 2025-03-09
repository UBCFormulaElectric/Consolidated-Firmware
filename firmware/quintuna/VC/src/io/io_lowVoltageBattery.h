#pragma once

#include <stdbool.h>
#ifdef TARGET_EMBEDDED
typedef enum
{
    CURRENT_REGULATING,
    VOLTAGE_REGULATING
} ChargingStatus;
#else
#include "app_utils.h"
EMPTY_STRUCT(ChargingStatus)
#endif

/**
 * Check if boost controller has fault.
 * @return return true if the boost controller has fault
 */
bool io_lowVoltageBattery_hasBoostControllerFault(void);

/**
 * Check which charging status the charging chip is set as
 * @return Returns the charing status.
 */
ChargingStatus io_lowVoltageBattery_chargingStatus(void);

/**
 * Enable or disable the charging chip
 * @param state enable/disable chip
 */
void io_lowVoltageBattery_chargingShutdown(bool state);
