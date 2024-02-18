#include "logfs.h"
#include "logfs_util.h"
#include <string.h>
#include <assert.h>

#include <stdio.h>

#define RET_ERR(err)         \
    if (err != LOGFS_ERR_OK) \
    {                        \
        return err;          \
    }

#define RET_VAL_IF_ERR(err, val) \
    if (err != LOGFS_ERR_OK)     \
    {                            \
        return val;              \
    }

static LogFsErr inline logfs_writeBlock(const LogFs *fs, uint32_t block)
{
    logfs_util_stampBlockCrc(fs);
    RET_ERR(fs->cfg->erase(fs->cfg, block));
    RET_ERR(fs->cfg->prog(fs->cfg, block, fs->cfg->block_cache));
    return LOGFS_ERR_OK;
}

static LogFsErr inline logfs_readBlock(const LogFs *fs, uint32_t block)
{
    RET_ERR(fs->cfg->read(fs->cfg, block, fs->cfg->block_cache));
    return logfs_util_checkBlockCrc(fs) ? LOGFS_ERR_OK : LOGFS_ERR_CORRUPT;
}

static LogFsErr inline logfs_writeCowBlock(const LogFs *fs, uint32_t block)
{
    // Power-loss resilience for critical blocks is maintained by keeping a
    // copy. In the event of a failed transaction (due to power-loss or other),
    // the data can be retrieved from the copy. Which one is invalid is
    // determined via calculating a checksum.

    // Write the primary block first.
    uint32_t block_primary = block;
    RET_ERR(logfs_writeBlock(fs, block_primary));

    // Write the second block next.
    uint32_t block_secondary = block + 1;
    RET_ERR(logfs_writeBlock(fs, block_secondary));

    return LOGFS_ERR_OK;
}

static LogFsErr inline logfs_readBlock(const LogFs *fs, uint32_t block)
{
    uint32_t block_primary   = block;
    uint32_t block_secondary = block + 1;

    if (logfs_readBlock(fs, block_primary) == LOGFS_ERR_OK)
    {
        uint32_t   primary_crc     = logfs_util_getBlockCrc(fs);
        const bool secondary_valid = logfs_readBlock(fs, block_secondary) == LOGFS_ERR_OK;
        bool       secondary_match = (secondary_valid && logfs_util_getBlockCrc(fs) == primary_crc);

        if (!secondary_match)
        {
            // CRCs do not match! Since the primary block gets  to
            // first, it has the most recent data, and so the secondary
            // should be updated.
            RET_ERR(logfs_readBlock(fs, block_primary));
            RET_ERR(logfs_writeBlock(fs, block_secondary));
        }
    }
    else if (logfs_readBlock(fs, block_secondary) == LOGFS_ERR_OK)
    {
        // We already know primary is invalid, so update it to the secondary block's copy.
        RET_ERR(logfs_writeBlock(fs, block_primary));
    }
    else
    {
        // Both primary and secondary copies are invalid. Corrupt!
        return LOGFS_ERR_CORRUPT;
    }

    return LOGFS_ERR_OK;
}

static LogFsErr logfs_createNewFile(LogFs *fs, const char *path, uint32_t prev_file)
{
    // Write new file to head.
    uint32_t new_file = fs->head;

    // Create COW block for new file.
    fs->cache_file_info->next = LOGFS_BLOCK_ADDR_INVALID;
    strcpy(fs->cache_file_info->path, path);
    RET_ERR(logfs_writeCowBlock(fs, new_file));

    // Create data header for first data blocks.
    fs->cache_data_hdr->next = LOGFS_BLOCK_ADDR_INVALID;
    RET_ERR(logfs_writeCowBlock(fs, new_file + LOGFS_COW_SIZE));

    // Create emtpy data file.  
    fs->cache_data->bytes = 0U;
    RET_ERR(logfs_writeBlock(fs, new_file + 2 * LOGFS_COW_SIZE));

    if (prev_file != LOGFS_BLOCK_ADDR_INVALID)
    {
        // This means it's the first file, so update the pointer in the
        // filesystem info blocks.
        RET_ERR(logfs_readCowBlock(fs, prev_file));
        fs->cache_file_info->next = new_file;
        RET_ERR(logfs_writeCowBlock(fs, prev_file));
    }

    fs->empty = false;
    fs->head += 2 * LOGFS_COW_SIZE + 1;
    return LOGFS_ERR_OK;
}

LogFsErr logfs_mount(LogFs *fs, const LogFsCfg *cfg) {}

LogFsErr logfs_format(LogFs *fs, const LogFsCfg *cfg)
{
    // Set filesystem config.
    fs->cfg  = cfg;
    fs->head = 2;
    fs->empty = true;

    // Setup cache pointers.
    fs->cache_fs_info   = fs->cfg->block_cache;
    fs->cache_file_info = fs->cfg->block_cache;
    fs->cache_data_hdr  = fs->cfg->block_cache;
    fs->cache_data      = fs->cfg->block_cache;

    const uint32_t data_block_metadata_size = sizeof(LogFsBlock_Data) - 1;
    fs->eff_block_size                      = cfg->block_size - data_block_metadata_size;

    // Write file system info block
    LogFsBlock_FsInfo *const fs_info = fs->cfg->block_cache;
    fs_info->version_major           = LOGFS_VERSION_MAJOR;
    fs_info->version_major           = LOGFS_VERSION_MINOR;
    logfs_util_stampBlockCrc(fs);

    // Write first block.
    RET_ERR(logfs_writeCowBlock(fs, LOGFS_ORIGIN));
    return LOGFS_ERR_OK;
}

