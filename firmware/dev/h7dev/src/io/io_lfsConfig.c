#include "io_lfsConfig.h"
#include "hw_sd.h"
#include "hw_gpio.h"

char lfs_read_buffer[IO_LFS_READ_SIZE];
char lfs_prog_buffer[IO_LFS_PROG_SIZE];

extern SdCard sd;
extern Gpio   sd_present;
extern bool   sd_inited;

static bool sdCardReady()
{
    return sd_inited && !hw_gpio_readPin(&sd_present);
}

int io_lfsConfig_read(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, void *buffer, lfs_size_t size)
{
    if (!sdCardReady())
    {
        return LFS_ERR_IO;
    }
    if (hw_sd_readOffset(
            &sd, (uint8_t *)buffer, (uint32_t)block * IO_LFS_BLOCK_SIZE_FACTOR, (uint32_t)off, (uint32_t)size) !=
        SD_CARD_OK)
    {
        return LFS_ERR_IO;
    }

    return LFS_ERR_OK;
}

int io_lfsConfig_prog(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, const void *buffer, lfs_size_t size)
{
    if (!sdCardReady())
    {
        return LFS_ERR_IO;
    }
    if (hw_sd_writeOffset(
            &sd, (uint8_t *)buffer, (uint32_t)block * IO_LFS_BLOCK_SIZE_FACTOR, (uint32_t)off, (uint32_t)size) !=
        SD_CARD_OK)
    {
        return LFS_ERR_IO;
    }
    return LFS_ERR_OK;
}

int io_lfsConfig_erase(const struct lfs_config *c, lfs_block_t block)
{
    if (!sdCardReady())
    {
        return LFS_ERR_IO;
    }
    uint32_t start = block * IO_LFS_BLOCK_SIZE_FACTOR;
    if (hw_sd_erase(&sd, start, start + IO_LFS_BLOCK_SIZE_FACTOR - 1))
    {
        return LFS_ERR_IO;
    }
    return LFS_ERR_OK;
}

int io_lfsConfig_sync(const struct lfs_config *c)
{
    if (!sdCardReady())
    {
        return LFS_ERR_IO;
    }
    return 0;
}

int io_lfsConfig(struct lfs_config *cfg)
{
    // the function
    cfg->read  = io_lfsConfig_read;
    cfg->prog  = io_lfsConfig_prog;
    cfg->erase = io_lfsConfig_erase;
    cfg->sync  = io_lfsConfig_sync;

    cfg->read_size      = IO_LFS_READ_SIZE;
    cfg->prog_size      = IO_LFS_PROG_SIZE;
    cfg->block_size     = IO_LFS_BLOCK_SIZE;
    cfg->lookahead_size = IO_LFS_LOOKAHEAD_SIZE;
    // cfg->block_count    = block_number / IO_LFS_BLOCK_SIZE_FACTOR;
    cfg->block_count  = 1000000;
    cfg->block_cycles = IO_LFS_BLOCK_CYCLES;
    cfg->cache_size   = IO_LFS_CACHE_SIZE;
    cfg->read_buffer  = lfs_read_buffer;
    cfg->prog_buffer  = lfs_prog_buffer;
    cfg->attr_max     = 0;

    return 0;
}