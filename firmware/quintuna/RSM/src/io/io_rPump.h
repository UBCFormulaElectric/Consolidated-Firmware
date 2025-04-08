#pragma once
#include <stdbool.h>

#ifdef TARGET_EMBEDDED
#include "hw_i2cs.h"
#include "hw_i2c.h"

/**
 * Checks rPump is communitcating over I2C
 * @return True if the I2C is ready
 */
bool io_rPump_isPumpReady();

/**
 *
 * @param data
 * @return
 */
void io_rPump_write(uint8_t data);

/**
 *
 * @return
 */
uint8_t io_rPump_read();

#endif

/**
 *
 * @param percentage
 */
void io_rPump_setPercentage(float percentage);

/**
 *
 * @return
 */
float io_rPump_readPercentage();