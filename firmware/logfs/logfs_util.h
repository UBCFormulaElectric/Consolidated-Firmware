#pragma once
#include <stdint.h>
#include <stddef.h>
#include "logfs.h"

#define LOGFS_UTIL_MIN(x, y) (((x) < (y)) ? (x) : (y))
#define LOGFS_UTIL_MAX(x, y) (((x) > (y)) ? (x) : (y))

void logfs_util_stampBlockCrc(const LogFs* fs, const void *buf);
bool logfs_util_checkBlockCrc(const LogFs* fs, const void *buf);
