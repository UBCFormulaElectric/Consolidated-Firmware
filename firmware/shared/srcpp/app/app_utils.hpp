#pragma once

#define NUM_ELEMENTS_IN_ARRAY(array_pointer) sizeof(array_pointer) / sizeof(array_pointer[0])

/* @brief Extract the basename from a file path */
#define __BASENAME__(path) (__builtin_strrchr(path, '/') ? __builtin_strrchr(path, '/') + 1 : path)

// Extra guard because HAL defines the same macro
#ifndef UNUSED
#define UNUSED(x) (void)(x)
#endif // UNUSED

#define EMPTY_STRUCT(name)     \
    typedef struct             \
    {                          \
        unsigned char _unused; \
    } name;

#ifdef __cplusplus
#define CFUNC extern "C"
#define NORET [[noreturn]]
#else
#define CFUNC
#define NORET _Noreturn
#endif

#define forever for (;;)