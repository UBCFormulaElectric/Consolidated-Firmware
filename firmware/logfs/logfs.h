#pragma once

#include <stdint.h>
#include <stdbool.h>

#define LOGFS_BLOCK_ADDR_INVALID 0xFFFFFFFF
#define LOGFS_MAX_PATH_LEN 128
#define LOGFS_ORIGIN 0

typedef enum
{
    LOGFS_ERR_OK,            // No error
    LOGFS_ERR_UNIMPLEMENTED, // Feature not implemented
    LOGFS_ERR_CORRUPT      // Feature not implemented
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
} LogFsCfg;

typedef struct
{
    uint32_t crc;
} LogFsBlock;

typedef struct
{
    const LogFsCfg *cfg;
    uint32_t        head;
} LogFs;

typedef struct
{
    uint32_t head;
    char     path[LOGFS_MAX_PATH_LEN];
} LogFsFile;

typedef struct
{
    char     path[LOGFS_MAX_PATH_LEN];
    uint32_t next;
    uint32_t head;
} LogFsBlockFileInfo;

typedef struct
{
    uint32_t prev;
    uint32_t size;
    uint8_t data_start;
} LogFsBlockData;

LogFsErr logfs_mount(LogFs *fs, const LogFsCfg *cfg);
LogFsErr logfs_format(LogFs *fs, const LogFsCfg *cfg);
void logfs_open(LogFs *fs, LogFsFile *file, const char *path);
void logfs_close(LogFs *fs, LogFsFile *file);
uint32_t logfs_read(LogFs *fs, LogFsFile *file, void *buf, uint32_t size);
LogFsErr logfs_write(LogFs *fs, LogFsFile *file, void *buf, uint32_t size);
