#include <lfs.h>
#include "hw_sd.h"
#include <stdint.h>

// those factor will be multiple with the block size
#define IO_LFS_BLOCK_SIZE_FACTOR 8
#define IO_LFS_READ_SIZE_FACTOR 8
#define IO_LFS_PROG_SIZE_FACTOR 8
#define IO_LFS_LOOKAHEAD_SIZE_FACTOR 1
#define IO_LFS_CACHE_SIZE_FACTOR 8 // a multiple of the read and program sizes
#define IO_LFS_BLOCK_CYCLES 500

#define IO_LFS_BLOCK_SIZE HW_DEVICE_SECTOR_SIZE *IO_LFS_BLOCK_SIZE_FACTOR
#define IO_LFS_READ_SIZE HW_DEVICE_SECTOR_SIZE *IO_LFS_READ_SIZE_FACTOR
#define IO_LFS_PROG_SIZE HW_DEVICE_SECTOR_SIZE *IO_LFS_PROG_SIZE_FACTOR
#define IO_LFS_LOOKAHEAD_SIZE HW_DEVICE_SECTOR_SIZE *IO_LFS_LOOKAHEAD_SIZE_FACTOR
#define IO_LFS_CACHE_SIZE HW_DEVICE_SECTOR_SIZE *IO_LFS_CACHE_SIZE_FACTOR

int io_lfs_read(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, void *buffer, lfs_size_t size);
int io_lfs_prog(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, const void *buffer, lfs_size_t size);
int io_lfs_erase(const struct lfs_config *c, lfs_block_t block);
int io_lfs_sync(const struct lfs_config *c);
int io_lfs_config(uint32_t block_size, uint32_t block_number, struct lfs_config *cfg);