#include "logfs.h"
#include "logfs_util.h"
#include <string.h>
#include <assert.h>

#include <stdio.h>

#define CHECK_ERR(err) (err != LOGFS_ERR_OK)

#define RET_ERR(err)    \
    if (CHECK_ERR(err)) \
    {                   \
        return err;     \
    }

#define RET_VAL_IF_ERR(err, val) \
    if (CHECK_ERR(err))          \
    {                            \
        return val;              \
    }

static void inline logfs_init(LogFs *fs, const LogFsCfg *cfg)
{
    // Set filesystem config.
    fs->cfg                                 = cfg;
    const uint32_t data_block_metadata_size = sizeof(LogFsBlock_Data) - 1;
    fs->eff_block_size                      = cfg->block_size - data_block_metadata_size;

    // Setup cache pointers.
    fs->cache_fs_info   = fs->cfg->block_cache;
    fs->cache_file_info = fs->cfg->block_cache;
    fs->cache_data_hdr  = fs->cfg->block_cache;
    fs->cache_data      = fs->cfg->block_cache;
}

static void inline logfs_setBlockType(const LogFs *fs, LogFsBlockType type)
{
    // First word is CRC, next byte is block type.
    *((uint8_t *)fs->cfg->block_cache + sizeof(uint32_t)) = (uint8_t)type;
}

static LogFsBlockType inline logfs_getBlockType(const LogFs *fs)
{
    // First word is CRC, next byte is block type.
    return *((uint8_t *)fs->cfg->block_cache + sizeof(uint32_t));
}

static LogFsErr inline logfs_writeBlock(const LogFs *fs, uint32_t block, LogFsBlockType type)
{
    logfs_setBlockType(fs, type);
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

static LogFsErr inline logfs_writeCowBlock(const LogFs *fs, uint32_t block, LogFsBlockType type)
{
    // Power-loss resilience for critical blocks is maintained by keeping a
    // copy. In the event of a failed transaction (due to power-loss or other),
    // the data can be retrieved from the copy. Which one is invalid is
    // determined via calculating a checksum.

    // Write the primary block first.
    uint32_t block_primary = block;
    RET_ERR(logfs_writeBlock(fs, block_primary, type));

    // Write the second block next.
    uint32_t block_secondary = block + 1;
    RET_ERR(logfs_writeBlock(fs, block_secondary, type));

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
            RET_ERR(logfs_writeBlock(fs, block_secondary, logfs_getBlockType(fs)));
        }
    }
    else if (logfs_readBlock(fs, block_secondary) == LOGFS_ERR_OK)
    {
        // We already know primary is invalid, so update it to the secondary block's copy.
        RET_ERR(logfs_writeBlock(fs, block_primary, logfs_getBlockType(fs)));
    }
    else
    {
        // Both primary and secondary copies are invalid. Corrupt!
        return LOGFS_ERR_CORRUPT;
    }

    return LOGFS_ERR_OK;
}

static LogFsErr logfs_createNewFile(LogFs *fs, LogFsFile *file, const char *path, uint32_t prev_file)
{
    // Write new file to head.
    uint32_t new_file = fs->head;

    // Create COW block for new file.
    fs->cache_file_info->next = LOGFS_INVALID_BLOCK;
    strcpy(fs->cache_file_info->path, path);
    RET_ERR(logfs_writeCowBlock(fs, new_file, LOGFS_BLOCK_FILE_INFO));

    // Create data header for first data blocks.
    fs->cache_data_hdr->next = LOGFS_INVALID_BLOCK;
    RET_ERR(logfs_writeCowBlock(fs, new_file + LOGFS_COW_SIZE, LOGFS_BLOCK_DATA_HDR));

    // Create emtpy data file.
    fs->cache_data->bytes = 0U;
    RET_ERR(logfs_writeBlock(fs, new_file + 2 * LOGFS_COW_SIZE, LOGFS_BLOCK_DATA));

    if (prev_file != LOGFS_INVALID_BLOCK)
    {
        // Link previous file to the new one. Do this last so if previous
        // writes fail, the last file won't point to garbage.
        RET_ERR(logfs_readCowBlock(fs, prev_file));
        fs->cache_file_info->next = new_file;
        RET_ERR(logfs_writeCowBlock(fs, prev_file, LOGFS_BLOCK_FILE_INFO));
    }

    // 2 blocks each for file info and header, 1 for data.
    fs->head += 2 * LOGFS_COW_SIZE + 1;
    fs->empty = false;

    // Creating new file succeeded, write metadata to file struct.
    file->info_block = new_file;
    file->last_hdr   = new_file + LOGFS_COW_SIZE;
    file->head       = fs->head - 1;

    return LOGFS_ERR_OK;
}

