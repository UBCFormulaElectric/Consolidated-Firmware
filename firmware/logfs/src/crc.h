#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stddef.h>
#include "logfs.h"

    void crc_stampBlock(const LogFs *fs, void *buf);
    bool crc_checkBlock(const LogFs *fs, void *buf);

#ifdef __cplusplus
}
#endif
