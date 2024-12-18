#pragma once
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Utility macro to define atoi-style functions for arbritrary uint types.
// Generates functions of name sil_atoi_TYPE, ie. sil_atoi_uint32_t.
// Returns 0 if invalid input.
#define SIL_ATOI_UINT_DEF(TYPE)                            \
    TYPE sil_atoi_##TYPE(char *in)                         \
    {                                                      \
        TYPE res = 0;                                      \
        int  len = strlen(in);                             \
        for (int i = 0; i < len; i += 1)                   \
        {                                                  \
            if ('0' > in[i] || in[i] > '9')                \
                return 0;                                  \
            res += (in[i] - '0') * pow(10, (len - i - 1)); \
        }                                                  \
        return res;                                        \
    }

SIL_ATOI_UINT_DEF(uint32_t)
SIL_ATOI_UINT_DEF(uint64_t)