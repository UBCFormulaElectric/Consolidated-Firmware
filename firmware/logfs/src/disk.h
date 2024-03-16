#pragma once

#include "logfs.h"

#ifdef __cplusplus
extern "C"
{
#endif

    inline LogFsErr disk_write(const LogFs *fs, uint32_t block, void *buf);
    inline LogFsErr disk_read(const LogFs *fs, uint32_t block, void *buf);

    LogFsErr disk_changeCache(const LogFs *fs, LogFsCache *cache, uint32_t block, bool write_back, bool fetch);
    LogFsErr disk_syncCache(const LogFs *fs, const LogFsCache *cache);

    void     disk_newPair(LogFsPair *pair, uint32_t block);
    LogFsErr disk_fetchPair(const LogFs *fs, LogFsPair *pair, uint32_t block);
    LogFsErr disk_writePair(const LogFs *fs, LogFsPair *pair);
    LogFsErr disk_readPair(const LogFs *fs, LogFsPair *pair);

#ifdef __cplusplus
}
#endif
