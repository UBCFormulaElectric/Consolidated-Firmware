#include "logfs.h"
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include "logfs_crc.h"
#include "logfs_util.h"
#include "logfs_blocks.h"

#define CHECK_ARG(arg)                \
    if (arg == NULL)                  \
    {                                 \
        return LOGFS_ERR_INVALID_ARG; \
    }

#define CHECK_BLOCK(block)              \
    if (block == LOGFS_INVALID_BLOCK)   \
    {                                   \
        return LOGFS_ERR_INVALID_BLOCK; \
    }

#define CHECK_PATH(path)                                   \
    CHECK_ARG(path);                                       \
    if (path[0] != '/' || strlen(path) > fs->max_path_len) \
    {                                                      \
        return LOGFS_ERR_INVALID_PATH;                     \
    }

#define CHECK_FS_VALID(fs)          \
    if (!fs->mounted)               \
    {                               \
        return LOGFS_ERR_UNMOUNTED; \
    }                               \
    if (fs->out_of_memory)          \
    {                               \
        return LOGFS_ERR_NOMEM;     \
    }

#define INC_HEAD(fs, num)                           \
    {                                               \
        if (fs->head + num >= fs->cfg->block_count) \
        {                                           \
            fs->out_of_memory = true;               \
            return LOGFS_ERR_NOMEM;                 \
        }                                           \
        fs->head += num;                            \
    }

static void inline logfs_init(LogFs *fs, const LogFsCfg *cfg)
{
    // Set filesystem config.
    fs->cfg                                 = cfg;
    const uint32_t data_block_metadata_size = sizeof(LogFsBlock_Data) - 1;
    fs->eff_block_size                      = cfg->block_size - data_block_metadata_size;
    fs->mounted                             = false;
    fs->out_of_memory                       = false;
    fs->max_path_len                        = fs->cfg->block_size - sizeof(LogFsBlock_FileInfo);

    // Setup cache pointers.
    fs->cache_fs_info   = fs->cfg->block_cache;
    fs->cache_file_info = fs->cfg->block_cache;
    fs->cache_data_hdr  = fs->cfg->block_cache;
    fs->cache_data      = fs->cfg->block_cache;
}

static LogFsErr logfs_createNewFile(LogFs *fs, LogFsFile *file, const char *path, uint32_t prev_file)
{
    CHECK_ARG(fs);
    CHECK_ARG(file);
    CHECK_PATH(path);
    CHECK_FS_VALID(fs);

    // Write new file to head.
    uint32_t new_file = fs->head;

    // Create COW block for new file.
    fs->cache_file_info->next = LOGFS_INVALID_BLOCK;
    strcpy(fs->cache_file_info->path, path);
    RET_ERR(logfs_blocks_cowWrite(fs, new_file, LOGFS_BLOCK_FILE_INFO));

    // Create data header for first data blocks.
    fs->cache_data_hdr->next = LOGFS_INVALID_BLOCK;
    RET_ERR(logfs_blocks_cowWrite(fs, new_file + LOGFS_COW_SIZE, LOGFS_BLOCK_DATA_HDR));

    // Create emtpy data file.
    fs->cache_data->bytes = 0U;
    RET_ERR(logfs_blocks_write(fs, new_file + 2 * LOGFS_COW_SIZE, LOGFS_BLOCK_DATA));

    if (prev_file != LOGFS_INVALID_BLOCK)
    {
        // Link previous file to the new one. Do this last so if previous
        // writes fail, the last file won't point to garbage.
        RET_ERR(logfs_blocks_cowRead(fs, prev_file));
        fs->cache_file_info->next = new_file;
        RET_ERR(logfs_blocks_cowWrite(fs, prev_file, LOGFS_BLOCK_FILE_INFO));
    }

    // 2 blocks each for file info and header, 1 for data.
    INC_HEAD(fs, 2 * LOGFS_COW_SIZE + 1);
    fs->empty = false;

    // Creating new file succeeded, write metadata to file struct.
    file->info_block  = new_file;
    file->last_hdr    = new_file + LOGFS_COW_SIZE;
    file->head        = fs->head - 1;
    file->read_uninit = true;

    return LOGFS_ERR_OK;
}

