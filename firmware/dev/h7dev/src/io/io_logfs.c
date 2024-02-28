#include "hw_sd.h"
#include "hw_gpio.h"
#include "logfs.h"

extern SdCard sd;
extern Gpio   sd_present;
extern bool   sd_inited;

static bool sd_card_ready()
{
    return sd_inited && !hw_gpio_readPin(&sd_present);
}

LogFsErr io_logfs_read(const LogFsCfg *cfg, uint32_t block, void *buf)
{
    if (!sd_card_ready())
    {
        return LOGFS_ERR_IO;
    }

    if (hw_sd_read(&sd, (uint8_t *)buf, block, 1) != SD_CARD_OK)
    {
        return LOGFS_ERR_IO;
    }

    return LOGFS_ERR_OK;
}

LogFsErr io_logfs_prog(const LogFsCfg *cfg, uint32_t block, void *buf)
{
    if (!sd_card_ready())
    {
        return LOGFS_ERR_IO;
    }
    if (hw_sd_write(&sd, (uint8_t *)buf, (uint32_t)block, 1) != SD_CARD_OK)
    {
        return LOGFS_ERR_IO;
    }
    return LOGFS_ERR_OK;
}

LogFsErr io_logfs_erase(const LogFsCfg *cfg, uint32_t block)
{
    if (!sd_card_ready())
    {
        return LOGFS_ERR_IO;
    }
    if (hw_sd_erase(&sd, block, block))
    {
        return LOGFS_ERR_IO;
    }
    return LOGFS_ERR_OK;
}

LogFsErr io_logfs_massErase(const LogFsCfg *cfg)
{
    if (!sd_card_ready())
    {
        return LOGFS_ERR_IO;
    }
    if (hw_sd_erase(&sd, 0, cfg->block_count - 1))
    {
        return LOGFS_ERR_IO;
    }
    return LOGFS_ERR_OK;
}
