#pragma once
#include <stdbool.h>

#ifdef TARGET_EMBEDDED
#include "hw_i2cs.h"

/**
 * Checks rPump is communitcating over I2C
 * @return True if the I2C is ready
 */
ExitCode io_fPump_isPumpReady(void);

/**
 * @param data data to write to the pump controller
 * @return success of operation
 */
ExitCode io_fPump_write(uint8_t data);

/**
 * @param dest destination to write the data to
 * @return success of operation
 */
ExitCode io_fPump_read(uint8_t *dest);

/**
 * Checks rPump is communitcating over I2C
 * @return True if the I2C is ready
 */
 ExitCode io_rrPump_isPumpReady(void);

 /**
  * @param data data to write to the pump controller
  * @return success of operation
  */
 ExitCode io_rrPump_write(uint8_t data);
 
 /**
  * @param dest destination to write the data to
  * @return success of operation
  */
 ExitCode io_rrPump_read(uint8_t *dest);
#endif

/**
 *
 * @param percentage
 */
void io_fPump_setPercentage(float percentage);

/**
 *
 * @return
 */
float io_fPump_readPercentage();

/**
 *
 * @param percentage
 */
 void io_rrPump_setPercentage(float percentage);

 /**
  *
  * @return
  */
 float io_rrPump_readPercentage();