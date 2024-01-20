#include <lfs.h>
#include "hw_sd.h"
#include <stdint.h>

int lfs_read(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, void *buffer, lfs_size_t size);
int lfs_prog(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, const void *buffer, lfs_size_t size);
int lfs_erase(const struct lfs_config *c, lfs_block_t block);
int lfs_sync(const struct lfs_config *c);
int lfs_config_object(uint32_t block_size, uint32_t block_number, struct lfs_config *cfg);