#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "hw_hal.h"

// For reference, see STM32's manual on using their cyclic redundancy check (CRC)
// driver:
// https://www.st.com/resource/en/application_note/an4187-using-the-crc-peripheral-on-stm32-microcontrollers-stmicroelectronics.pdf

/**
 * Initialize the CRC driver.
 * @param crc_handle Handle to CRC peripheral.
 */
void hw_crc_init(CRC_HandleTypeDef *crc_handle);

/**
 * Calculate a checksum using the CRC peripheral (HW).
 * @param buffer Pointer to data buffer to take the checksum of.
 * @param size_bytes Number of bytes of data to take the checksum of.
 * @return Checksum value.
 */
uint32_t hw_crc_calculate(const uint8_t *buffer, size_t size_bytes);

/**
 * Calculate a checksum using the CRC peripheral (custom poly/init).
 * @param buffer Pointer to data buffer to take the checksum of.
 * @param size_bytes Number of bytes of data to take the checksum of.
 * @param polynomial CRC polynomial (normal, MSB-first form).
 * @param init_value Initial CRC value.
 * @return Checksum value.
 */
uint32_t hw_crc_calculate_custom(const uint8_t *buffer, size_t size_bytes, uint32_t polynomial, uint32_t init_value);
