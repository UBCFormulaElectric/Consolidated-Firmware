#pragma once

#include <stdint.h>
#include <stdbool.h>

#define LOGFS_ORIGIN 0                                   // Filesystem starts at address zero
#define LOGFS_COW_SIZE 2                                 // 2 copies for copy-on-write (COW)
#define LOGFS_FIRST_FILE (LOGFS_ORIGIN + LOGFS_COW_SIZE) // First 2 blocks reserved for filesystem info
#define LOGFS_MAX_PATH_LEN 128
#define LOGFS_BLOCK_ADDR_INVALID 0xFFFFFFFF

#define LOGFS_VERSION_MAJOR 0
#define LOGFS_VERSION_MINOR 1

typedef enum
{
    LOGFS_ERR_OK,            // No error
    LOGFS_ERR_UNIMPLEMENTED, // Feature not implemented
    LOGFS_ERR_CORRUPT,       // File system was corrupted
    LOGFS_ERR_DNE,           // File does not exist
} LogFsErr;

typedef struct
{
    // Size of a block, in bytes.
    uint32_t block_size;
    // Number of contiguous blocks available to the filesystem.
    uint32_t block_count;
    // Read a block at a given address.
    LogFsErr (*read)(uint32_t block, void *buf);
    // Program a block at a given address. Will be erased beforehand.
    LogFsErr (*prog)(uint32_t block, void *buf);
    // Erase a block at a given address.
    LogFsErr (*erase)(uint32_t block);

    uint8_t *block_buf;
} LogFsCfg;

typedef struct
{
    // Config info.
    const LogFsCfg *cfg;

    // State info.
    // Last file that was written to.
    uint32_t cur_file;
    uint32_t head;
} LogFs;

typedef struct
{
    uint32_t origin;
    uint32_t head;
    char     path[LOGFS_MAX_PATH_LEN];
} LogFsFile;

// Block types.
typedef struct
{
    uint32_t crc; // First word has to be 32-bit CRC.
    uint8_t  version_major;
    uint8_t  version_minor;
    uint32_t first_file;
} LogFsBlock_FsInfo;

typedef struct
{
    uint32_t crc; // First word has to be 32-bit CRC.
    uint32_t next;
    char     path[LOGFS_MAX_PATH_LEN];
} LogFsBlock_FileInfo;

typedef struct
{
    uint32_t crc; // First word has to be 32-bit CRC.
    uint32_t next;
    uint32_t bytes;
    void    *data;
} LogFsBlock_FileData;

LogFsErr logfs_mount(LogFs *fs, const LogFsCfg *cfg);
LogFsErr logfs_format(LogFs *fs, const LogFsCfg *cfg);
LogFsErr logfs_open(LogFs *fs, LogFsFile *file, const char *path);
LogFsErr logfs_close(LogFs *fs, LogFsFile *file);
uint32_t logfs_read(LogFs *fs, LogFsFile *file, void *buf, uint32_t size);
uint32_t logfs_write(LogFs *fs, LogFsFile *file, void *buf, uint32_t size);

int logfs_add(int a, int b);
