// GENERATED WITH: pycrc --model crc-8 --algorithm table-driven --generate h
// C++ port of firmware/shared/src/app/app_crc8.h.
//
// Configuration:
//   Width        = 8
//   Poly         = 0x07
//   XorIn        = 0x00
//   ReflectIn    = False
//   XorOut       = 0x00
//   ReflectOut   = False
//   Algorithm    = table-driven
//
// Output is byte-identical to app_crc8 (C) and to the Python decoder in
// firmware/logfs/python/logfs/can_logger.py.
#pragma once

#include <cstdint>
#include <cstddef>

namespace app::crc8
{
/**
 * Calculate the initial crc value.
 */
static inline uint8_t init()
{
    return 0x00;
}

/**
 * Update the crc value with new data.
 */
uint8_t update(uint8_t crc, const void *data, size_t data_len);

/**
 * Calculate the final crc value.
 */
static inline uint8_t finalize(const uint8_t crc)
{
    return crc;
}
} // namespace app::crc8
