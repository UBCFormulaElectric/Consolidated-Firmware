#include "logfs.h"
#include <string.h>
#include <stdio.h>
#include "disk.h"
#include "utils.h"
#include <assert.h>

inline static void logfs_init(LogFs *fs, const LogFsCfg *cfg)
{
    assert(cfg->block_size > sizeof(LogFsBlock_File));
    assert(cfg->block_size > sizeof(LogFsBlock_Metadata));
    assert(cfg->block_size > sizeof(LogFsBlock_Data));

    fs->cfg                                 = cfg;
    const uint32_t data_block_metadata_size = sizeof(LogFsBlock_Data) - 1;
    fs->eff_block_size_bytes                = cfg->block_size - data_block_metadata_size;
    fs->mounted                             = false;
    fs->out_of_memory                       = false;
    fs->head_file_addr                      = LOGFS_INVALID_BLOCK;
    fs->head_addr                           = 0;

    fs->max_path_len_bytes = MIN((int)((int)fs->cfg->block_size - sizeof(LogFsBlock_File)), LOGFS_PATH_BYTES);
    assert(fs->max_path_len_bytes > 0);

    fs->cache_pair_hdr = fs->cfg->cache;
    fs->cache_file     = fs->cfg->cache;
    fs->cache_metadata = fs->cfg->cache;
    fs->cache_data     = fs->cfg->cache;
}

inline static void logfs_initFile(LogFsFile *file, const LogFsFileCfg *cfg, LogFsOpenFlags flags)
{
    file->cache.cached_addr = LOGFS_INVALID_BLOCK;
    file->cache.buf         = cfg->cache;
    file->cache_data        = (LogFsBlock_Data *)cfg->cache;
    file->is_open           = false;
    file->flags             = flags;
    strcpy(file->path, cfg->path);
}

static LogFsErr logfs_createNewFile(LogFs *fs, LogFsFile *file, LogFsFileCfg *cfg)
{
    CHECK_ARG(fs);
    CHECK_ARG(file);

    const uint32_t new_file_block     = fs->head_addr;
    const uint32_t new_metadata_block = new_file_block + LOGFS_PAIR_SIZE;
    const uint32_t new_data_block     = new_metadata_block + LOGFS_PAIR_SIZE;

    // Write file pair to disk.
    fs->cache_file->head_data_addr = new_data_block;
    fs->cache_file->prev_head_addr = LOGFS_INVALID_BLOCK;
    fs->cache_file->metadata_addr  = new_metadata_block;
    fs->cache_file->next_file_addr = LOGFS_INVALID_BLOCK;
    strcpy(fs->cache_file->path, cfg->path);
    disk_newPair(&file->file_pair, new_file_block);
    RET_ERR(disk_writePair(fs, &file->file_pair, false));

    // Write metadata pair to disk.
    fs->cache_metadata->num_bytes = 0;
    disk_newPair(&file->metadata_pair, new_metadata_block);
    RET_ERR(disk_writePair(fs, &file->metadata_pair, false));

    // Write empty data block to disk.
    file->cache_data->num_bytes      = 0;
    file->cache_data->prev_data_addr = LOGFS_INVALID_BLOCK;
    file->cache.cached_addr          = new_data_block;
    RET_ERR(disk_syncCache(fs, &file->cache));

    if (fs->head_file_addr != LOGFS_INVALID_BLOCK)
    {
        // Update previous file to point to the new one.
        LogFsPair prev_file_pair;
        RET_ERR(disk_fetchPair(fs, &prev_file_pair, fs->head_file_addr));
        RET_ERR(disk_readPair(fs, &prev_file_pair));
        fs->cache_file->next_file_addr = new_file_block;
        RET_ERR(disk_writePair(fs, &prev_file_pair, false));
    }

    // Update filesystem state variables (do this last so if previous writes
    // fail, the filesystem won't be left in an invalid state).
    file->head_data_addr = new_data_block;
    file->is_open        = true;
    file->read_iter_init = false;
    fs->head_file_addr   = new_file_block;
    INC_HEAD(fs, 2 * LOGFS_PAIR_SIZE + 1);
    return LOGFS_ERR_OK;
}

