#include "io_logfsConfig.h"
#include "hw_gpio.h"

extern SdCard sd;
extern Gpio   sd_present;
extern bool   sd_inited;

static uint8_t cache_buf[HW_DEVICE_SECTOR_SIZE];

static bool sdCardReady()
{
    return sd_inited; // && !hw_gpio_readPin(&sd_present);
}

void io_logfsConfig_getConfig(LogFsCfg *cfg)
{
    cfg->context      = NULL;
    cfg->block_size   = HW_DEVICE_SECTOR_SIZE;
    cfg->block_count  = sd.hsd->SdCard.BlockNbr;
    cfg->read         = io_logfsConfig_read;
    cfg->write        = io_logfsConfig_write;
    cfg->cache        = cache_buf;
    cfg->write_cycles = 100;
    cfg->rd_only      = false;
}

LogFsErr io_logfsConfig_read(const LogFsCfg *cfg, uint32_t block, void *buf)
{
    if (!sdCardReady())
    {
        return LOGFS_ERR_IO;
    }

    if (hw_sd_read(&sd, buf, block, 1) != SD_CARD_OK)
    {
        return LOGFS_ERR_IO;
    }

    return LOGFS_ERR_OK;
}

LogFsErr io_logfsConfig_write(const LogFsCfg *cfg, uint32_t block, void *buf)
{
    if (!sdCardReady())
    {
        return LOGFS_ERR_IO;
    }

    if (hw_sd_write(&sd, buf, block, 1) != SD_CARD_OK)
    {
        return LOGFS_ERR_IO;
    }

    return LOGFS_ERR_OK;
}