LogFsErr logfs_open(LogFs *fs, LogFsFile *file, const char *path)
{
    // If filesystem is empty, just create a new file.
    if (fs->empty)
    {
        return logfs_createNewFile(fs, path, LOGFS_BLOCK_ADDR_INVALID);
    }

    // Traverse the file info blocks as a linked-list.
    uint32_t cur_file = LOGFS_FIRST_FILE;
    while (true)
    {
        RET_ERR(logfs_readCowBlock(fs, cur_file) != LOGFS_ERR_OK);

        if (strcmp(fs->cache_file_info->path, path) == 0)
        {
            // File has been found, return.
            // First block of data is right after the COW file info pair.
            file->info_block = cur_file;
            file->head       = LOGFS_BLOCK_ADDR_INVALID; // TODO: Also get the head pointer!
            return LOGFS_ERR_OK;
        }

        // If we've reached the end of the file linked-list, break out of the
        // loop. Otherwise, jump to the next file.
        if (fs->cache_file_info->next == LOGFS_BLOCK_ADDR_INVALID)
        {
            break;
        }
        else
        {
            cur_file = fs->cache_file_info->next;
        }
    }

    // File couldn't be found, so create a new one.
    return logfs_createNewFile(fs, path, cur_file);
}

LogFsErr logfs_close(LogFs *fs, LogFsFile *file)
{
    // No-op - this doesn't need to do anything since we currently don't cache
    // anything. So anything that needs to be written to disk already has been.
    return LOGFS_ERR_UNIMPLEMENTED;
}

uint32_t logfs_read(LogFs *fs, LogFsFile *file, void *buf, uint32_t size)
{
    return LOGFS_ERR_OK;

    // uint32_t                     cur_data  = file->info_block + LOGFS_COW_SIZE;
    // uint32_t                     num_read  = 0;
    // const LogFsBlock_Data *const file_data = fs->cfg->block_cache;

    // while (num_read < size)
    // {
    //     // Read block into cache.
    //     if (logfs_readBlock(fs, cur_data) != LOGFS_ERR_OK)
    //     {
    //         // Read failure, return what we've read.
    //         return num_read;
    //     }

    //     // Read out data from cached block.
    //     uint32_t num_available = LOGFS_UTIL_MIN(size - num_read, file_data->bytes);
    //     memcpy(((uint8_t *)buf) + num_read, &file_data->data, num_available);
    //     num_read += num_available;

    //     if (file_data->next == LOGFS_BLOCK_ADDR_INVALID)
    //     {
    //         // No more data in file, return what we've read.
    //         return num_read;
    //     }
    //     else
    //     {
    //         cur_data = file_data->next;
    //     }
    // }

    // return num_read;
}

uint32_t logfs_write(LogFs *fs, LogFsFile *file, void *buf, uint32_t size)
{
    uint32_t               cur_data    = file->head;
    uint32_t               prev_data   = LOGFS_BLOCK_ADDR_INVALID;
    uint32_t               num_written = 0;
    bool                   new_block   = false;
    bool                   needs_hdr = false;

    while (num_written < size)
    {
        if(needs_hdr)
        {
            // Create hdr for 
            fs->cache_data_hdr->next = LOGFS_BLOCK_ADDR_INVALID;
            RET_ERR(logfs_writeCowBlock(fs, new_file + LOGFS_COW_SIZE));
        }

        if (new_block && needs_hdr)
        {
            // Update previous block to point to new block.
            // Previous block's data should still be in cache.
            file_data->next = cur_data;
            RET_VAL_IF_ERR(logfs_writeBlock(fs, prev_data), num_written);
        }

        if (new_block || logfs_readBlock(fs, cur_data) != LOGFS_ERR_OK)
        {
            // Init metadata for new block.
            file_data->next  = LOGFS_BLOCK_ADDR_INVALID;
            file_data->bytes = 0;
        }

        // Write new data to disk.
        uint32_t num_available = LOGFS_UTIL_MIN(fs->eff_block_size - file_data->bytes, size - num_written);
        memcpy(&file_data->data + file_data->bytes, ((uint8_t *)buf) + num_written, num_available);
        file_data->bytes += num_available;

        RET_VAL_IF_ERR(logfs_writeBlock(fs, cur_data), num_written);
        num_written += num_available;

        // Head pointers should be moved to current block because we've
        // written to it.
        file->head = cur_data;
        fs->head   = LOGFS_UTIL_MAX(fs->head, cur_data);

        // Need to initialize a new block on next write since we're moving to
        // a new block.
        new_block = true;

        // New data should be written to head.
        prev_data = cur_data;
        cur_data  = fs->head + 1;
    }

    return LOGFS_ERR_OK;
}
