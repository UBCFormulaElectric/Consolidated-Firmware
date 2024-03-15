#include "logfs.h"
#include <string.h>
#include <stdio.h>
#include "disk.h"
#include "utils.h"
#include "crc.h"

static void inline logfs_init(LogFs *fs, const LogFsCfg *cfg)
{
    fs->cfg                                 = cfg;
    const uint32_t data_block_metadata_size = sizeof(LogFsBlock_Data) - 1;
    fs->eff_block_size                      = cfg->block_size - data_block_metadata_size;
    fs->mounted                             = false;
    fs->out_of_memory                       = false;
    fs->max_path_len                        = MIN(fs->cfg->block_size - sizeof(LogFsBlock_File), LOGFS_PATH_BYTES);
    fs->cache_file                          = fs->cfg->cache;
    fs->cache_data                          = fs->cfg->cache;
    fs->head_file                           = LOGFS_INVALID_BLOCK;
    fs->head                                = 0;
}

static void inline logfs_initFile(LogFs *fs, LogFsFile *file, const LogFsFileCfg *cfg)
{
    file->cfg         = cfg;
    file->cache.block = LOGFS_INVALID_BLOCK;
    file->cache.buf   = cfg->cache;
    file->cache_data  = (LogFsBlock_Data *)cfg->cache;
}

static LogFsErr logfs_createNewFile(LogFs *fs, LogFsFile *file)
{
    CHECK_ARG(fs);
    CHECK_ARG(file);

    const uint32_t new_file_block = fs->head;
    const uint32_t new_data_block = fs->head + LOGFS_PAIR_SIZE;

    // Write file pair to disk.
    fs->cache_file->head = new_data_block;
    fs->cache_file->next = LOGFS_INVALID_BLOCK;
    strcpy(fs->cache_file->path, file->cfg->path);

    disk_newPair(&file->pair, new_file_block);
    RET_ERR(disk_writePair(fs, &file->pair));

    // Write empty data block to disk.
    file->cache_data->bytes = 0;
    file->cache_data->prev  = LOGFS_INVALID_BLOCK;
    RET_ERR(disk_write(fs, new_data_block, fs->cfg->cache));

    if (fs->head_file != LOGFS_INVALID_BLOCK)
    {
        // Update previous file to point to the new one.
        LogFsPair prev_file_pair;
        RET_ERR(disk_fetchPair(fs, &prev_file_pair, fs->head_file));
        RET_ERR(disk_readPair(fs, &prev_file_pair));
        fs->cache_file->next = new_file_block;
        RET_ERR(disk_writePair(fs, &prev_file_pair));
    }

    // Update filesystem state variables (do this last so if previous writes
    // fail, the filesystem won't be left in a bad state).
    file->head      = new_data_block;
    file->read_init = false;
    fs->head_file   = new_file_block;
    INC_HEAD(fs, LOGFS_PAIR_SIZE + 1);
    return LOGFS_ERR_OK;
}

LogFsErr logfs_format(LogFs *fs, const LogFsCfg *cfg)
{
    CHECK_ARG(fs);
    CHECK_ARG(cfg);

    // Shared initialization.
    logfs_init(fs, cfg);

    // Create root file.
    fs->head = LOGFS_ORIGIN;

    // Initialize file cache.
    LogFsFileCfg root_cfg = { .cache = cfg->cache, .path = "/.root" };
    logfs_initFile(fs, &fs->root, &root_cfg);
    RET_ERR(logfs_createNewFile(fs, &fs->root));

    // Format was successful.
    fs->mounted = true;
    return LOGFS_ERR_OK;
}

LogFsErr logfs_mount(LogFs *fs, const LogFsCfg *cfg)
{
    CHECK_ARG(fs);
    CHECK_ARG(cfg);

    // Shared initialization.
    logfs_init(fs, cfg);

    // Find filesystem head.
    uint32_t  cur_file = LOGFS_ORIGIN;
    uint32_t  cur_head = LOGFS_ORIGIN + LOGFS_PAIR_SIZE;
    LogFsPair cur_file_pair;

    while (true)
    {
        const LogFsErr err = disk_fetchPair(fs, &cur_file_pair, cur_file) | disk_readPair(fs, &cur_file_pair);
        if (err == LOGFS_ERR_CORRUPT && cur_file != LOGFS_ORIGIN)
        {
            // Next file is corrupt, assume it is the end (this will be fixed
            // upon creating a new file).
            break;
        }
        else if (IS_ERR(err))
        {
            // Other read error, return.
            return err;
        }

        if (fs->cache_file->next == LOGFS_INVALID_BLOCK)
        {
            // Reached end of file linked-list, return.
            break;
        }

        cur_file = fs->cache_file->next;
        cur_head = MAX(cur_head, fs->cache_file->head);
    }

    // Successfully found the the head file, update state variables.
    fs->head_file = cur_file;
    fs->head      = cur_head;
    fs->mounted   = true;
    return LOGFS_ERR_OK;
}

