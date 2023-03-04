#pragma once

/**
 * Driver for interfacing with the Imu LSM6DSMTR via I2c
 *
 * The Application Note for this Imu can be found here:
 * https://www.st.com/content/ccc/resource/technical/document/datasheet/76/27/cf/88/c5/03/42/6b/DM00218116.pdf/files/DM00218116.pdf/jcr:content/translations/en.DM00218116.pdf
 */

#include <stdint.h>
#include <stm32f4xx_hal.h>
#include <stdbool.h>
#include "App_SharedExitCode.h"

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
