#pragma once

#include "logfs.h"
#include <stdint.h>

LogFsErr logfs_blocks_write(const LogFs *fs, uint32_t block);
LogFsErr logfs_blocks_read(const LogFs *fs, uint32_t block);

LogFsErr logfs_blocks_cowWrite(const LogFs *fs, uint32_t block);
LogFsErr logfs_blocks_cowRead(const LogFs *fs, uint32_t block);