LogFsErr logfs_open(LogFs *fs, LogFsFile *file, LogFsFileCfg *cfg)
{
    CHECK_ARG(fs);
    CHECK_ARG(cfg);
    CHECK_ARG(file);
    CHECK_FS_VALID(fs);

    // Initialize file.
    logfs_initFile(fs, file, cfg);

    // Traverse the file blocks as a linked-list. The first file is placed at
    // the origin.
    uint32_t  cur_file = LOGFS_ORIGIN;
    LogFsPair cur_file_pair;

    while (true)
    {
        const LogFsErr err = disk_fetchPair(fs, &cur_file_pair, cur_file) | disk_readPair(fs, &cur_file_pair);
        if (err == LOGFS_ERR_CORRUPT)
        {
            // Next file is corrupt, so create a new file in its place.
            return logfs_createNewFile(fs, file);
        }
        else if (IS_ERR(err))
        {
            // Other read error, return.
            return err;
        }

        if (strcmp(fs->cache_file->path, cfg->path) == 0)
        {
            // File has been found, return.
            memcpy(&file->pair, &cur_file_pair, sizeof(LogFsPair));
            file->head      = fs->cache_file->head;
            file->read_init = false;
            return LOGFS_ERR_OK;
        }

        if (fs->cache_file->next == LOGFS_INVALID_BLOCK)
        {
            // Invalid block, means we've reached the end of the filesystem
            // linked-list without finding the file. So we need to create a new
            // file.
            return logfs_createNewFile(fs, file);
        }
        else
        {
            // Jump to the next file in the list.
            cur_file = fs->cache_file->next;
        }
    }
}

LogFsErr logfs_close(LogFs *fs, LogFsFile *file)
{
    RET_ERR(logfs_sync(fs, file));
    file->cache.block = LOGFS_INVALID_BLOCK;
    return LOGFS_ERR_OK;
}

LogFsErr logfs_sync(LogFs *fs, LogFsFile *file)
{
    return disk_syncCache(fs, &file->cache);
}

LogFsErr logfs_write(LogFs *fs, LogFsFile *file, void *buf, uint32_t size, uint32_t *num_written)
{
    CHECK_ARG(fs);
    CHECK_ARG(file);
    CHECK_ARG(buf);
    CHECK_ARG(num_written);
    CHECK_FS_VALID(fs);

    bool new_block = false; // Whether or not a new data block needs to be created.

    // Read file head block into cache.
    uint32_t cur_data = file->head;

    RET_ERR(disk_changeCache(fs, &file->cache, cur_data, true, true));
    // RET_ERR(disk_read(fs, file->read_cur_data, file->cache_data));

    *num_written = 0;
    while (*num_written < size)
    {
        if (new_block)
        {
            // Writes to new blocks are done at the head.
            cur_data = fs->head;
            RET_ERR(disk_changeCache(fs, &file->cache, cur_data, true, false));
            file->cache_data->prev  = file->head;
            file->cache_data->bytes = 0U;
        }

        // Write new data to disk.
        const uint32_t num_available = MIN(fs->eff_block_size - file->cache_data->bytes, size - *num_written);
        memcpy(&file->cache_data->data + file->cache_data->bytes, ((uint8_t *)buf) + *num_written, num_available);
        file->cache_data->bytes += num_available;
        *num_written += num_available;
        // RET_ERR(disk_write(fs, file->read_cur_data, file->cache_data));

        // After a new block, update state. Update here, because if a previous
        // read/write failed, changing filesystem state
        // prematurely could leave it in a bad state.
        if (new_block)
        {
            // If creating a new block, update the last block to point to the
            // new one. The file's "head block" will still point to the last
            // block.
            RET_ERR(disk_readPair(fs, &file->pair));
            fs->cache_file->head = file->head;
            RET_ERR(disk_writePair(fs, &file->pair));

            // After a successful write, now we can update filesystem state.
            printf("update cur data: %d\n", cur_data);
            file->head = cur_data;
            INC_HEAD(fs, 1);
        }

        // We'll always need a new block if there is any remaining data to be
        // written to disk.
        new_block = true;
    }

    return LOGFS_ERR_OK;
}

