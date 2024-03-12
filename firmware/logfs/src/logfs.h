#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stdbool.h>

#define LOGFS_ORIGIN 0                 // Filesystem starts at address zero
#define LOGFS_INVALID_BLOCK 0xFFFFFFFF // Indicates invalid block
#define LOGFS_PATH_BYTES 128           // Max bytes to allocate for a path

    typedef enum
    {
        LOGFS_ERR_OK,            // No error
        LOGFS_ERR_IO,            // Error during I/O operation
        LOGFS_ERR_CORRUPT,       // File system was corrupted (bad CRC)
        LOGFS_ERR_INVALID_ARG,   // Invalid argument
        LOGFS_ERR_INVALID_PATH,  // Invalid path
        LOGFS_ERR_INVALID_BLOCK, // Requested an operator on an invalid block
        LOGFS_ERR_UNMOUNTED,     // Filesystem hasn't been successfully mounted
        LOGFS_ERR_NOMEM,         // Filesystem is full (no more memory)
        LOGFS_ERR_EMPTY,         // Filesystem is empty
        LOGFS_ERR_UNIMPLEMENTED, // Feature not implemented
    } LogFsErr;

    typedef struct _LogFsCfg
    {
        void *context;
        // Size of a block, in bytes.
        uint32_t block_size;
        // Number of contiguous blocks available to the filesystem.
        uint32_t block_count;
        // Read a block at a given address.
        LogFsErr (*read)(const struct _LogFsCfg *cfg, uint32_t block, void *buf);
        // Program a block at a given address. Will be erased beforehand.
        LogFsErr (*write)(const struct _LogFsCfg *cfg, uint32_t block, void *buf);
        // 1-block sized cache.
        void *cache;
    } LogFsCfg;

    typedef struct
    {
        uint32_t crc;
        uint32_t next;
        uint32_t prev;
        char     path[LOGFS_PATH_BYTES];
    } LogFsBlock_File;

    typedef struct
    {
        uint32_t crc;
        uint32_t prev;
        uint32_t bytes;
        uint8_t  data;
    } LogFsBlock_Data;

    typedef struct
    {
        bool     init;
        uint8_t  id;
        uint32_t blocks[2];
        uint8_t  active;
    } LogFsPair;

    typedef struct
    {
        uint32_t file;
        uint32_t head;
    } LogFsFile;

    typedef struct
    {
        const LogFsCfg *cfg;
        uint32_t        head_file;
        uint32_t        head;

        uint32_t eff_block_size;
        uint32_t max_path_len;
        bool     mounted;
        bool     out_of_memory;
    } LogFs;

    LogFsErr logfs_mount(LogFs *fs, const LogFsCfg *cfg);
    LogFsErr logfs_format(LogFs *fs, const LogFsCfg *cfg);

    LogFsErr logfs_open(LogFs *fs, LogFsFile *file, const char *path);
    LogFsErr logfs_read(LogFs *fs, LogFsFile *file, void *buf, uint32_t size, uint32_t *num_read);
    LogFsErr logfs_write(LogFs *fs, LogFsFile *file, void *buf, uint32_t size, uint32_t *num_written);

#ifdef __cplusplus
}
#endif