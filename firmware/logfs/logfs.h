#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

    /*

    Make a new "superblock" header that pre-fixes every data section.
    These can be used to identify the bounds of blocks. And should make
    it better to implement O(1) writes.
    Probably needs to be COW.
    Means we need to add a "block ID" param so the filesystem can identify
    which block is which.

    */

    // Block 0: Fs info 1
    // Block 1: Fs info 2
    // Block 2: File info 1
    // Block 3: File info 2
    // Block 4: File header 1
    // Block 5: File header 2
    // Block 6: Data

#include <stdint.h>
#include <stdbool.h>

#define LOGFS_ORIGIN 0 // Filesystem starts at address zero
#define LOGFS_FIRST_FILE 2
#define LOGFS_COW_SIZE 2      // 2 copies for copy-on-write (COW)
#define LOGFS_MAX_PATH_LEN 24 // TODO: Pick a better value for this!
#define LOGFS_BLOCK_ADDR_INVALID 0xFFFFFFFF
#define LOGFS_BLOCK_ADDR_NEXT 0

#define LOGFS_VERSION_MAJOR 0
#define LOGFS_VERSION_MINOR 1

    typedef enum
    {
        LOGFS_ERR_OK,            // No error
        LOGFS_ERR_UNIMPLEMENTED, // Feature not implemented
        LOGFS_ERR_CORRUPT,       // File system was corrupted
        LOGFS_ERR_DNE,           // File does not exist
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
        // Cache for reading blocks (must be block_size bytes in length).
        void *block_cache;
    } LogFsCfg;

    typedef struct
    {
        uint32_t info_block;
        uint32_t head;
        uint32_t last_hdr;
    } LogFsFile;

    // Block types.
    typedef struct
    {
        uint32_t crc; // First word has to be 32-bit CRC.
        uint8_t  version_major;
        uint8_t  version_minor;
    } LogFsBlock_FsInfo;

    typedef struct
    {
        uint32_t crc;                      // First word has to be 32-bit CRC.
        uint32_t next;                     // Next file info block.
        char     path[LOGFS_MAX_PATH_LEN]; // Path to file.
    } LogFsBlock_FileInfo;

    typedef struct
    {
        uint32_t crc;  // First word has to be 32-bit CRC.
        uint32_t next; // Address of next data header for this file.
    } LogFsBlock_DataHeader;

    typedef struct
    {
        uint32_t crc;   // First word has to be 32-bit CRC.
        uint32_t bytes; // Number of bytes in this file.
        uint8_t  data;  // Used to get address of first data element.
    } LogFsBlock_Data;

    typedef struct
    {
        // Config info.
        const LogFsCfg *cfg;

        // State info.
        uint32_t head;
        uint32_t eff_block_size;
        bool     empty;

        // Block-specific pointers to the cache.
        LogFsBlock_FsInfo     *cache_fs_info;
        LogFsBlock_FileInfo   *cache_file_info;
        LogFsBlock_DataHeader *cache_data_hdr;
        LogFsBlock_Data       *cache_data;

    } LogFs;

    LogFsErr logfs_mount(LogFs *fs, const LogFsCfg *cfg);
    LogFsErr logfs_format(LogFs *fs, const LogFsCfg *cfg);
    LogFsErr logfs_open(LogFs *fs, LogFsFile *file, const char *path);
    LogFsErr logfs_close(LogFs *fs, LogFsFile *file);
    uint32_t logfs_read(LogFs *fs, LogFsFile *file, void *buf, uint32_t size);
    uint32_t logfs_write(LogFs *fs, LogFsFile *file, void *buf, uint32_t size);

#ifdef __cplusplus
}
#endif
