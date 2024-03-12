#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "logfs.h"

    static inline LogFsErr disk_writeCache(const LogFs *fs, uint32_t block);
    static inline LogFsErr disk_readCache(const LogFs *fs, uint32_t block);
    static inline LogFsErr disk_writePair(const LogFs *fs, LogFsPair *pair, uint32_t block);
    static inline LogFsErr disk_readPair(const LogFs *fs, LogFsPair *pair, uint32_t block);

#ifdef __cplusplus
}
#endif