static uint32_t logfs_findLastDataHdr(LogFs *fs, uint32_t file_info_block)
{
    // Iterate over linked-list of data headers to find the last (most recently
    // written) chunk of data.
    uint32_t file_hdr = file_info_block + LOGFS_COW_SIZE;
    while (true)
    {
        RET_VAL_IF_ERR(logfs_readCowBlock(fs, file_hdr), LOGFS_INVALID_BLOCK);

        if (fs->cache_data_hdr->next != LOGFS_INVALID_BLOCK)
        {
            file_hdr = fs->cache_data_hdr->next;
        }
        else
        {
            // Reached last header.
            return file_hdr;
        }
    }

    // Should never get here.
    return LOGFS_INVALID_BLOCK;
}

static uint32_t logfs_findEndOfChunk(LogFs *fs, uint32_t data_hdr)
{
    // Iterate over data chunk started by the last header.
    uint32_t cur_data = data_hdr + LOGFS_COW_SIZE;
    while (true)
    {
        // If this fails, that probably means we've reached the end of the
        // filesystem image.
        RET_VAL_IF_ERR(logfs_readBlock(fs, cur_data), cur_data - 1);

        if (logfs_getBlockType(fs) != LOGFS_BLOCK_DATA)
        {
            // If we reach another type of block, we've reached the end of the
            // file.
            return cur_data - 1;
        }
        else
        {
            cur_data++;
        }
    }

    // Should never get here.
    return LOGFS_INVALID_BLOCK;
}

LogFsErr logfs_mount(LogFs *fs, const LogFsCfg *cfg)
{
    // Shared initialization.
    logfs_init(fs, cfg);

    // First just try reading the filesystem info blocks.
    RET_ERR(logfs_readCowBlock(fs, LOGFS_ORIGIN));

    // Binary search the filesystem image to find the head, which will be the
    // greatest valid block.
    uint32_t left  = LOGFS_COW_SIZE;
    uint32_t right = fs->cfg->block_count;
    while ((right - left) > 1U)
    {
        const uint32_t mid = (right + left) / 2;
        if (CHECK_ERR(logfs_readBlock(fs, mid)))
        {
            right = mid;
        }
        else
        {
            left = mid;
        }
    }

    // If head is only right after the filesystem info blocks, the filesystem is empty.
    fs->empty = fs->head == LOGFS_COW_SIZE;
    fs->head  = right;
    return LOGFS_ERR_OK;
}

LogFsErr logfs_format(LogFs *fs, const LogFsCfg *cfg)
{
    // Shared initialization.
    logfs_init(fs, cfg);

    fs->head  = LOGFS_COW_SIZE;
    fs->empty = true;

    // Write file system info block
    LogFsBlock_FsInfo *const fs_info = fs->cfg->block_cache;
    fs_info->version_major           = LOGFS_VERSION_MAJOR;
    fs_info->version_major           = LOGFS_VERSION_MINOR;
    logfs_util_stampBlockCrc(fs);

    // Write first block.
    RET_ERR(logfs_writeCowBlock(fs, LOGFS_ORIGIN, LOGFS_BLOCK_FS_INFO));
    return LOGFS_ERR_OK;
}

LogFsErr logfs_open(LogFs *fs, LogFsFile *file, const char *path)
{
    // If filesystem is empty, just create a new file.
    if (fs->empty)
    {
        return logfs_createNewFile(fs, file, path, LOGFS_INVALID_BLOCK);
    }

    // Traverse the file info blocks as a linked-list.
    // First file placed directly after the filesystem info blocks.
    uint32_t cur_file = LOGFS_ORIGIN + LOGFS_COW_SIZE;
    while (true)
    {
        RET_ERR(logfs_readCowBlock(fs, cur_file));

        if (strcmp(fs->cache_file_info->path, path) == 0)
        {
            // File has been found, return.
            // First block of data is right after the COW file info pair.
            file->info_block = cur_file;
            file->last_hdr   = logfs_findLastDataHdr(fs, cur_file);
            file->head       = logfs_findEndOfChunk(fs, file->last_hdr);
            return LOGFS_ERR_OK;
        }

        // If we've reached the end of the file linked-list, break out of the
        // loop. Otherwise, jump to the next file.
        if (fs->cache_file_info->next == LOGFS_INVALID_BLOCK)
        {
            break;
        }
        else
        {
            cur_file = fs->cache_file_info->next;
        }
    }

    // File couldn't be found, so create a new one.
    return logfs_createNewFile(fs, file, path, cur_file);
}

