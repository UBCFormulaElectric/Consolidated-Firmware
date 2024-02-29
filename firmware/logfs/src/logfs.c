#include "logfs.h"
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include "logfs_crc.h"
#include "logfs_util.h"
#include "logfs_cache.h"

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

#define INC_HEAD(fs, num)                                      \
    {                                                          \
        if (fs->head_data_block + num >= fs->cfg->block_count) \
        {                                                      \
            fs->out_of_memory = true;                          \
            return LOGFS_ERR_NOMEM;                            \
        }                                                      \
        fs->head_data_block += num;                            \
    }

static void inline logfs_init(LogFs *fs, const LogFsCfg *cfg)
{
    // Set filesystem config.
    fs->cfg                                 = cfg;
    const uint32_t data_block_metadata_size = sizeof(LogFsBlock_Data) - 1;
    fs->eff_block_size                      = cfg->block_size - data_block_metadata_size;
    fs->mounted                             = false;
    fs->out_of_memory                       = false;
    fs->max_path_len                        = MIN(fs->cfg->block_size - sizeof(LogFsBlock_File), LOGFS_PATH_BYTES);
}

static LogFsErr logfs_createNewFile(LogFs *fs, LogFsFile *file, const char *path)
{
    CHECK_ARG(fs);
    CHECK_ARG(file);
    CHECK_PATH(path);
    CHECK_FS_VALID(fs);
    LogFsBlock_File *buf_file = fs->cfg->safe_cache;
    LogFsBlock_Data *buf_data = fs->cfg->safe_cache;

    // Write new file to head.
    const uint32_t new_file_block  = fs->head_data_block;
    const uint32_t last_file_block = fs->head_file_block;

    // Create redundant block for new file.
    buf_file->next_file_block = LOGFS_INVALID_BLOCK;
    strcpy(buf_file->path, path);
    RET_ERR(logfs_cache_writeCopies(fs, new_file_block));

    // Create emtpy data file.
    buf_data->bytes = 0U;
    RET_ERR(logfs_cache_writeSafe(fs, new_file_block + LOGFS_NUM_COPIES));

    if (fs->head_file_block != LOGFS_INVALID_BLOCK)
    {
        // Link previous file to the new one. Do this last so if previous
        // writes fail, the last file won't point to garbage.
        RET_ERR(logfs_cache_readCopies(fs, last_file_block));
        buf_file->next_file_block = new_file_block;
        RET_ERR(logfs_cache_writeCopies(fs, last_file_block));
    }

    // 2 blocks for file, 1 for data.
    INC_HEAD(fs, LOGFS_NUM_COPIES + 1);
    fs->head_file_block = new_file_block;

    // Creating new file succeeded, write metadata to file struct.
    file->file_block  = new_file_block;
    file->head_block  = new_file_block + LOGFS_NUM_COPIES;
    file->read_uninit = true;
    return LOGFS_ERR_OK;
}

static LogFsErr logfs_findFileHead(LogFs *fs, uint32_t file_block, uint32_t *head_block)
{
    CHECK_ARG(fs);
    CHECK_BLOCK(file_block);
    CHECK_ARG(head_block);
    CHECK_FS_VALID(fs);
    LogFsBlock_Data *buf_data = fs->cfg->safe_cache;

    // Iterate over data chunk started by the given header.
    *head_block = file_block + LOGFS_NUM_COPIES;
    while (true)
    {
        const LogFsErr err = logfs_cache_readSafe(fs, *head_block);
        if (err == LOGFS_ERR_CORRUPT)
        {
            // If this fails due to invalid block, that probably means we've
            // reached the end of the filesystem image.
            return LOGFS_ERR_OK;
        }
        else
        {
            // Other read error, return.
            return err;
        }

        const uint32_t next_data_block = buf_data->next_block;
        if (next_data_block == LOGFS_INVALID_BLOCK)
        {
            // No next block means we've reached the end of the file, return.
            return LOGFS_ERR_OK;
        }
        else
        {
            *head_block = next_data_block;
        }
    }

    // Should never get here.
    return LOGFS_ERR_OK;
}

LogFsErr logfs_mount(LogFs *fs, const LogFsCfg *cfg)
{
    CHECK_ARG(fs);
    CHECK_ARG(cfg);

    // Shared initialization.
    logfs_init(fs, cfg);
    LogFsBlock_Fs *buf_fs = fs->cfg->safe_cache;

    // First just try reading the filesystem info blocks.
    RET_ERR(logfs_cache_readCopies(fs, LOGFS_ORIGIN));

    // Update boot count.
    RET_ERR(logfs_cache_readCopies(fs, LOGFS_ORIGIN));
    buf_fs->boot_count++;
    fs->boot_count = buf_fs->boot_count;
    RET_ERR(logfs_cache_writeCopies(fs, LOGFS_ORIGIN));

    // If head is only right after the filesystem info blocks, the filesystem is empty.
    fs->head_data_block = LOGFS_INVALID_BLOCK; // TODO: Find head file/data!
    fs->head_file_block = LOGFS_INVALID_BLOCK; // TODO: Find head file/data!
    fs->mounted         = true;
    return LOGFS_ERR_OK;
}