LogFsErr logfs_format(LogFs *fs, const LogFsCfg *cfg)
{
    CHECK_ARG(fs);
    CHECK_ARG(cfg);

    // Shared initialization.
    logfs_init(fs, cfg);

    if (fs->cfg->rd_only)
    {
        return LOGFS_ERR_RD_ONLY;
    }

    // Create root file.
    fs->head_addr = LOGFS_ORIGIN;

    // Initialize file cache.
    LogFsFileCfg root_cfg = { .cache = cfg->cache, .path = "/.root" };
    logfs_initFile(&fs->root_file, &root_cfg, LOGFS_OPEN_RD_ONLY | LOGFS_OPEN_CREATE);
    return logfs_createNewFile(fs, &fs->root_file, &root_cfg);
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
        // Read file.
        LogFsErr err = disk_fetchPair(fs, &cur_file_pair, cur_file);
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

        RET_ERR(disk_readPair(fs, &cur_file_pair));
        cur_head = MAX(cur_head, fs->cache_file->head_data_addr + 1);

        // Address of first item in pair, so need to add 2 to get first unused block.
        cur_head = MAX(cur_head, fs->cache_file->head_data_addr + 2);
        cur_head = MAX(cur_head, fs->cache_file->metadata_addr + 2);

        if (fs->cache_file->next_file_addr == LOGFS_INVALID_BLOCK)
        {
            // Reached end of file linked-list, return.
            break;
        }
        else
        {
            cur_file = fs->cache_file->next_file_addr;
        }
    }

    // Successfully found the the head file, update state variables.
    fs->head_file_addr = cur_file;
    fs->head_addr      = cur_head;
    fs->mounted        = true;
    return LOGFS_ERR_OK;
}

LogFsErr logfs_open(LogFs *fs, LogFsFile *file, LogFsFileCfg *cfg, uint32_t flags)
{
    CHECK_ARG(fs);
    CHECK_ARG(file);
    CHECK_ARG(cfg);
    CHECK_PATH(cfg->path);
    CHECK_FS(fs);

    // Initialize file.
    logfs_initFile(file, cfg, flags);

    // Traverse the file blocks as a linked-list. The first file is placed at
    // the origin.
    uint32_t  cur_file = LOGFS_ORIGIN;
    LogFsPair cur_file_pair;

    while (true)
    {
        const LogFsErr err = disk_fetchPair(fs, &cur_file_pair, cur_file);
        if (err == LOGFS_ERR_CORRUPT)
        {
            // Next file is corrupt (can create a new one in its place).
            break;
        }
        else if (IS_ERR(err))
        {
            // Other read error, return.
            return err;
        }

        // Fetch file from disk.
        RET_ERR(disk_readPair(fs, &cur_file_pair));
        if (strcmp(fs->cache_file->path, cfg->path) == 0)
        {
            // File name matches, find data head address.
            file->head_data_addr = fs->cache_file->head_data_addr;
            if (IS_ERR(disk_exchangeCache(fs, &file->cache, fs->cache_file->head_data_addr, false, true)))
            {
                file->head_data_addr = fs->cache_file->prev_head_addr;
                if (fs->cache_file->prev_head_addr == LOGFS_INVALID_BLOCK ||
                    IS_ERR(disk_exchangeCache(fs, &file->cache, fs->cache_file->prev_head_addr, false, true)))
                {
                    // If both the head and backup are corrupt, file can't be opened.
                    return LOGFS_ERR_CORRUPT;
                }
            }

            // Attempt to fetch metadata pair (just checking that its valid).
            RET_ERR(disk_fetchPair(fs, &file->metadata_pair, fs->cache_file->metadata_addr));

            // File has found and opened successfully.
            file->read_iter_init = false;
            memcpy(&file->file_pair, &cur_file_pair, sizeof(LogFsPair));
            file->is_open = true;
            return LOGFS_ERR_OK;
        }

        if (fs->cache_file->next_file_addr == LOGFS_INVALID_BLOCK)
        {
            // Invalid block, means we've reached the end of the filesystem
            // linked-list without finding the file.
            break;
        }
        else
        {
            // Jump to the next file in the list.
            cur_file = fs->cache_file->next_file_addr;
        }
    }

    if (!(file->flags & LOGFS_OPEN_CREATE))
    {
        // File doesn't exist.
        return LOGFS_ERR_DNE;
    }

    if (fs->cfg->rd_only)
    {
        // Filesystem is read-only, can't create a new file.
        return LOGFS_ERR_RD_ONLY;
    }

    return logfs_createNewFile(fs, file, cfg);
}

