#pragma once

// clang-format off

#define NUM_ELEMENTS_IN_ARRAY(array_pointer) \
    sizeof(array_pointer) / sizeof(array_pointer[0])

/* @brief Extract the basename from a file path */
#define __BASENAME__(path) \
    (__builtin_strrchr(path, '/') ? __builtin_strrchr(path, '/') + 1 : path)

#define min(x, y) (((x) < (y)) ? (x) : (y))

// Extra guard because HAL defines the same macro
#ifndef UNUSED
#define UNUSED(x) (void)(x)
#endif // UNUSED

#define JOIN(a, b) _JOIN(a, b)
#define _JOIN(a, b) a##b

// Pack byte into word
// TODO: Should not require this any longer once code is updated
#define BYTES_TO_WORD(h_byte, l_byte) ((uint16_t)(((h_byte) << 8) | (l_byte)))

// Change endianness of a 16-bit number
#define CHANGE_WORD_ENDIANNESS(word) ((uint16_t)(((word) >> 8U) | ((word) << 8U)))
#define S_TO_MS (1000U)
