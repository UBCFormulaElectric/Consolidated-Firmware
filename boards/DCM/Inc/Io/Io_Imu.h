#pragma once

/**
 * Driver for interfacing with the Imu LSM6DSMTR via I2c
 *
 * The Application Note for this Imu can be found here:
 * https://www.st.com/content/ccc/resource/technical/document/datasheet/76/27/cf/88/c5/03/42/6b/DM00218116.pdf/files/DM00218116.pdf/jcr:content/translations/en.DM00218116.pdf
 */

#include <stdint.h>
#include <stm32f4xx_hal_i2c.h>
#include <stdbool.h>
#include "App_SharedExitCode.h"
#define RXBUFFERSIZE 12
static const uint8_t LSM6DSLTR_ADDR = 0x6B << 1; // slave address and SDO high, bit shift to make into 8 bit
#define LSM6DSLTR_AccelX_LSB 0x28
#define LSM6DSLTR_AccelX_MSB  0x29
#define LSM6DSLTR_AccelY_LSB 0x2A
#define LSM6DSLTR_AccelY_MSB 0x2B
#define LSM6DSLTR_AccelZ_LSB 0x2C
#define LSM6DSLTR_AccelZ_MSB 0x2D
// Read 1 write 0
/**
 * Get x acceleration from Imu
 * @return The acceleration (m/s^2) measured on the x-axis.
 */
float Io_Imu_GetAccelerationX(void);

/**
 * Get y acceleration from Imu
 * @return The acceleration (m/s^2) measured on the y-axis.
 */
float Io_Imu_GetAccelerationY(void);

/**
 * Get z acceleration from Imu
 * @return The acceleration (m/s^2) measured on the z-axis.
 */
float Io_Imu_GetAccelerationZ(void);
