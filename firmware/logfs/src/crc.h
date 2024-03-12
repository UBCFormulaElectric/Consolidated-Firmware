#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stddef.h>
#include "logfs.h"

    uint32_t crc_getFromBlock(const LogFs *fs);
    void     crc_stampBlock(const LogFs *fs);
    bool     crc_checkBlock(const LogFs *fs);

#ifdef __cplusplus
}
#endif