static LogFsErr logfs_findLastDataHdr(LogFs *fs, uint32_t file_info_block, uint32_t *last_hdr)
{
    CHECK_ARG(fs);
    CHECK_BLOCK(file_info_block);
    CHECK_ARG(last_hdr);
    CHECK_FS_VALID(fs);

    // Iterate over linked-list of data headers to find the last (most recently
    // written) chunk of data.
    *last_hdr = file_info_block + LOGFS_COW_SIZE;
    while (true)
    {
        RET_ERR(logfs_blocks_cowRead(fs, *last_hdr));

        if (fs->cache_data_hdr->next != LOGFS_INVALID_BLOCK)
        {
            *last_hdr = fs->cache_data_hdr->next;
        }
        else
        {
            // Reached last header.
            return LOGFS_ERR_OK;
        }
    }

    // Should never get here.
    return LOGFS_ERR_OK;
}

static LogFsErr logfs_findEndOfChunk(LogFs *fs, uint32_t data_hdr, uint32_t *end_of_chunk)
{
    CHECK_ARG(fs);
    CHECK_BLOCK(data_hdr);
    CHECK_ARG(end_of_chunk);
    CHECK_FS_VALID(fs);

    // Iterate over data chunk started by the given header.
    *end_of_chunk = data_hdr + LOGFS_COW_SIZE;
    while (true)
    {
        // If this fails due to invalid block, that probably means we've
        // reached the end of the filesystem image.
        const LogFsErr err = logfs_blocks_read(fs, *end_of_chunk);
        if (err == LOGFS_ERR_CORRUPT)
        {
            return LOGFS_ERR_OK;
        }
        else
        {
            return err;
        }

        if (logfs_blocks_getType(fs) != LOGFS_BLOCK_DATA)
        {
            // If we reach another type of block, we've reached the end of the
            // file.
            *end_of_chunk -= 1;
            return LOGFS_ERR_OK;
        }
        else
        {
            (*end_of_chunk)++;
        }
    }

    // Should never get here.
    return LOGFS_ERR_OK;
}

LogFsErr logfs_fs_mount(LogFs *fs, const LogFsCfg *cfg)
{
    CHECK_ARG(fs);
    CHECK_ARG(cfg);

    // Shared initialization.
    logfs_init(fs, cfg);

    // First just try reading the filesystem info blocks.
    RET_ERR(logfs_blocks_cowRead(fs, LOGFS_ORIGIN));

    // Binary search the filesystem image to find the head, which will be the
    // greatest valid block.
    uint32_t left  = LOGFS_COW_SIZE;
    uint32_t right = fs->cfg->block_count;
    while ((right - left) > 1U)
    {
        const uint32_t mid      = (right + left) / 2;
        const LogFsErr read_err = logfs_blocks_read(fs, mid);
        if (read_err == LOGFS_ERR_OK)
        {
            right = mid;
        }
        else if (read_err == LOGFS_ERR_CORRUPT)
        {
            left = mid;
        }
        else
        {
            return read_err;
        }
    }

    // Update boot count.
    RET_ERR(logfs_blocks_cowRead(fs, LOGFS_ORIGIN));
    fs->cache_fs_info->boot_count++;
    fs->boot_count = fs->cache_fs_info->boot_count;
    RET_ERR(logfs_blocks_cowWrite(fs, LOGFS_ORIGIN, LOGFS_BLOCK_FS_INFO));

    // If head is only right after the filesystem info blocks, the filesystem is empty.
    fs->empty   = fs->head == LOGFS_COW_SIZE;
    fs->head    = right;
    fs->mounted = true;
    return LOGFS_ERR_OK;
}