LogFsErr logfs_read(LogFs *fs, LogFsFile *file, void *buf, uint32_t size, LogFsRead mode, uint32_t *num_read)
{
    CHECK_ARG(fs);
    CHECK_ARG(file);
    CHECK_ARG(buf);
    CHECK_ARG(num_read);
    CHECK_FS_VALID(fs);

    printf("test1: start: %d\n", file->read_cur_data);

    if (mode == LOGFS_READ_START || !file->read_init)
    {
        // Set read iterator state variables to start of file.
        printf("reset!\n");
        file->read_cur_data = file->head;
        file->read_cur_num  = 0;
        file->read_init     = true;
    }

    RET_ERR(disk_changeCache(fs, &file->cache, file->read_cur_data, true, true));
    // RET_ERR(disk_read(fs, file->read_cur_data, file->cache_data));

    printf("test2: start: %d\n", file->read_cur_data);

    *num_read = 0;
    while (*num_read < size && file->read_cur_data != LOGFS_INVALID_BLOCK)
    {
        printf("test3: %d\n", file->read_cur_data);

        // Read current block.
        RET_ERR(disk_changeCache(fs, &file->cache, file->read_cur_data, false, true));
        // RET_ERR(disk_read(fs, file->read_cur_data, file->cache_data));

        printf("test4\n");

        // Calculate number of available bytes.
        const uint32_t num_in_block     = file->cache_data->bytes - file->read_cur_num;
        const uint32_t num_left_to_read = size - *num_read;
        uint32_t       num_available    = MIN(num_left_to_read, num_in_block);

        printf("test5\n");

        if (num_available == 0)
        {
            // No more data left in this block, reached end of file.
            return LOGFS_ERR_OK;
        }

        // Read out data from cached block.
        uint8_t *const buf_end   = (uint8_t *)buf + size - *num_read;
        uint8_t *const cache_end = &file->cache_data->data + file->cache_data->bytes - file->read_cur_num;
        memcpy(buf_end - num_available, cache_end - num_available, num_available);
        *num_read += num_available;

        // Move to next block if we've read all bytes from the current block.
        file->read_cur_num += num_available;
        if (file->read_cur_num >= file->cache_data->bytes)
        {
            file->read_cur_num  = 0;
            file->read_cur_data = file->cache_data->prev;
            printf("test3: start: %d\n", file->cache_data->prev);
        }
    }

    if (*num_read < size)
    {
        memmove(buf, (uint8_t *)buf + size - *num_read, *num_read);
    }

    return LOGFS_ERR_OK;
}

LogFsErr logfs_firstPath(LogFs *fs, LogFsPath *path)
{
    CHECK_ARG(fs);
    CHECK_ARG(path);
    CHECK_FS_VALID(fs);

    // Try to fetch root file from disk.
    LogFsPair file_pair;
    RET_ERR(disk_fetchPair(fs, &file_pair, LOGFS_ORIGIN));
    RET_ERR(disk_readPair(fs, &file_pair));

    path->file = LOGFS_ORIGIN;
    path->next = fs->cache_file->next;
    strcpy(path->path, fs->cache_file->path);

    return LOGFS_ERR_OK;
}

LogFsErr logfs_nextPath(LogFs *fs, LogFsPath *path)
{
    CHECK_ARG(fs);
    CHECK_ARG(path);
    CHECK_FS_VALID(fs);

    if (path->next == LOGFS_INVALID_BLOCK)
    {
        // Reached end of files.
        return LOGFS_ERR_INVALID_PATH;
    }

    // Read the next file block and retrieve the path.
    LogFsPair file_pair;
    RET_ERR(disk_fetchPair(fs, &file_pair, path->next));
    RET_ERR(disk_readPair(fs, &file_pair));
    path->file = path->next;
    path->next = fs->cache_file->next;
    strcpy(path->path, fs->cache_file->path);
    return LOGFS_ERR_OK;
}