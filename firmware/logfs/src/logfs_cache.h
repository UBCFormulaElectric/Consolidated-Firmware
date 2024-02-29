#pragma once

#include "logfs.h"
#include <stdint.h>

LogFsErr logfs_cache_writeData(const LogFs *fs, uint32_t block, LogFsBlock_Data *buf);
LogFsErr logfs_cache_readData(const LogFs *fs, uint32_t block, LogFsBlock_Data *buf);

LogFsErr logfs_cache_writeSafe(const LogFs *fs, uint32_t block);
LogFsErr logfs_cache_readSafe(const LogFs *fs, uint32_t block);

LogFsErr logfs_cache_writeCopies(const LogFs *fs, uint32_t block);
LogFsErr logfs_cache_readCopies(const LogFs *fs, uint32_t block);