LogFsErr logfs_fs_format(LogFs *fs, const LogFsCfg *cfg)
{
    CHECK_ARG(fs);
    CHECK_ARG(cfg);

    // Shared initialization.
    logfs_init(fs, cfg);

    // The filesystem design requires that all blocks greater than the current
    // head be invalid (i.e. bad CRC). This means we need to perform a mass
    // erase at formatting. Otherwise, if there are currently blocks on the
    // disk with valid CRCs, such as from previous logfs images, the filesystem
    // operation could be unreliable.
    RET_ERR(fs->cfg->mass_erase(fs->cfg));

    // Write file system info block
    LogFsBlock_FsInfo *const fs_info = fs->cfg->block_cache;
    fs_info->boot_count              = 1;

    // Write first block.
    RET_ERR(logfs_blocks_cowWrite(fs, LOGFS_ORIGIN, LOGFS_BLOCK_FS_INFO));

    fs->head       = LOGFS_COW_SIZE;
    fs->empty      = true;
    fs->boot_count = 1;
    fs->mounted    = true;
    return LOGFS_ERR_OK;
}

LogFsErr logfs_fs_mounted(LogFs *fs, bool *mounted)
{
    CHECK_ARG(fs);
    CHECK_ARG(mounted);

    *mounted = fs->mounted;
    return LOGFS_ERR_OK;
}

LogFsErr logfs_fs_bootCount(LogFs *fs, uint32_t *boot_count)
{
    CHECK_ARG(fs);
    CHECK_ARG(boot_count);
    CHECK_FS_VALID(fs);

    *boot_count = fs->boot_count;
    return LOGFS_ERR_OK;
}

