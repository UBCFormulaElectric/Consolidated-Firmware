#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stdbool.h>

#define LOGFS_ORIGIN 0        // Filesystem starts at address zero
#define LOGFS_COW_SIZE 2      // 2 copies for copy-on-write (COW)
#define LOGFS_MAX_PATH_LEN 24 // TODO: Pick a better value for this!
#define LOGFS_INVALID_BLOCK 0xFFFFFFFF

#define LOGFS_VERSION_MAJOR 1
#define LOGFS_VERSION_MINOR 0

    typedef enum
    {
        LOGFS_ERR_OK,            // No error
        LOGFS_ERR_IO,            // Error during I/O operation
        LOGFS_ERR_CORRUPT,       // File system was corrupted (bad CRC)
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
        LogFsErr (*prog)(const struct _LogFsCfg *cfg, uint32_t block, void *buf);
        // Erase a block at a given address.
        LogFsErr (*erase)(const struct _LogFsCfg *cfg, uint32_t block);
        // Erase the entire filesystem image.
        LogFsErr (*mass_erase)(const struct _LogFsCfg *cfg);
        // Cache for reading blocks (must be block_size bytes in length).
        void *block_cache;
    } LogFsCfg;

    typedef enum
    {
        LOGFS_READ_START,
        LOGFS_READ_ITER,
    } LogFsRead;

    typedef enum
    {
        LOGFS_WRITE_APPEND,
        LOGFS_WRITE_EDIT,
    } LogFsWrite;

    typedef struct
    {
        uint32_t info_block;
        uint32_t last_hdr;
        uint32_t head;

        // State for iterating reads.
        uint32_t read_uninit;
        uint32_t read_cur_byte; // Current byte to read from
        uint32_t read_cur_data; // Current data block being read from.
        uint32_t read_cur_hdr;  // Current header for chunk being read from.
    } LogFsFile;

    // Block types.
    typedef enum
    {
        LOGFS_BLOCK_FS_INFO,
        LOGFS_BLOCK_FILE_INFO,
        LOGFS_BLOCK_DATA_HDR,
        LOGFS_BLOCK_DATA,
    } LogFsBlockType;

    typedef struct
    {
        uint32_t crc;  // First word has to be 32-bit CRC.
        uint8_t  type; // Next byte needs to be block type.
        uint8_t  version_major;
        uint8_t  version_minor;
    } LogFsBlock_FsInfo;

    typedef struct
    {
        uint32_t crc;                      // First word has to be 32-bit CRC.
        uint8_t  type;                     // Next byte needs to be block type.
        uint32_t next;                     // Next file info block.
        char     path[LOGFS_MAX_PATH_LEN]; // Path to file.
    } LogFsBlock_FileInfo;

    typedef struct
    {
        uint32_t crc;  // First word has to be 32-bit CRC.
        uint8_t  type; // Next byte needs to be block type.
        uint32_t next; // Address of next data header for this file.
    } LogFsBlock_DataHeader;

    typedef struct
    {
        uint32_t crc;   // First word has to be 32-bit CRC.
        uint8_t  type;  // Next byte needs to be block type.
        uint32_t bytes; // Number of bytes in this file.
        uint8_t  data;  // Used to get address of first data element.
    } LogFsBlock_Data;

    typedef struct
    {
        const LogFsCfg *cfg; // Config info.
        uint32_t        eff_block_size;
        uint32_t        head;
        bool            empty;

        // Block-specific pointers to the cache.
        LogFsBlock_FsInfo     *cache_fs_info;
        LogFsBlock_FileInfo   *cache_file_info;
        LogFsBlock_DataHeader *cache_data_hdr;
        LogFsBlock_Data       *cache_data;

    } LogFs;

    LogFsErr logfs_mount(LogFs *fs, const LogFsCfg *cfg);
    LogFsErr logfs_format(LogFs *fs, const LogFsCfg *cfg);
    LogFsErr logfs_open(LogFs *fs, LogFsFile *file, const char *path);
    uint32_t logfs_read(LogFs *fs, LogFsFile *file, void *buf, uint32_t size, LogFsRead mode);
    uint32_t logfs_write(LogFs *fs, LogFsFile *file, void *buf, uint32_t size, LogFsWrite mode);

#ifdef __cplusplus
}
#endif
