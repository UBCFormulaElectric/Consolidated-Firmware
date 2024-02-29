#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stddef.h>
#include "logfs.h"

    uint32_t logfs_crc_getFromBlock(void *buf);
    void     logfs_crc_stampBlock(const LogFs *fs, void *buf);
    bool     logfs_crc_checkBlock(const LogFs *fs, void *buf);

#ifdef __cplusplus
}
#endif
