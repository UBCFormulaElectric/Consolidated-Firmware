#include "hw_sd.h"
#include <stdint.h>

void io_lfsConfig_showdown(void);

#define LFS_NO_MALLOC 1
#define LFS_ASSERT(test) ((test) ? (void)0 : lfsAssert())
#include <lfs.h>
// those factor will be multiple with the block size
#define IO_LFS_BLOCK_SIZE_FACTOR 1
#define IO_LFS_READ_SIZE_FACTOR 1
#define IO_LFS_PROG_SIZE_FACTOR 1
#define IO_LFS_LOOKAHEAD_SIZE_FACTOR 1
#define IO_LFS_CACHE_SIZE_FACTOR 1 // a multiple of the read and program sizes
#define IO_LFS_BLOCK_CYCLES 500
#define IO_LFS_BLOCK_COUNT 1000000

#define IO_LFS_BLOCK_SIZE HW_DEVICE_SECTOR_SIZE *IO_LFS_BLOCK_SIZE_FACTOR
#define IO_LFS_READ_SIZE HW_DEVICE_SECTOR_SIZE *IO_LFS_READ_SIZE_FACTOR
#define IO_LFS_PROG_SIZE HW_DEVICE_SECTOR_SIZE *IO_LFS_PROG_SIZE_FACTOR
#define IO_LFS_LOOKAHEAD_SIZE HW_DEVICE_SECTOR_SIZE *IO_LFS_LOOKAHEAD_SIZE_FACTOR
#define IO_LFS_CACHE_SIZE HW_DEVICE_SECTOR_SIZE *IO_LFS_CACHE_SIZE_FACTOR // a multiple of the read and program sizes

int io_lfsConfig_read(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, void *buffer, lfs_size_t size);
int io_lfsConfig_prog(
    const struct lfs_config *c,
    lfs_block_t              block,
    lfs_off_t                off,
    const void              *buffer,
    lfs_size_t               size);
int io_lfsConfig_erase(const struct lfs_config *c, lfs_block_t block);
int io_lfsConfig_sync(const struct lfs_config *c);
int io_lfsConfig(struct lfs_config *cfg);