uint32_t logfs_read(LogFs *fs, LogFsFile *file, void *buf, uint32_t size)
{
    uint32_t cur_data = file->info_block + 2 * LOGFS_COW_SIZE;
    uint32_t cur_hdr  = file->info_block + LOGFS_COW_SIZE;
    uint32_t num_read = 0;

    // Read next header's address.
    RET_VAL_IF_ERR(logfs_readCowBlock(fs, cur_hdr), 0);
    uint32_t next_hdr = fs->cache_data_hdr->next;

    while (num_read < size)
    {
        if (cur_data == fs->head)
        {
            // If we've reached the head, we've read the entire filesystem.
            return num_read;
        }

        // Read current block.
        RET_VAL_IF_ERR(logfs_readBlock(fs, cur_data), num_read);

        // If the next block type isn't a data block, that means we've reached
        // the end of the current data chunk.
        if (logfs_getBlockType(fs) != LOGFS_BLOCK_DATA)
        {
            if (next_hdr != LOGFS_INVALID_BLOCK)
            {
                // Reached end of current header, move to the next one.
                cur_data = next_hdr + LOGFS_COW_SIZE;
                RET_VAL_IF_ERR(logfs_readCowBlock(fs, next_hdr), num_read);
                next_hdr = fs->cache_data_hdr->next;
                continue;
            }
            else
            {
                // Reached end of file.
                return num_read;
            }
        }

        // Read out data from cached block.
        uint32_t num_available = LOGFS_UTIL_MIN(size - num_read, fs->cache_data->bytes);
        memcpy(((uint8_t *)buf) + num_read, &fs->cache_data->data, num_available);
        num_read += num_available;
        cur_data++;
    }

    return num_read;
}

uint32_t logfs_write(LogFs *fs, LogFsFile *file, void *buf, uint32_t size)
{
    uint32_t num_written = 0;     // Number of bytes successfully written to disk.
    bool     new_block   = false; // Whether or not a new data block needs to be created.
    bool     new_hdr     = false; // If we need to make a new header for the next block.

    // Read file header data block into cache.
    RET_ERR(logfs_readBlock(fs, file->head));

    while (num_written < size)
    {
        uint32_t cur_data = file->head; // Current file data block we're updating
        if (new_block || new_hdr)
        {
            // Writes to new blocks are done at the head.
            cur_data = fs->head;
        }

        if (new_hdr)
        {
            // Create new data header COW block.
            fs->cache_data_hdr->next = LOGFS_INVALID_BLOCK;
            RET_ERR(logfs_writeCowBlock(fs, cur_data, LOGFS_BLOCK_DATA_HDR));
            cur_data += LOGFS_COW_SIZE;
        }

        if (new_block)
        {
            // Init metadata for new block.
            fs->cache_data->bytes = 0U;
        }

        // Write new data to disk.
        const uint32_t num_available = LOGFS_UTIL_MIN(fs->eff_block_size - fs->cache_data->bytes, size - num_written);
        memcpy(&fs->cache_data->data + fs->cache_data->bytes, ((uint8_t *)buf) + num_written, num_available);
        fs->cache_data->bytes += num_available;
        const bool cur_block_full = fs->cache_data->bytes == fs->eff_block_size; // Current block is full!
        RET_VAL_IF_ERR(logfs_writeBlock(fs, cur_data, LOGFS_BLOCK_DATA), num_written);
        num_written += num_available;

        if (new_hdr)
        {
            // If creating a new header, update the last header to point to the new one.
            // Also update it in the file handle.
            RET_VAL_IF_ERR(logfs_readCowBlock(fs, file->last_hdr), num_written);
            fs->cache_data_hdr->next = cur_data - LOGFS_COW_SIZE;
            RET_VAL_IF_ERR(logfs_writeCowBlock(fs, file->last_hdr, LOGFS_BLOCK_DATA_HDR), num_written);
            file->last_hdr = cur_data - LOGFS_COW_SIZE;
        }

        // After a successful write, update the head. Update here, because
        // if a previous read/write failed, changing filesystem state
        // prematurely could leave it in a bad state.
        if (new_hdr)
        {
            // 3 new blocks: 2 for COW, 1 for data.
            fs->head += LOGFS_COW_SIZE + 1;
            file->head = fs->head; // We always write new blocks to the head.
        }
        else if (new_block)
        {
            // 1 new block for data.
            fs->head++;
            file->head = fs->head; // We always write new blocks to the head.
        }

        if (cur_block_full)
        {
            new_block = true;

            // If the filesystem head is greater than the last written block,
            // there is other file's data between here and the head. So, we
            // need to add a header
            if (fs->head > cur_data + 1)
            {
                new_hdr = true;
            }
        }
        else
        {
            // There is space left in the current block, don't create anything new.
            new_block = false;
            new_hdr   = false;
        }
    }

    return LOGFS_ERR_OK;
}
