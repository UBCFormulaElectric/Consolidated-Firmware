// GENERATED WITH: pycrc --model crc-32 --algorithm table-driven --generate h

/**
 * \file
 * Functions and types for CRC checks.
 *
 * Generated on Thu May 22 18:19:59 2025
 * by pycrc v0.10.0, https://pycrc.org
 * using the configuration:
 *  - Width         = 32
 *  - Poly          = 0x04c11db7
 *  - XorIn         = 0xffffffff
 *  - ReflectIn     = True
 *  - XorOut        = 0xffffffff
 *  - ReflectOut    = True
 *  - Algorithm     = table-driven
 *
 * This file defines the functions crc_init(), crc_update() and crc_finalize().
 *
 * The crc_init() function returns the initial \c crc value and must be called
 * before the first call to crc_update().
 * Similarly, the crc_finalize() function must be called after the last call
 * to crc_update(), before the \c crc is being used.
 * is being used.
 *
 * The crc_update() function can be called any number of times (including zero
 * times) in between the crc_init() and crc_finalize() calls.
 *
 * This pseudo-code shows an example usage of the API:
 * \code{.c}
 * crc_t crc;
 * unsigned char data[MAX_DATA_LEN];
 * size_t data_len;
 *
 * crc = crc_init();
 * while ((data_len = read_data(data, MAX_DATA_LEN)) > 0) {
 *     crc = crc_update(crc, data, data_len);
 * }
 * crc = crc_finalize(crc);
 * \endcode
 */
#pragma once

#include <cstdint>

namespace app::crc32
{
/**
 * Reflect all bits of a \a data word of \a data_len bytes.
 *
 * \param[in] data     The data word to be reflected.
 * \param[in] data_len The width of \a data expressed in number of bits.
 * \return             The reflected data.
 */
uint32_t reflect(uint32_t data, size_t data_len);

/**
 * Calculate the initial crc value.
 *
 * \return     The initial crc value.
 */
static inline uint32_t init(void)
{
    return 0xffffffff;
}

/**
 * Update the crc value with new data.
 *
 * \param[in] crc      The current crc value.
 * \param[in] data     Pointer to a buffer of \a data_len bytes.
 * \param[in] data_len Number of bytes in the \a data buffer.
 * \return             The updated crc value.
 */
uint32_t update(uint32_t crc, const void *data, size_t data_len);

/**
 * Calculate the final crc value.
 *
 * \param[in] crc  The current crc value.
 * \return     The final crc value.
 */
static inline uint32_t finalize(const uint32_t crc)
{
    return crc ^ 0xffffffff;
}
} // namespace app::crc32