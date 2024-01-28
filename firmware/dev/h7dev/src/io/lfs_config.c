#include "lfs_config.h"
#include "hw_sd.h"
#include "hw_gpio.h"

// those factor will be multiple with the block size
#define LFS_READ_SIZE_FACTOR 1
#define LFS_PROG_SIZE_FACTOR 1
#define LFS_LOOKAHEAD_SIZE_FACTOR 1
#define LFS_CACHE_SIZE_FACTOR 8 // a multiple of the read and program sizes
#define LFS_FILE_CACHE_SIZE_FACTOR 8
#define LFS_BLOCK_CYCLES 500

#define LFS_BLOCK_SIZE 512
#define LFS_READ_SIZE LFS_BLOCK_SIZE *LFS_READ_SIZE_FACTOR
#define LFS_PROG_SIZE LFS_BLOCK_SIZE *LFS_PROG_SIZE_FACTOR
#define LFS_LOOKAHEAD_SIZE LFS_BLOCK_SIZE *LFS_LOOKAHEAD_SIZE_FACTOR
#define LFS_CACHE_SIZE LFS_BLOCK_SIZE *LFS_CACHE_SIZE_FACTOR
#define LFS_FILE_CACHE_SIZE LFS_BLOCK_SIZE *LFS_FILE_CACHE_SIZE_FACTOR

char *lfs_read_buffer;
char *lfs_prog_buffer;

extern SdCard      sd;
extern Gpio sd_present;
extern bool sd_inited;

static bool sd_card_ready()
{
    return sd_inited && !hw_gpio_readPin(&sd_present);
}

int lfs_read(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, void *buffer, lfs_size_t size)
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

int lfs_prog(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, const void *buffer, lfs_size_t size)
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

int lfs_erase(const struct lfs_config *c, lfs_block_t block)
{
    if (!sd_card_ready())
    {
        return LFS_ERR_IO;
    }
    if (hw_sd_erase(&sd, (uint32_t)block, (uint32_t)block))
    {
        return LFS_ERR_IO;
    }
    return LFS_ERR_OK;
}

int lfs_sync(const struct lfs_config *c)
{
    if (!sd_card_ready())
    {
        return LFS_ERR_IO;
    }
    return 0;
}

int lfs_config_object(uint32_t block_size, uint32_t block_number, struct lfs_config *cfg)
{
    // the function
    cfg->read  = lfs_read;
    cfg->prog  = lfs_prog;
    cfg->erase = lfs_erase;
    cfg->sync  = lfs_sync;

    assert(LFS_BLOCK_SIZE == block_size);

    cfg->read_size      = LFS_READ_SIZE;
    cfg->prog_size      = LFS_PROG_SIZE;
    cfg->block_size     = LFS_BLOCK_SIZE;
    cfg->lookahead_size = LFS_LOOKAHEAD_SIZE;
    cfg->block_size     = BLOCKSIZE;
    cfg->block_count    = block_number;
    cfg->block_cycles   = LFS_BLOCK_CYCLES;
    cfg->cache_size     = LFS_CACHE_SIZE;
    cfg->read_buffer    = lfs_read_buffer;
    cfg->prog_buffer    = lfs_prog_buffer;

    return 0;
}