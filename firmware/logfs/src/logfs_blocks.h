#pragma once

#include "logfs.h"
#include <stdint.h>

void logfs_blocks_setType(const LogFs *fs, LogFsBlockType type);
LogFsBlockType logfs_blocks_getType(const LogFs *fs);

LogFsErr logfs_blocks_write(const LogFs *fs, uint32_t block, LogFsBlockType type);
LogFsErr logfs_blocks_read(const LogFs *fs, uint32_t block);

LogFsErr logfs_blocks_cowWrite(const LogFs *fs, uint32_t block, LogFsBlockType type);
LogFsErr logfs_blocks_cowRead(const LogFs *fs, uint32_t block);