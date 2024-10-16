#pragma once

#include <stdbool.h>
#include <stdint.h>

extern "C"
{
#include "hw_hal.h"
}

#ifdef TARGET_EMBEDDED
extern I2C_HandleTypeDef hi2c1;
#endif

class i2cPotentiometer
{
  public:
    /**
     * Initializes the i2c interface with the potentiometer.
     * @return True if i2c potentiometer is ready, else false.
     */
    bool isReady();

    /**
     * Sets potentiometer wiper.
     * @param position Wiper position, 0 (min) to 127 (full).
     */
    void setWiper(uint8_t position);

    /**
     * Reads the current state of the wiper.
     * @return Current state of the wiper, 0 to 127.
     */
    uint8_t readWiper();
};