LogFsErr logfs_file_open(LogFs *fs, LogFsFile *file, const char *path)
{
    CHECK_ARG(fs);
    CHECK_ARG(file);
    CHECK_PATH(path);
    CHECK_FS_VALID(fs);

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
        RET_ERR(logfs_blocks_cowRead(fs, cur_file));

        if (strcmp(fs->cache_file_info->path, path) == 0)
        {
            // File has been found, return.
            // First block of data is right after the COW file info pair.
            uint32_t last_hdr;
            RET_ERR(logfs_findLastDataHdr(fs, cur_file, &last_hdr));
            uint32_t head;
            RET_ERR(logfs_findEndOfChunk(fs, last_hdr, &head));

            file->info_block  = cur_file;
            file->last_hdr    = last_hdr;
            file->head        = head;
            file->read_uninit = true;
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

LogFsErr logfs_file_read(LogFs *fs, LogFsFile *file, void *buf, uint32_t size, LogFsRead mode, uint32_t *num_read)
{
    CHECK_ARG(fs);
    CHECK_ARG(file);
    CHECK_ARG(buf);
    CHECK_ARG(num_read);
    CHECK_FS_VALID(fs);

    if (mode == LOGFS_READ_START || file->read_uninit)
    {
        // Set read iterator state variables to start of file.
        file->read_cur_data = file->info_block + 2 * LOGFS_COW_SIZE;
        file->read_cur_hdr  = file->info_block + LOGFS_COW_SIZE;
        file->read_cur_byte = 0;
        file->read_uninit   = false;
    }

    // Read next header's address.
    *num_read = 0;
    RET_ERR(logfs_blocks_cowRead(fs, file->read_cur_hdr));
    uint32_t next_hdr = fs->cache_data_hdr->next;

    while (*num_read < size)
    {
        if (file->read_cur_data == fs->head)
        {
            // If we've reached the head, we've read the entire filesystem.
            return LOGFS_ERR_OK;
        }

        // Read current block.
        RET_ERR(logfs_blocks_read(fs, file->read_cur_data));

        // If the next block type isn't a data block, that means we've reached
        // the end of the current data chunk.
        if (logfs_blocks_getType(fs) != LOGFS_BLOCK_DATA)
        {
            if (next_hdr != LOGFS_INVALID_BLOCK)
            {
                // Reached end of current header, move to the next one.
                file->read_cur_data = next_hdr + LOGFS_COW_SIZE;
                RET_ERR(logfs_blocks_cowRead(fs, next_hdr));
                next_hdr = fs->cache_data_hdr->next;
                continue;
            }
            else
            {
                // Reached end of file.
                return LOGFS_ERR_OK;
            }
        }

        // Calculate number of available bytes.
        const uint32_t num_in_block     = fs->cache_data->bytes - file->read_cur_byte;
        const uint32_t num_left_to_read = size - *num_read;
        uint32_t       num_available    = MIN(num_left_to_read, num_in_block);
        if (num_available == 0)
        {
            // No more data left in this block, reached end of file.
            return LOGFS_ERR_OK;
        }

        // Read out data from cached block.
        uint8_t *const buf_ptr  = (uint8_t *)buf + *num_read;
        uint8_t *const read_ptr = &fs->cache_data->data + file->read_cur_byte;
        memcpy(buf_ptr, read_ptr, num_available);
        *num_read += num_available;

        // Move to next block if we've read all bytes from the current block.
        file->read_cur_byte += num_available;
        if (file->read_cur_byte == fs->eff_block_size)
        {
            file->read_cur_byte = 0;
            file->read_cur_data++;
        }
    }

    return LOGFS_ERR_OK;
}

LogFsErr logfs_file_write(LogFs *fs, LogFsFile *file, void *buf, uint32_t size, uint32_t *num_written)
{
    CHECK_ARG(fs);
    CHECK_ARG(file);
    CHECK_ARG(buf);
    CHECK_ARG(num_written);
    CHECK_FS_VALID(fs);

    bool new_block = false; // Whether or not a new data block needs to be created.
    bool new_hdr   = false; // If we need to make a new header for the next block.

    // Read file header data block into cache.
    RET_ERR(logfs_blocks_read(fs, file->head));

    *num_written = 0;
    while (*num_written < size)
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
            RET_ERR(logfs_blocks_cowWrite(fs, cur_data, LOGFS_BLOCK_DATA_HDR));
            cur_data += LOGFS_COW_SIZE;
        }

        if (new_block)
        {
            // Init metadata for new block.
            fs->cache_data->bytes = 0U;
        }

        // Write new data to disk.
        const uint32_t num_available = MIN(fs->eff_block_size - fs->cache_data->bytes, size - *num_written);
        memcpy(&fs->cache_data->data + fs->cache_data->bytes, ((uint8_t *)buf) + *num_written, num_available);
        fs->cache_data->bytes += num_available;
        const bool cur_block_full = fs->cache_data->bytes == fs->eff_block_size; // Current block is full!
        RET_ERR(logfs_blocks_write(fs, cur_data, LOGFS_BLOCK_DATA));
        *num_written += num_available;

        if (new_hdr)
        {
            // If creating a new header, update the last header to point to the new one.
            // Also update it in the file handle.
            RET_ERR(logfs_blocks_cowRead(fs, file->last_hdr));
            fs->cache_data_hdr->next = cur_data - LOGFS_COW_SIZE;
            RET_ERR(logfs_blocks_cowWrite(fs, file->last_hdr, LOGFS_BLOCK_DATA_HDR));
            file->last_hdr = cur_data - LOGFS_COW_SIZE;
        }

        // After a successful write, update the head. Update here, because
        // if a previous read/write failed, changing filesystem state
        // prematurely could leave it in a bad state.
        if (new_hdr)
        {
            // 3 new blocks: 2 for COW, 1 for data.
            INC_HEAD(fs, LOGFS_COW_SIZE + 1);
            file->head = fs->head; // We always write new blocks to the head.
        }
        else if (new_block)
        {
            // 1 new block for data.
            INC_HEAD(fs, 1);
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
