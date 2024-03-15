#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

    // TODO: If you write a piece of data that crosses a block boundary, the
    // second write could fail after the first succeeds. This could be bad,
    // because if you rely on the last N bytes (ex. boot count), it could
    // grab a partially-written byte sequence, and so carry invalid data.

    // TODO 2: Consider splitting logfs_open into a 2 functions, 1 that only
    // searches the disk for existing files, and another that just creates a
    // new file.

#include <stdint.h>
#include <stdbool.h>

#define LOGFS_ORIGIN 0                 // Filesystem starts at address zero
#define LOGFS_INVALID_BLOCK 0xFFFFFFFF // Indicates invalid block
#define LOGFS_PATH_BYTES 128           // Max bytes to allocate for a path
#define LOGFS_PAIR_SIZE 2

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
        LOGFS_ERR_UNIMPLEMENTED, // Feature not implemented
    } LogFsErr;

    typedef enum
    {
        LOGFS_READ_START,
        LOGFS_READ_ITER,
    } LogFsRead;

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
        char *path;
        void *cache;
    } LogFsFileCfg;

    typedef struct
    {
        uint32_t crc;
        uint8_t  seq_num;
        uint32_t next;
        uint32_t head;
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
        uint32_t blocks[LOGFS_PAIR_SIZE];
        uint8_t  seq_num;
        bool     valid;
    } LogFsPair;

    typedef struct
    {
        uint32_t block;
        void    *buf;
    } LogFsCache;

    typedef struct
    {
        const LogFsFileCfg *cfg;
        LogFsCache          cache;
        LogFsBlock_Data    *cache_data;
        LogFsPair           pair;
        uint32_t            head;

        // State for iterating reads.
        uint32_t read_init;
        uint32_t read_cur_num;  // Number of bytes read from current block
        uint32_t read_cur_data; // Current data block being read from
    } LogFsFile;

    typedef struct
    {
        uint32_t file;                   // Block for this file's info
        uint32_t next;                   // Block for next file's info
        char     path[LOGFS_PATH_BYTES]; // Path string
    } LogFsPath;

    typedef struct
    {
        const LogFsCfg *cfg;
        uint32_t        head_file;
        uint32_t        head;

        uint32_t  eff_block_size;
        uint32_t  max_path_len;
        bool      mounted;
        bool      out_of_memory;
        LogFsFile root;

        LogFsBlock_File *cache_file;
        LogFsBlock_Data *cache_data;
    } LogFs;

    LogFsErr logfs_mount(LogFs *fs, const LogFsCfg *cfg);
    LogFsErr logfs_format(LogFs *fs, const LogFsCfg *cfg);

    LogFsErr logfs_open(LogFs *fs, LogFsFile *file, LogFsFileCfg *cfg);
    LogFsErr logfs_close(LogFs *fs, LogFsFile *file);
    LogFsErr logfs_sync(LogFs *fs, LogFsFile *file);

    LogFsErr logfs_write(LogFs *fs, LogFsFile *file, void *buf, uint32_t size, uint32_t *num_written);
    LogFsErr logfs_read(LogFs *fs, LogFsFile *file, void *buf, uint32_t size, LogFsRead mode, uint32_t *num_read);

    LogFsErr logfs_firstPath(LogFs *fs, LogFsPath *path);
    LogFsErr logfs_nextPath(LogFs *fs, LogFsPath *path);

#ifdef __cplusplus
}
#endif