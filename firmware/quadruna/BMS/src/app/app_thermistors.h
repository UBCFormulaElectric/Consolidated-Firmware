#pragma once
#include "io_thermistors.h"

/**
 * Initialize the thermistors module.
 */
void app_thermistors_init();

/**
 * Read a thermistor value then update the MUX channel to give time to switch before next read
 */
void app_thermistors_updateAuxThermistorTemps(void);

/**
 * Broadcast thermistor temperatures over CAN
 */
void app_thermistors_broadcast(void);
