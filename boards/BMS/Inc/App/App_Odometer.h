#pragma once

#include "App_SharedExitCode.h"
#include "App_Eeprom.h"

#define ODOMETER_ADDRESS 0U
#define TIRE_CIRCUMFRANCE_KM 0.001276743f

struct Odometer;

/**
 * Allocate and initialize an Odometer
 * @return Odometer object pointer
 */
struct Odometer *App_Odometer_Create(void);

/**
 * Destroy Odometer object
 * @param odometer to be destroyed
 */
void App_Odometer_Destroy(struct Odometer *odometer);

/**
 * Read stored odometer reading into EEPROM, write to odometer object
 * @param eeprom to read from
 * @param address Address where odometer can be found
 * @return odometer reading
 */
float App_Odometer_ReadValFromEeprom(struct Eeprom *eeprom, uint16_t address);

/**
 * Write current odometer reading to EEPROM
 * @param odometer to get reading from
 * @param eeprom to write to
 * @param address where to write odometer reading on EEPROM
 * @return ExitCode status of write
 */
ExitCode App_Odometer_WriteValToEeprom(struct Odometer *odometer, struct Eeprom *eeprom, uint16_t address);

/**
 * Update odometer reading in km
 * @param odometer to calculate reading for
 * @return updated odometer reading in km
 */
float App_Odometer_UpdateReading(struct Odometer *odometer);

/**
 * Get current odometer reading in km
 * @param odometer to calculate reading for
 * @return odometer reading in km
 */
float App_Odometer_GetReading(struct Odometer *odometer);

/**
 * Set odometer reading back to given value
 * @param odometer to set reading of
 * @param odometer_value new value to write to odometer
 */
void App_Odometer_SetReading(struct Odometer *odometer, float odometer_value);

/**
 * Tick Odometer write counter, this allows the odometer to be written to the EEPROM less frequently, removing the need
 * for wear levlling
 * @param odometer to tick counter for
 * @return will return true once every SECONDS_BETWEEN_ODOMETER_WRITES ticks, false otherwise
 */
bool App_Odometer_TickCounter(struct Odometer *odometer);

/**
 * Reset Odometer write counter to zero
 * @param odometer to reset counter for
 */
void App_Odometer_ResetCounter(struct Odometer *odometer);