LogFsErr logfs_format(LogFs *fs, const LogFsCfg *cfg)
{
    CHECK_ARG(fs);
    CHECK_ARG(cfg);

    // Shared initialization.
    logfs_init(fs, cfg);
    LogFsBlock_Fs *buf_fs = fs->cfg->safe_cache;

    // Write file system info block
    buf_fs->boot_count = 1;

    // Write first block and erase next.
    RET_ERR(logfs_cache_writeCopies(fs, LOGFS_ORIGIN));
    RET_ERR(fs->cfg->erase(fs->cfg, LOGFS_ORIGIN + LOGFS_NUM_COPIES));
    RET_ERR(fs->cfg->erase(fs->cfg, LOGFS_ORIGIN + LOGFS_NUM_COPIES + 1));

    fs->head_file_block = LOGFS_INVALID_BLOCK;
    fs->head_data_block = LOGFS_NUM_COPIES;
    fs->boot_count      = 1;
    fs->mounted         = true;
    return LOGFS_ERR_OK;
}

LogFsErr logfs_bootCount(LogFs *fs, uint32_t *boot_count)
{
    CHECK_ARG(fs);
    CHECK_ARG(boot_count);
    CHECK_FS_VALID(fs);

    *boot_count = fs->boot_count;
    return LOGFS_ERR_OK;
}

LogFsErr logfs_open(LogFs *fs, LogFsFile *file, const char *path)
{
    CHECK_ARG(fs);
    CHECK_ARG(file);
    CHECK_PATH(path);
    CHECK_FS_VALID(fs);
    LogFsBlock_File *buf_file = fs->cfg->safe_cache;

    // Traverse the file info blocks as a linked-list.
    // First file placed directly after the filesystem info blocks.
    uint32_t cur_file_block = LOGFS_ORIGIN + LOGFS_NUM_COPIES;
    while (true)
    {
        if (cur_file_block == LOGFS_INVALID_BLOCK)
        {
            // Invalid block, means we've reached the end of the filesystem
            // linked-list without finding the file. So we need to create a new
            // file.
            return logfs_createNewFile(fs, file, path);
        }

        const LogFsErr err = logfs_cache_readCopies(fs, cur_file_block);
        if (cur_file_block == LOGFS_INVALID_BLOCK || err == LOGFS_ERR_CORRUPT)
        {
            // Next file is corrup, so create a new file in its place.
            return logfs_createNewFile(fs, file, path);
        }
        else if (CHECK_ERR(err))
        {
            // Other read error, return.
            return err;
        }

        if (strcmp(buf_file->path, path) == 0)
        {
            // File has been found, return.
            uint32_t file_head_block;
            RET_ERR(logfs_findFileHead(fs, cur_file_block, &file_head_block));
            file->file_block  = cur_file_block;
            file->head_block  = file_head_block;
            file->read_uninit = true;

            return LOGFS_ERR_OK;
        }

        // Jump to the next file in the list.
        cur_file_block = buf_file->next_file_block;
    }

    // Should never get here.
    return LOGFS_ERR_OK;
}

LogFsErr logfs_read(LogFs *fs, LogFsFile *file, void *buf, uint32_t size, LogFsRead mode, uint32_t *num_read)
{
    CHECK_ARG(fs);
    CHECK_ARG(file);
    CHECK_ARG(buf);
    CHECK_ARG(num_read);
    CHECK_FS_VALID(fs);
    LogFsBlock_Data *buf_data = fs->cfg->safe_cache;
    LogFsBlock_File *buf_file = fs->cfg->safe_cache;

    if (mode == LOGFS_READ_START || file->read_uninit)
    {
        // Set read iterator state variables to start of file.
        file->read_cur_data = file->file_block + LOGFS_NUM_COPIES;
        file->read_cur_byte = 0;
        file->read_uninit   = false;
    }

    *num_read = 0;
    while (*num_read < size)
    {
        if (file->read_cur_data == fs->head_data_block || file->read_cur_data == LOGFS_INVALID_BLOCK)
        {
            // If we've reached the head, we've read the entire filesystem.
            // If the next file data is invalid, we've reached the end of the
            // file.
            return LOGFS_ERR_OK;
        }

        // Read current block.
        RET_ERR(logfs_cache_readSafe(fs, file->read_cur_data));

        // Calculate number of available bytes.
        const uint32_t num_in_block     = buf_data->bytes - file->read_cur_byte;
        const uint32_t num_left_to_read = size - *num_read;
        uint32_t       num_available    = MIN(num_left_to_read, num_in_block);

        if (num_available == 0)
        {
            // No more data left in this block, reached end of file.
            return LOGFS_ERR_OK;
        }

        // Read out data from cached block.
        uint8_t *const buf_ptr  = (uint8_t *)buf + *num_read;
        uint8_t *const read_ptr = &buf_data->data + file->read_cur_byte;
        memcpy(buf_ptr, read_ptr, num_available);
        *num_read += num_available;

        // Move to next block if we've read all bytes from the current block.
        file->read_cur_byte += num_available;
        if (file->read_cur_byte == fs->eff_block_size)
        {
            file->read_cur_byte = 0;
            file->read_cur_data = buf_data->next_block;
        }
    }

    return LOGFS_ERR_OK;
}

