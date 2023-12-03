#include "lfs_config.h"
#include "hw_sd.h"

#define LFS_NO_MALLOC
#define LFS_CACHE_SIZE 512
#define LFS_LOOKAHEAD_SIZE 512

#define LFS_READ_SIZE 512
#define LFS_PROG_SIZE 512
#define LFS_CHACHE_SIZE 512
#define LFS_LOOKAHEAD_SIZE 512
#define LFS_BLOCK_CYCLES 500

char lfs_read_buffer[LFS_READ_SIZE];
char lfs_prog_buffer[LFS_PROG_SIZE];

SdCard sd;

int lfs_read(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, void *buffer, lfs_size_t size)
{
    SdCardStatus status = hw_sd_readOffset(&sd, (uint8_t *)buffer, (uint32_t)block, (uint32_t)off, (uint32_t)size);

    return status;
}

int lfs_prog(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, const void *buffer, lfs_size_t size)
{
    SdCardStatus status = hw_sd_writeOffset(&sd, (uint8_t *)buffer, (uint32_t)block, (uint32_t)off, (uint32_t)size);
    return status;
}

int lfs_erase(const struct lfs_config *c, lfs_block_t block)
{
    SdCardStatus status = hw_sd_erase(&sd, (uint32_t)block, (uint32_t)block);
    return status;
}

int lfs_sync(const struct lfs_config *c)
{
    return 0;
}

int lfs_config_object(uint32_t block_size, uint32_t block_number, struct lfs_config *cfg)
{
    // the function
    cfg->read = lfs_read;
    cfg->prog = lfs_prog;
    cfg->erase = lfs_erase;
    cfg->sync = lfs_sync;

    cfg->read_size = LFS_READ_SIZE;
    cfg->prog_size = LFS_PROG_SIZE;
    cfg->block_size = LFS_CHACHE_SIZE;
    cfg->lookahead_size = LFS_LOOKAHEAD_SIZE;

    cfg->block_size = block_size;
    cfg->block_count = block_number;

    cfg->block_cycles = 500;
    cfg->cache_size = LFS_CACHE_SIZE;
    cfg->lookahead_size = LFS_LOOKAHEAD_SIZE;

    cfg->read_buffer = lfs_read_buffer;
    cfg->prog_buffer = lfs_prog_buffer;

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
