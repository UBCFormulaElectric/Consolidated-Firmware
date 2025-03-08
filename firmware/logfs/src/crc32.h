/**
 * \file
 * Functions and types for CRC checks.
 *
 * Generated on Sat Mar  1 16:35:17 2025
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
 * This file defines the functions crc32_init(), crc32_update() and crc32_finalize().
 *
 * The crc32_init() function returns the initial \c crc value and must be called
 * before the first call to crc32_update().
 * Similarly, the crc32_finalize() function must be called after the last call
 * to crc32_update(), before the \c crc is being used.
 * is being used.
 *
 * The crc32_update() function can be called any number of times (including zero
 * times) in between the crc32_init() and crc32_finalize() calls.
 *
 * This pseudo-code shows an example usage of the API:
 * \code{.c}
 * crc32_t crc;
 * unsigned char data[MAX_DATA_LEN];
 * size_t data_len;
 *
 * crc = crc32_init();
 * while ((data_len = read_data(data, MAX_DATA_LEN)) > 0) {
 *     crc = crc32_update(crc, data, data_len);
 * }
 * crc = crc32_finalize(crc);
 * \endcode
 */
#ifndef CRC32_H
#define CRC32_H

#include <stdlib.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


/**
 * The definition of the used algorithm.
 *
 * This is not used anywhere in the generated code, but it may be used by the
 * application code to call algorithm-specific code, if desired.
 */
#define CRC_ALGO_TABLE_DRIVEN 1


/**
 * The type of the CRC values.
 *
 * This type must be big enough to contain at least 32 bits.
 */
typedef uint_fast32_t crc32_t;


/**
 * Reflect all bits of a \a data word of \a data_len bytes.
 *
 * \param[in] data     The data word to be reflected.
 * \param[in] data_len The width of \a data expressed in number of bits.
 * \return             The reflected data.
 */
crc32_t crc32_reflect(crc32_t data, size_t data_len);


/**
 * Calculate the initial crc value.
 *
 * \return     The initial crc value.
 */
static inline crc32_t crc32_init(void)
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
crc32_t crc32_update(crc32_t crc, const void *data, size_t data_len);


/**
 * Calculate the final crc value.
 *
 * \param[in] crc  The current crc value.
 * \return     The final crc value.
 */
static inline crc32_t crc32_finalize(crc32_t crc)
{
    return crc ^ 0xffffffff;
}


#ifdef __cplusplus
}           /* closing brace for extern "C" */
#endif

#endif      /* CRC32_H */
