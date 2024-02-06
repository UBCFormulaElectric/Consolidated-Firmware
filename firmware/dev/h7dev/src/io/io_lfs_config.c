#include "io_lfs_config.h"
#include "hw_sd.h"
#include "hw_gpio.h"

char lfs_read_buffer[IO_LFS_READ_SIZE];
char lfs_prog_buffer[IO_LFS_PROG_SIZE];

extern SdCard sd;
extern Gpio   sd_present;
extern bool   sd_inited;

static bool sd_card_ready()
{
    return sd_inited && !hw_gpio_readPin(&sd_present);
}

int io_lfs_read(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, void *buffer, lfs_size_t size)
{
    if (!sd_card_ready())
    {
        return LFS_ERR_IO;
    }
    if (hw_sd_readOffset(&sd, (uint8_t *)buffer, (uint32_t)block, (uint32_t)off, (uint32_t)size) != SD_CARD_OK)
    {
        return LFS_ERR_IO;
    }

    return LFS_ERR_OK;
}

int io_lfs_prog(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, const void *buffer, lfs_size_t size)
{
    if (!sd_card_ready())
    {
        return LFS_ERR_IO;
    }
    if (hw_sd_writeOffset(&sd, (uint8_t *)buffer, (uint32_t)block, (uint32_t)off, (uint32_t)size) != SD_CARD_OK)
    {
        return LFS_ERR_IO;
    }
    return LFS_ERR_OK;
}

int io_lfs_erase(const struct lfs_config *c, lfs_block_t block)
{
    if (!sd_card_ready())
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

int io_lfs_sync(const struct lfs_config *c)
{
    if (!sd_card_ready())
    {
        return LFS_ERR_IO;
    }
    return 0;
}

int io_lfs_config(uint32_t block_size, uint32_t block_number, struct lfs_config *cfg)
{
    // the function
    cfg->read  = io_lfs_read;
    cfg->prog  = io_lfs_prog;
    cfg->erase = io_lfs_erase;
    cfg->sync  = io_lfs_sync;

    assert(IO_LFS_BLOCK_SIZE % block_size == 0);

    cfg->read_size      = IO_LFS_READ_SIZE;
    cfg->prog_size      = IO_LFS_PROG_SIZE;
    cfg->block_size     = IO_LFS_BLOCK_SIZE;
    cfg->lookahead_size = IO_LFS_LOOKAHEAD_SIZE;
    cfg->block_count    = block_number / IO_LFS_BLOCK_SIZE_FACTOR;
    cfg->block_cycles   = IO_LFS_BLOCK_CYCLES;
    cfg->cache_size     = IO_LFS_CACHE_SIZE;
    cfg->read_buffer    = lfs_read_buffer;
    cfg->prog_buffer    = lfs_prog_buffer;
    cfg->attr_max       = 0;

    return 0;
}