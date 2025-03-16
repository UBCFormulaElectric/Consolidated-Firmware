#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "hw_spi.h"

/**
 * @brief Sends multiple bytes to a daisy-chained shift register setup.
 *
 * If you have multiple shift registers daisy-chained, you can send
 * as many bytes as needed. The first byte clocked in will end up in the
 * last device in the chain (MSB-first), and so on.
 *
 * @param spi Pointer to the SPI device config.
 * @param data Pointer to data buffer.
 * @param length Number of bytes in buffer.
 * @return true if transmission was successful, false otherwise.
 */
bool io_shift_register_writeBytes(const SpiDevice *spi, const uint8_t *data, uint16_t length);