LogFsErr logfs_close(LogFs *fs, LogFsFile *file)
{
    CHECK_ARG(fs);
    CHECK_ARG(file);
    CHECK_FS(fs);
    CHECK_FILE(file);

    if (!fs->cfg->rd_only && file->flags & LOGFS_OPEN_WR_ONLY)
    {
        // Only write to disk if file isn't read-only.
        RET_ERR(logfs_sync(fs, file));
    }

    file->is_open = false;
    return LOGFS_ERR_OK;
}

LogFsErr logfs_sync(LogFs *fs, LogFsFile *file)
{
    CHECK_ARG(fs);
    CHECK_ARG(file);
    CHECK_FS(fs);
    CHECK_FILE(file);

    if (fs->cfg->rd_only || !(file->flags & LOGFS_OPEN_WR_ONLY))
    {
        // File is read only, fail.
        return LOGFS_ERR_RD_ONLY;
    }

    return disk_syncCache(fs, &file->cache);
}

LogFsErr logfs_write(LogFs *fs, LogFsFile *file, void *buf, uint32_t size)
{
    CHECK_ARG(fs);
    CHECK_ARG(file);
    CHECK_ARG(buf);
    CHECK_FS(fs);
    CHECK_FILE(file);

    if (fs->cfg->rd_only || !(file->flags & LOGFS_OPEN_WR_ONLY))
    {
        // File is read only, fail.
        return LOGFS_ERR_RD_ONLY;
    }

    bool new_block = false; // Whether or not a new data block needs to be created.

    // Read file head block into cache.
    uint32_t cur_data = file->head_data_addr;
    RET_ERR(disk_exchangeCache(fs, &file->cache, cur_data, true, true));

    uint32_t num_written = 0;
    while (num_written < size)
    {
        if (new_block)
        {
            // Writes to new blocks are done at the head.
            cur_data = fs->head_addr;
            RET_ERR(disk_exchangeCache(fs, &file->cache, cur_data, true, false));
            file->cache_data->prev_data_addr = file->head_data_addr;
            file->cache_data->num_bytes      = 0U;
            file->head_data_addr             = cur_data;
            RET_ERR(disk_syncCache(fs, &file->cache));
            INC_HEAD(fs, 1);
        }

        // Write new data to disk.
        const uint32_t num_available = MIN(fs->eff_block_size_bytes - file->cache_data->num_bytes, size - num_written);
        memcpy(&file->cache_data->data + file->cache_data->num_bytes, ((uint8_t *)buf) + num_written, num_available);
        file->cache_data->num_bytes += num_available;
        num_written += num_available;

        // After a new block, update state. Update here, because if a previous
        // read/write failed, changing filesystem state
        // prematurely could leave it in a bad state.
        if (new_block)
        {
            // If creating a new block, update the last block to point to the
            // new one. The file's "head block" will still point to the last
            // block.
            RET_ERR(disk_readPair(fs, &file->file_pair));
            fs->cache_file->prev_head_addr = fs->cache_file->head_data_addr;
            fs->cache_file->head_data_addr = file->head_data_addr;
            RET_ERR(disk_writePair(fs, &file->file_pair, false));
        }

        // We'll always need a new block if there is any remaining data to be
        // written to disk.
        new_block = true;
    }

    return LOGFS_ERR_OK;
}

LogFsErr logfs_read(LogFs *fs, LogFsFile *file, void *buf, uint32_t size, LogFsReadFlags flags, uint32_t *num_read)
{
    CHECK_ARG(fs);
    CHECK_ARG(file);
    CHECK_ARG(buf);
    CHECK_ARG(num_read);
    CHECK_FS(fs);
    CHECK_FILE(file);

    if (!(file->flags & LOGFS_OPEN_RD_ONLY))
    {
        // File is write only, fail.
        return LOGFS_ERR_WR_ONLY;
    }

    if (flags == LOGFS_READ_END || !file->read_iter_init)
    {
        // Set read iterator state variables to start of file.
        file->read_iter_data_addr = file->head_data_addr;
        file->read_iter_data_byte = 0;
        file->read_iter_init      = true;
    }

    // Sync whatever is currently in the cache.
    if (!fs->cfg->rd_only)
    {
        // TODO: Check if this data is from a read or a write. If it's from a write, we need to sync it, but if its from
        // a read, we don't!
        RET_ERR(disk_syncCache(fs, &file->cache));
    }

    *num_read = 0;
    while (*num_read < size && file->read_iter_data_addr != LOGFS_INVALID_BLOCK)
    {
        // Read current block.
        RET_ERR(disk_exchangeCache(fs, &file->cache, file->read_iter_data_addr, false, true));

        // Calculate number of available bytes.
        const uint32_t num_in_block     = file->cache_data->num_bytes - file->read_iter_data_byte;
        const uint32_t num_left_to_read = size - *num_read;
        uint32_t       num_available    = MIN(num_left_to_read, num_in_block);

        // Read out data from cached block.
        uint8_t *const buf_end   = (uint8_t *)buf + size - *num_read;
        uint8_t *const cache_end = &file->cache_data->data + file->cache_data->num_bytes - file->read_iter_data_byte;
        memcpy(buf_end - num_available, cache_end - num_available, num_available);
        *num_read += num_available;

        // Move to next block if we've read all bytes from the current block.
        file->read_iter_data_byte += num_available;

        if (file->read_iter_data_byte >= file->cache_data->num_bytes)
        {
            file->read_iter_data_byte = 0;
            file->read_iter_data_addr = file->cache_data->prev_data_addr;
        }
    }

    if (*num_read < size)
    {
        // Re-align read data if buffer is only partially full.
        memmove(buf, (uint8_t *)buf + size - *num_read, *num_read);
    }

    return LOGFS_ERR_OK;
}

