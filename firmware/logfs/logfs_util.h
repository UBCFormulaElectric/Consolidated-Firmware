#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stddef.h>
#include "logfs.h"

#define LOGFS_UTIL_MIN(x, y) (((x) < (y)) ? (x) : (y))
#define LOGFS_UTIL_MAX(x, y) (((x) > (y)) ? (x) : (y))

    uint32_t logfs_util_getBlockCrc(const LogFs *fs);
    void     logfs_util_stampBlockCrc(const LogFs *fs);
    bool     logfs_util_checkBlockCrc(const LogFs *fs);

#ifdef __cplusplus
}
#endif
