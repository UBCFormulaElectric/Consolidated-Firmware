#include <stdint.h>
#include "hw_sd.h"
#include "logfs.h"

LogFsErr io_logfs_read(const LogFsCfg *cfg, uint32_t block, void *buf);
LogFsErr io_logfs_prog(const LogFsCfg *cfg, uint32_t block, void *buf);
LogFsErr io_logfs_erase(const LogFsCfg *cfg, uint32_t block);
LogFsErr io_logfs_massErase(const LogFsCfg *cfg);