LogFsErr logfs_writeMetadata(LogFs *fs, LogFsFile *file, void *buf, uint32_t size)
{
    CHECK_ARG(fs);
    CHECK_ARG(file);
    CHECK_ARG(buf);
    CHECK_FS(fs);
    CHECK_FILE(file);

    if (fs->cfg->rd_only || !(file->flags & LOGFS_OPEN_WR_ONLY))
    {
        // File is read only, fail.
        return LOGFS_ERR_RD_ONLY;
    }

    if (size > fs->eff_block_size_bytes)
    {
        // We only allocate a single block to metadata.
        return LOGFS_ERR_NOMEM;
    }

    RET_ERR(disk_readPair(fs, &file->metadata_pair));
    memcpy(&fs->cache_metadata->data, buf, size);
    fs->cache_metadata->num_bytes = size;
    RET_ERR(disk_writePair(fs, &file->metadata_pair, true));
    return LOGFS_ERR_OK;
}

LogFsErr logfs_readMetadata(LogFs *fs, LogFsFile *file, void *buf, uint32_t size, uint32_t *num_read)
{
    CHECK_ARG(fs);
    CHECK_ARG(file);
    CHECK_ARG(buf);
    CHECK_FS(fs);
    CHECK_FILE(file);

    if (!(file->flags & LOGFS_OPEN_RD_ONLY))
    {
        // File is write only, fail.
        return LOGFS_ERR_WR_ONLY;
    }

    RET_ERR(disk_readPair(fs, &file->metadata_pair));
    size = MIN(size, fs->cache_metadata->num_bytes);
    memcpy(buf, &fs->cache_metadata->data, size);
    *num_read = fs->cache_metadata->num_bytes;
    return LOGFS_ERR_OK;
}

LogFsErr logfs_firstPath(LogFs *fs, LogFsPath *path)
{
    CHECK_ARG(fs);
    CHECK_ARG(path);
    CHECK_FS(fs);

    // Try to fetch root file from disk.
    LogFsPair file_pair;
    RET_ERR(disk_fetchPair(fs, &file_pair, LOGFS_ORIGIN));
    RET_ERR(disk_readPair(fs, &file_pair));

    path->file_addr      = LOGFS_ORIGIN;
    path->next_file_addr = fs->cache_file->next_file_addr;
    strcpy(path->path, fs->cache_file->path);
    return LOGFS_ERR_OK;
}

LogFsErr logfs_nextPath(LogFs *fs, LogFsPath *path)
{
    CHECK_ARG(fs);
    CHECK_ARG(path);
    CHECK_FS(fs);

    if (path->next_file_addr == LOGFS_INVALID_BLOCK)
    {
        // Reached end of files.
        return LOGFS_ERR_NO_MORE_FILES;
    }

    // Read the next file block and retrieve the path.
    LogFsPair file_pair;
    RET_ERR(disk_fetchPair(fs, &file_pair, path->next_file_addr));
    RET_ERR(disk_readPair(fs, &file_pair));

    path->file_addr      = path->next_file_addr;
    path->next_file_addr = fs->cache_file->next_file_addr;
    strcpy(path->path, fs->cache_file->path);
    return LOGFS_ERR_OK;
}
