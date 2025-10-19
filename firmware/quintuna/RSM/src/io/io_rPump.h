#pragma once
#include "utils.h"
#include <stdbool.h>

#ifdef TARGET_EMBEDDED
#include "hw_i2cs.h"

/**
 * Checks rPump is communitcating over I2C
 * @return True if the I2C is ready
 */
ExitCode io_rPump_isPumpReady(void);

/**
 * @param data data to write to the pump controller
 * @return success of operation
 */
ExitCode io_rPump_write(uint8_t data);

/**
 * @param dest destination to write the data to
 * @return success of operation
 */
ExitCode io_rPump_read(uint8_t *dest);
#endif

/**
 *
 * @param percentage
 */
ExitCode io_rPump_setPercentage(float percentage);

/**
 *
 * @return
 */
ExitCode io_rPump_readPercentage(float *dest);