LogFsErr logfs_write(LogFs *fs, LogFsFile *file, void *buf, uint32_t size, uint32_t *num_written)
{
    CHECK_ARG(fs);
    CHECK_ARG(file);
    CHECK_ARG(buf);
    CHECK_ARG(num_written);
    CHECK_FS_VALID(fs);
    LogFsBlock_Data *buf_data = fs->cfg->safe_cache;
    LogFsBlock_File *buf_file = fs->cfg->safe_cache;

    bool new_block = false; // Whether or not a new data block needs to be created.

    // Read file head block into cache.
    RET_ERR(logfs_cache_readSafe(fs, file->head_block));

    *num_written = 0;
    while (*num_written < size)
    {
        uint32_t cur_data_block = file->head_block;
        if (new_block)
        {
            // Writes to new blocks are done at the head.
            cur_data_block       = fs->head_data_block;
            buf_data->next_block = LOGFS_INVALID_BLOCK;
            buf_data->bytes      = 0U;
        }

        // Write new data to disk.
        const uint32_t num_available = MIN(fs->eff_block_size - buf_data->bytes, size - *num_written);
        memcpy(&buf_data->data + buf_data->bytes, ((uint8_t *)buf) + *num_written, num_available);
        buf_data->bytes += num_available;
        RET_ERR(logfs_cache_writeSafe(fs, cur_data_block));
        *num_written += num_available;

        // After a new block, update the heads. Update here, because
        // if a previous read/write failed, changing filesystem state
        // prematurely could leave it in a bad state.
        if (new_block)
        {
            // If creating a new block, update the last block to point to the
            // new one. The file's "head block" will still point to the last
            // block.
            RET_ERR(logfs_cache_readSafe(fs, file->head_block));
            buf_data->next_block = cur_data_block;
            RET_ERR(logfs_cache_writeSafe(fs, file->head_block));

            // After a successful write, now we can update filesystem state.
            file->head_block = cur_data_block;
            INC_HEAD(fs, 1);
        }

        // We'll always need a new block if there is any remaining data to be
        // written to disk.
        new_block = true;
    }

    return LOGFS_ERR_OK;
}

LogFsErr logfs_firstPath(LogFs *fs, LogFsPath *path)
{
    CHECK_ARG(fs);
    CHECK_ARG(path);
    CHECK_FS_VALID(fs);
    LogFsBlock_File *buf_file = fs->cfg->safe_cache;

    const uint32_t first_file = LOGFS_ORIGIN + LOGFS_NUM_COPIES;
    const LogFsErr err        = logfs_cache_readCopies(fs, first_file);
    if (err == LOGFS_ERR_CORRUPT)
    {
        // This probably just means there are no files.
        return LOGFS_ERR_EMPTY;
    }
    else if (CHECK_ERR(err))
    {
        // Other error, return.
        return err;
    }

    path->file_block      = first_file;
    path->next_file_block = buf_file->next_file_block;
    strcpy(path->path, buf_file->path);
    return LOGFS_ERR_OK;
}

LogFsErr logfs_nextPath(LogFs *fs, LogFsPath *path)
{
    CHECK_ARG(fs);
    CHECK_ARG(path);
    CHECK_FS_VALID(fs);
    LogFsBlock_File *buf_file = fs->cfg->safe_cache;

    if (path->next_file_block == LOGFS_INVALID_BLOCK)
    {
        // Reached end of files.
        return LOGFS_ERR_INVALID_PATH;
    }

    // Read the next file info block and retrieve the path.
    RET_ERR(logfs_cache_readCopies(fs, path->next_file_block));
    path->file_block      = path->next_file_block;
    path->next_file_block = buf_file->next_file_block;
    strcpy(path->path, buf_file->path);
    return LOGFS_ERR_OK;
}