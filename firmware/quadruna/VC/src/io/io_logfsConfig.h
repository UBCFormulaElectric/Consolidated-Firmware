#pragma once

#include "logfs.h"
#include <stdint.h>
#include "hw_sd.h"

void     io_logfsConfig_getConfig(LogFsCfg *cfg);
LogFsErr io_logfsConfig_read(const LogFsCfg *cfg, uint32_t block, void *buf);
LogFsErr io_logfsConfig_write(const LogFsCfg *cfg, uint32_t block, void *buf);
