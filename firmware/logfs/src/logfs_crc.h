#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stddef.h>
#include "logfs.h"

    uint32_t logfs_crc_getFromBlock(const LogFs *fs);
    void     logfs_crc_stampBlock(const LogFs *fs);
    bool     logfs_crc_checkBlock(const LogFs *fs);

#ifdef __cplusplus
}
#endif
