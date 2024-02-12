#include "logfs.h"
#include "logfs_util.h"
#include <string.h>
#include <assert.h>

static void inline logfs_writeBlock(const LogFs *fs, uint32_t block, const void *buf)
{
    fs->cfg->erase(block);
    fs->cfg->prog(block, buf);
}

static void logfs_writeCowBlock(const LogFs *fs, uint32_t block, const void *buf)
{
    // Power-loss resilience for critical blocks is maintained by keeping a
    // copy. In the event of a failed transaction (due to power-loss or other),
    // the data can be retrieved from the copy. Which one is invalid is
    // determined via calculating a checksum.
    logfs_util_stampBlockCrc(fs, buf);

    uint32_t block_primary   = block;
    uint32_t block_secondary = block + 1;

    // Write the primary block first.
    fs->cfg->erase(block_primary);
    fs->cfg->prog(block_primary, buf);

    // Write the second block next.
    fs->cfg->erase(block_secondary);
    fs->cfg->prog(block_secondary, buf);
}

static void logfs_readCowBlock(const LogFs *fs, uint32_t block, void *buf)
{
    uint32_t block_primary   = block;
    uint32_t block_secondary = block + 1;
    uint8_t  buf_primary[fs->cfg->block_size];
    uint8_t  buf_secondary[fs->cfg->block_size];

    fs->cfg->read(block_primary, buf_primary);
    fs->cfg->read(buf_secondary, buf_secondary);

    const bool primary_valid   = logfs_util_checkBlockCrc(fs, buf);
    const bool secondary_valid = logfs_util_checkBlockCrc(fs, buf);

    if (primary_valid && secondary_valid)
    {
        // Primary and secondary copies are both valid.
        memcpy(buf, buf_primary, fs->cfg->block_size);
    }
    else if (primary_valid)
    {
        // Only primary block is valid, fix secondary.
        memcpy(buf, buf_primary, fs->cfg->block_size);
        logfs_writeBlock(fs, block_secondary, buf_primary);
    }
    else if (secondary_valid)
    {
        // Only secondary block is valid, fix primary.
        memcpy(buf, buf_secondary, fs->cfg->block_size);
        logfs_writeBlock(fs, block_primary, buf_secondary);
    }
    else
    {
        // Neither are valid, filesystem is corrupt!
        assert(false);
    }
}

LogFsErr logfs_mount(LogFs *fs, const LogFsCfg *cfg)
{

}

LogFsErr logfs_format(LogFs *fs, const LogFsCfg *cfg)
{
    LogFsBlockFileInfo info;
    info.head = LOGFS_BLOCK_ADDR_INVALID;
    info.

    fs->head = LOGFS_ORIGIN;
}

void logfs_open(LogFs *fs, LogFsFile *file, const char *path)
{
    uint32_t            block = LOGFS_ORIGIN;
    uint8_t             buf[fs->cfg->block_size];
    LogFsBlockFileInfo *info = buf;
    
    // Copy the file path to the file struct.
    strcpy(file->path, path);

    while (true)
    {
        logfs_readCowBlock(block, buf, block);
        if (strcmp(info->path, path))
        {
            // File has been found, return.
            file->head = info->head;
            return;
        }

        if (info->next == LOGFS_BLOCK_ADDR_INVALID)
        {
            // File does not exist on disk, must be created.
            break;
        }
        else
        {
            // Search the next file on the disk.
            block = info->next;
        }
    }

    // Mark the head pointer as invalid - the file info block won't be created
    // until a write is made to the file.
    file->head = LOGFS_BLOCK_ADDR_INVALID;
}

void logfs_close(LogFs *fs, LogFsFile *file)
{
    // No-op - this doesn't need to do anything since we currently don't cache 
    // anything. So anything that needs to be written to disk already has been.
}

uint32_t logfs_read(LogFs *fs, LogFsFile *file, void *buf, uint32_t size)
{
    // Each block contains a pointer to the previous block in the file. To read
    // a file, we start from the most recently appended block (the head) and 
    // traverse backwards.

    uint32_t read_count = 0;
    uint8_t read_buf[fs->cfg->block_size];
    LogFsBlockData* read_data = read_buf;
    uint32_t block = file->head;

    while(read_count < size)
    {
        // Read current block.
        fs->cfg->read(block, read_buf);
        
        // Write out the maximum number of bytes available.
        uint32_t bytes_to_transfer = LOGFS_UTIL_MIN(read_data->size, size - read_count);
        memcpy(buf, &read_data->data_start, bytes_to_transfer);
        read_count += bytes_to_transfer;
        
        // An invalid prev block identifier means we've reached the end of the file.
        if(read_data->prev == LOGFS_BLOCK_ADDR_INVALID)
        {
            return read_count;
        }
        else
        {
            block = read_data->prev;
        }
    }

    return read_count;
}

LogFsErr logfs_write(LogFs *fs, LogFsFile *file, void *buf, uint32_t size)
{
    
}
