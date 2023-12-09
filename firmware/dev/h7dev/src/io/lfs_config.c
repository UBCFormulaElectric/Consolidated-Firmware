#include "lfs_config.h"
#include "hw_sd.h"
#include "App_SharedMacros.h"

#define LFS_NO_MALLOC

#define SD_BLOCK_SIZE 512
#define LFS_BLOCK_MULTIPLIER 64
#define LFS_BLOCK_SIZE (LFS_BLOCK_MULTIPLIER * SD_BLOCK_SIZE)

#define LFS_READ_SIZE SD_BLOCK_SIZE
#define LFS_PROG_SIZE SD_BLOCK_SIZE

#define LFS_CACHE_SIZE (SD_BLOCK_SIZE)
#define LFS_LOOKAHEAD_SIZE LFS_CACHE_SIZE

char lfs_read_buffer[LFS_CACHE_SIZE];
char lfs_prog_buffer[LFS_CACHE_SIZE];
char lfs_lookahead_buffer[LFS_CACHE_SIZE];

SdCard sd;

int lfs_read(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, void *buffer, lfs_size_t size)
{
    uint32_t sd_off_num_blocks = off / SD_BLOCK_SIZE;
    uint32_t sd_block_start    = block * LFS_BLOCK_MULTIPLIER + sd_off_num_blocks;

    uint32_t sd_block          = sd_block_start;
    uint32_t sd_off            = off - sd_off_num_blocks * SD_BLOCK_SIZE;
    uint32_t bytes_transferred = 0;

    while (size - bytes_transferred > 0)
    {
        uint32_t bytes_to_transfer = MIN(size - bytes_transferred, SD_BLOCK_SIZE);
        hw_sd_readOffset(&sd, (uint8_t *)buffer + bytes_transferred, sd_block, sd_off, bytes_to_transfer);

        sd_block += 1;
        sd_off = 0;
        bytes_transferred += bytes_to_transfer;
    }

    return 0;
}

int lfs_prog(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, const void *buffer, lfs_size_t size)
{
    uint32_t sd_off_num_blocks = off / SD_BLOCK_SIZE;
    uint32_t sd_block_start    = block * LFS_BLOCK_MULTIPLIER + sd_off_num_blocks;

    uint32_t sd_block          = sd_block_start;
    uint32_t sd_off            = off - sd_off_num_blocks * SD_BLOCK_SIZE;
    uint32_t bytes_transferred = 0;

    while (size - bytes_transferred > 0)
    {
        uint32_t bytes_to_transfer = MIN(size - bytes_transferred, SD_BLOCK_SIZE);
        hw_sd_writeOffset(&sd, (uint8_t *)buffer + bytes_transferred, sd_block, sd_off, bytes_to_transfer);

        sd_block += 1;
        sd_off = 0;
        bytes_transferred += bytes_to_transfer;
    }

    return 0;
}

int lfs_erase(const struct lfs_config *c, lfs_block_t block)
{
    hw_sd_erase(&sd, block * LFS_BLOCK_MULTIPLIER, (block + 1) * LFS_BLOCK_MULTIPLIER);
    return 0;
}

int lfs_sync(const struct lfs_config *c)
{
    return 0;
}

int lfs_config_object(uint32_t block_size, uint32_t block_number, struct lfs_config *cfg)
{
    // the function
    cfg->read  = lfs_read;
    cfg->prog  = lfs_prog;
    cfg->erase = lfs_erase;
    cfg->sync  = lfs_sync;

    cfg->read_size      = LFS_READ_SIZE;
    cfg->prog_size      = LFS_PROG_SIZE;
    cfg->lookahead_size = LFS_LOOKAHEAD_SIZE;

    cfg->block_size  = LFS_BLOCK_SIZE;
    cfg->block_count = block_number / LFS_BLOCK_MULTIPLIER;

    cfg->block_cycles = -1;
    cfg->cache_size   = LFS_CACHE_SIZE;

    cfg->read_buffer      = lfs_read_buffer;
    cfg->prog_buffer      = lfs_prog_buffer;
    cfg->lookahead_buffer = lfs_lookahead_buffer;

    // #ifdef LFS_THREADSAFE
    //     // Lock the underlying block device. Negative error codes
    //     // are propagated to the user.
    //     // int (*lock)(const struct lfs_config *c);

    //     // // Unlock the underlying block device. Negative error codes
    //     // // are propagated to the user.
    //     // int (*unlock)(const struct lfs_config *c);
    // #endif

    return 0;
}
