#include "logfs.h"
#include "logfs_util.h"
#include <string.h>
#include <assert.h>

#define RET_ERR(err)         \
    if (err != LOGFS_ERR_OK) \
    {                        \
        return err;          \
    }

static LogFsErr inline logfs_writeBlock(const LogFs *fs, uint32_t block)
{
    logfs_util_stampBlockCrc(fs);
    RET_ERR(fs->cfg->erase(block));
    RET_ERR(fs->cfg->prog(block, fs->cfg->block_buf));
    return LOGFS_ERR_OK;
}

static LogFsErr inline logfs_readBlock(const LogFs *fs, uint32_t block)
{
    RET_ERR(fs->cfg->read(block, fs->cfg->block_buf));
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

static LogFsErr inline logfs_readCowBlock(const LogFs *fs, uint32_t block)
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

LogFsErr logfs_mount(LogFs *fs, const LogFsCfg *cfg) {}

LogFsErr logfs_format(LogFs *fs, const LogFsCfg *cfg)
{
    // Set filesystem config.
    fs->cfg = cfg;

    // Write file system info block
    LogFsBlock_FsInfo *const fs_info = fs->cfg->block_buf;
    fs_info->version_major           = LOGFS_VERSION_MAJOR;
    fs_info->version_major           = LOGFS_VERSION_MINOR;
    fs_info->first_file              = LOGFS_BLOCK_ADDR_INVALID;
    logfs_util_stampBlockCrc(fs);

    // Write first block.
    logfs_writeCowBlock(fs, LOGFS_ORIGIN);
}

LogFsErr logfs_open(LogFs *fs, LogFsFile *file, const char *path)
{
    // Copy the file path to the file struct.
    strcpy(file->path, path);

    // Read filesystem info block to get position of first file.
    RET_ERR(logfs_readBlock(fs, LOGFS_ORIGIN));
    const LogFsBlock_FsInfo *const fs_info = fs->cfg->block_buf;

    uint32_t prev_file = LOGFS_FIRST_FILE;
    uint32_t cur_file  = fs_info->first_file;

    // Traverse the file info blocks as a linked-list.
    LogFsBlock_FileInfo *const file_info = fs->cfg->block_buf;
    while (cur_file != LOGFS_BLOCK_ADDR_INVALID)
    {
        RET_ERR(logfs_readCowBlock(cur_file, file_info));

        if (strcmp(file_info->path, path))
        {
            // File has been found, return.
            // First block of data is right after the COW file info pair.
            file->origin = cur_file;
            file->head   = LOGFS_BLOCK_ADDR_INVALID; // TODO: Also get the head pointer!
            return LOGFS_ERR_OK;
        }

        prev_file = cur_file;
        cur_file  = file_info->next;
    }

    // Couldn't find the file, write new file to disk at the filesystem head.
    uint32_t new_block = fs->head + 1;
    file_info->next    = LOGFS_BLOCK_ADDR_INVALID;
    strcpy(file_info->path, path);
    RET_ERR(logfs_writeCowBlock(fs, new_block));

    // Write empty data block.
    LogFsBlock_FileData *const file_data = fs->cfg->block_buf;
    file_data->bytes                     = 0U;
    file_data->next                      = LOGFS_BLOCK_ADDR_INVALID;
    RET_ERR(logfs_writeBlock(fs, new_block + LOGFS_COW_SIZE));

    // Update previous file to point to new one.
    RET_ERR(logfs_readBlock(fs, prev_file));
    file_info->next = new_block;
    RET_ERR(logfs_writeBlock(fs, prev_file));
    fs->head = new_block + LOGFS_COW_SIZE;

    // Update file object to point to new file.
    file->origin = new_block;
    file->head   = new_block;
    return LOGFS_ERR_OK;
}

LogFsErr logfs_close(LogFs *fs, LogFsFile *file)
{
    // No-op - this doesn't need to do anything since we currently don't cache
    // anything. So anything that needs to be written to disk already has been.
    return LOGFS_ERR_UNIMPLEMENTED;
}

uint32_t logfs_read(LogFs *fs, LogFsFile *file, void *buf, uint32_t size)
{
    uint32_t                         cur_data  = file->origin + LOGFS_COW_SIZE;
    uint32_t                         num_read  = 0;
    const LogFsBlock_FileData *const file_data = fs->cfg->block_buf;

    while (num_read < size)
    {
        // Read block into cache.
        if (logfs_readBlock(fs, cur_data) != LOGFS_ERR_OK)
        {
            // Read failure, return what we've read.
            return num_read;
        }

        // Read out data from cached block.
        uint32_t num_available = LOGFS_UTIL_MIN(size - num_read, file_data->bytes);
        memcpy(((uint8_t *)buf) + num_read, file_data->data, num_available);
        num_read += num_available;

        if (file_data->next == LOGFS_BLOCK_ADDR_INVALID)
        {
            // No more data in file, return what we've read.
            return num_read;
        }
        else
        {
            cur_data = file_data->next;
        }
    }

    return num_read;
}

uint32_t logfs_write(LogFs *fs, LogFsFile *file, void *buf, uint32_t size)
{
    LogFsBlock_FileData *const file_data   = fs->cfg->block_buf;
    uint32_t                   cur_data    = file->head;
    uint32_t                   prev_data   = LOGFS_BLOCK_ADDR_INVALID;
    uint32_t                   num_written = 0;
    bool                       new_block   = false;

    while (num_written < size)
    {
        if (new_block || logfs_readBlock(fs, cur_data) != LOGFS_ERR_OK)
        {
            // Init metadata for new block.
            file_data->next  = LOGFS_BLOCK_ADDR_INVALID;
            file_data->bytes = 0;
        }

        if (new_block)
        {
            // Update previous block to point to new block.
            // Previous block's data should still be in cache.
            file_data->next = cur_data;
            RET_ERR(logfs_writeBlock(fs, prev_data));
        }

        // Write new data to disk.
        uint32_t num_available = LOGFS_UTIL_MIN(fs->cfg->block_size - file_data->bytes, size - num_written);
        memcpy(file_data->data, ((uint8_t *)buf) + num_written, num_available);
        file_data->bytes += num_available;
        RET_ERR(logfs_writeBlock(fs, cur_data));

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
}

int logfs_add(int a, int b)
{
    return a + b;
}
