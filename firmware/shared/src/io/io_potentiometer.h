#pragma once

#include <stdbool.h>
#include <stdint.h>

#ifdef TARGET_EMBEDDED

#include "hw_i2cs.h"

typedef struct
{
    const I2cDevice *i2c_handle;
} Potentiometer;

#else
#include "app_utils.h"
EMPTY_STRUCT(Potentiometer)
#endif

typedef enum
{
    WIPER0 = 0,
    WIPER1 = 1,
} POTENTIOMETER_WIPER;
/**
 * Reads the potentiometer wiper value as a percentage (0 to 100) of its maximum range.
 * @param potentiometer Pointer to the potentiometer object
 * @param wiper The wiper to read
 * @param dest Pointer to store the read percentage
 * @return If read was successful
 */
ExitCode io_potentiometer_readPercentage(const Potentiometer *potentiometer, POTENTIOMETER_WIPER wiper, uint8_t *dest);

/**
 * Writes the potentiometer wiper value as a percentage (0 to 100) of its maximum range.
 * @param potentiometer Pointer to the potentiometer object
 * @param wiper The wiper to write
 * @param percentage The percentage to write to the potentiometer
 * @return If write was successful
 */
ExitCode
    io_potentiometer_writePercentage(const Potentiometer *potentiometer, POTENTIOMETER_WIPER wiper, uint8_t percentage);

/**
 * Reads the potentiometer wiper value.
 * @param potentiometer Pointer to the potentiometer object
 * @param wiper The wiper to read
 * @param dest Pointer to store the read wiper value
 * @return If read was successful
 */
ExitCode io_potentiometer_readWiper(const Potentiometer *potentiometer, POTENTIOMETER_WIPER wiper, uint8_t dest[2]);

/**
 * Writes the potentiometer wiper value.
 * @param potentiometer Pointer to the potentiometer object
 * @param wiper The wiper to write
 * @param data The wiper value to write to the potentiometer
 * @return If write was successful
 */
ExitCode io_potentiometer_writeWiper(const Potentiometer *potentiometer, POTENTIOMETER_WIPER wiper, uint8_t data);