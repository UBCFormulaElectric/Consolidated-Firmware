#pragma once

#include <stdint.h>
#include "hw_hal.hpp"

// For reference, see STM32's manual on using their cyclic redundancy check (CRC)
// driver:
// https://www.st.com/resource/en/application_note/an4187-using-the-crc-peripheral-on-stm32-microcontrollers-stmicroelectronics.pdf

namespace hw::crc
{
/**
 * Initialize the CRC driver.
 * @param crc_handle Handle to CRC peripheral.
 */
void init(CRC_HandleTypeDef *crc_handle);

/**
 * Calculate a checksum using the CRC peripheral.
 * @param buffer Pointer to data buffer to take the checksum of.
 * @param size_words Number of words (32 bits) of data to take the checksum of.
 * @return Checksum value.
 */
[[nodiscard]] uint32_t calculate(const uint32_t *buffer, const uint32_t size_words);
} // namespace hw::crc
