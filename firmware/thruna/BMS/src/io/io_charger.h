#pragma once

#include <stdbool.h>
#include "App_SharedMacros.h"

#ifdef TARGET_EMBEDDED
#include "hw_gpio.h"

typedef struct
{
    const Gpio enable_gpio;
    const Gpio connected_gpio;
    const Gpio faulted_gpio;
} Charger;
#else
EMPTY_STRUCT(Charger);
#endif

/*
 * This library is modelled after the BRUSA NLG513 charger. It has the concept
 * of a master switch (Pin 3: PON). In short, if the BRUSA NLG513 is powered on
 * but the master switch is disabled, it can never start the charging sequence.
 * For the charging sequence to start, the BRUSA NLG513 must be powered on and
 * its master switch enabled.
 */

/**
 * Initialize the charger.
 * @param chgr Charger config struct.
 */
void io_charger_init(const Charger* chgr);

/**
 * Enable/disable the PON pin of the BRUSA NLG513 charger
 * @param enable Whether or not to enable the charger
 */
void io_charger_enable(bool enable);

/**
 * Check if the BRUSA NLG513 charger is connected
 * @return true if the BRUSA NLG513 charger is connected, else false
 */
bool io_charger_isConnected(void);

/**
 * Check if the BRUSA has a fault
 * Indicates a fault if the charger is enabled or idle if disabled
 * @return true if the BRUSA NLG513 charger has faulted/idle, else false
 */
bool io_charger_hasFaulted(void);
