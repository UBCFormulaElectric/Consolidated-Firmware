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
#define LOGFS_PAIR_SIZE 2

    /* Enum definitions */

    typedef enum
    {
        LOGFS_ERR_OK            = 0,   // No error
        LOGFS_ERR_IO            = -2,  // Error during I/O operation
        LOGFS_ERR_CORRUPT       = -3,  // File system was corrupted (bad CRC)
        LOGFS_ERR_INVALID_ARG   = -4,  // Invalid argument
        LOGFS_ERR_INVALID_PATH  = -5,  // Invalid path
        LOGFS_ERR_UNMOUNTED     = -6,  // Filesystem hasn't been successfully mounted
        LOGFS_ERR_NOMEM         = -7,  // Filesystem is full (no more memory)
        LOGFS_ERR_NOT_OPEN      = -8,  // File hasn't been opened
        LOGFS_ERR_RD_ONLY       = -9,  // File is read only, and a write was attempted
        LOGFS_ERR_WR_ONLY       = -10, // File is write only, and a read was attempted
        LOGFS_ERR_DNE           = -11, // File does not exist
        LOGFS_ERR_NO_MORE_FILES = -12, // Traversed all files on the filesystem
    } LogFsErr;

    typedef enum
    {
        LOGFS_OPEN_RD_ONLY = 0x01,
        LOGFS_OPEN_WR_ONLY = 0x02,
        LOGFS_OPEN_RD_WR   = 0x03,
        LOGFS_OPEN_CREATE  = 0x10,
    } LogFsOpenFlags;

    typedef enum
    {
        LOGFS_READ_END,  // Read from end of file
        LOGFS_READ_ITER, // Read next N bytes of file
    } LogFsReadFlags;

    /* Config structs */

    typedef struct _LogFsCfg
    {
        // User-provided context used to send extra data to the I/O functions.
        void *context;
        // Size of a block, in bytes.
        uint32_t block_size;
        // Number of contiguous blocks available to the filesystem.
        uint32_t block_count;
        // Read a block at a given address.
        LogFsErr (*read)(const struct _LogFsCfg *cfg, uint32_t block, void *buf);
        // Program a block at a given address. Will be erased beforehand.
        LogFsErr (*write)(const struct _LogFsCfg *cfg, uint32_t block, void *buf);
        // Pointer to cache buffer (must be 1 block in size).
        void *cache;
        // Number of write cycles before blocks are evicted and replaced.
        uint32_t write_cycles;
        // If the entire filesystem should be marked as read-only.
        bool rd_only;
    } LogFsCfg;

    typedef struct
    {
        const char *path;  // File path string
        void       *cache; // Pointer to cache buffer (must be 1 block in size)
    } LogFsFileCfg;

    /* Block definitions */

    typedef struct
    {
        uint32_t crc;              // Checksum must be first word in block
        uint8_t  seq_num;          // Sequence number (used to calculate most recent version of the pair)
        uint32_t write_cycles;     // Number of times this pair has been written to
        uint32_t replacement_addr; // Replacement address if evicted (invalid address if this block is still valid)
    } LogFsPairHeader;

    typedef struct
    {
        LogFsPairHeader pair_hdr;       // Pair data (pairs guarantee power loss resilience, must be first)
        uint32_t        next_file_addr; // Address of the next file block
        uint32_t        metadata_addr;  // Address of the file's metadata block
        uint32_t        head_data_addr; // Address of file's newest data block
        uint32_t        prev_head_addr; // Address of file's previous head (for redundnacy, if head is corrupted)
        char            path[1];        // File path string
    } LogFsBlock_File;

    typedef struct
    {
        LogFsPairHeader pair_hdr;  // Pair data (pairs guarantee power loss resilience, must be first)
        uint32_t        num_bytes; // Number of data bytes in this block
        uint8_t         data;      // First data byte, used to get pointer to the actual data bytes
    } LogFsBlock_Metadata;

    typedef struct
    {
        uint32_t crc;            // Checksum must be first word in block
        uint32_t prev_data_addr; // Previous data block address
        uint32_t num_bytes;      // Number of data bytes in this block
        uint8_t  data;           // First data byte, used to get pointer to the actual data bytes
    } LogFsBlock_Data;

    /* Internal struct definitions */

    typedef struct
    {
        uint32_t addrs[LOGFS_PAIR_SIZE]; // The 2 block addresses in the pair
        uint8_t  seq_num;                // Sequence number (used to calculate most recent version of the pair)
        bool     seq_num_on_disk;        // Whether or not the sequence number has been written to disk
    } LogFsPair;

    typedef struct
    {
        uint32_t cached_addr; // Address of block in the cache
        void    *buf;         // Pointer to cache buffer (must be 1 block in size)
    } LogFsCache;

    typedef struct
    {
        // File config info.
        LogFsCache       cache;                  // Each file has its own cache
        LogFsBlock_Data *cache_data;             // Convenience pointer to cache, as a data block
        char             path[LOGFS_PATH_BYTES]; // File path string
        uint32_t         flags;

        // File state variables.
        bool      is_open;        // If the file is open
        LogFsPair file_pair;      // The pair on disk representing this file
        LogFsPair metadata_pair;  // The pair on disk holding file metadata
        uint32_t  head_data_addr; // Address of file's newest data block

        // State for iterating reads.
        uint32_t read_iter_init;      // If the read iterator has been initialized
        uint32_t read_iter_data_byte; // Number of bytes read from current block
        uint32_t read_iter_data_addr; // Current data block being read from
    } LogFsFile;

    typedef struct
    {
        char     path[LOGFS_PATH_BYTES]; // Path string
        uint32_t file_addr;              // Block for this file's info
        uint32_t next_file_addr;         // Block for next file's info
    } LogFsPath;

    typedef struct
    {
        // Filesystem config.
        const LogFsCfg *cfg; // Pointer to config struct

        // Addresses to keep track of disk usage.
        LogFsFile root_file;      // Reserved empty root file, created during a format operation
        uint32_t  head_file_addr; // Address of newest file
        uint32_t  head_addr;      // Smallest available block, to be written to next

        // Misc. state variables.
        uint32_t eff_block_size_bytes; // How many bytes
        int      max_path_len_bytes;   // Max path length that can be stored in a block,
        bool     mounted;              // If filesystem has been mounted
        bool     out_of_memory;        // If the filesystem has run out of memory

        // Utility pointers to filesystem cache.
        LogFsPairHeader     *cache_pair_hdr; // Pair header in filesystem cache (used for pair read/write ops)
        LogFsBlock_File     *cache_file;     // File block pointer to the filesystem cache (for convenience)
        LogFsBlock_Metadata *cache_metadata; // Metadata block pointer to the filesystem cache (for convenience)
        LogFsBlock_Data     *cache_data;     // Data block pointer to the filesystem cache (for convenience)
    } LogFs;

    /**
     * Mount the filesystem. Must be called before other filesystem operations can be used.
     *
     * @param fs Filesystem object.
     * @param cfg Config object.
     * @return LOGFS_ERR_OK if successful, or an error code.
     */
    LogFsErr logfs_mount(LogFs *fs, const LogFsCfg *cfg);

    /**
     * Format the filesystem. Will erase any data currently on the card. This does not mount the filesystem, call
     * `logfs_mount` after to use the filesystem.
     *
     * @param fs Filesystem object.
     * @param cfg Config object.
     * @return LOGFS_ERR_OK if successful, or an error code.
     */
    LogFsErr logfs_format(LogFs *fs, const LogFsCfg *cfg);

    /**
     * Open a file for reading/writing.
     *
     * @param fs Filesystem object.
     * @param file File handle, will represent an open file after (if successful).
     * @param cfg Config on the file you're trying to open.
     * @param flags Flags specifying how to open the file. See `LogFsOpenFlags` for options (can be ORed together).
     * @return LOGFS_ERR_OK if successful, or an error code.
     */
    LogFsErr logfs_open(LogFs *fs, LogFsFile *file, LogFsFileCfg *cfg, uint32_t flags);

    /**
     * Close a file. Syncs all cached data to the disk.
     *
     * @param fs Filesystem object.
     * @param file File handle to close.
     * @return LOGFS_ERR_OK if successful, or an error code.
     */
    LogFsErr logfs_close(LogFs *fs, LogFsFile *file);

    /**
     * Syncs all cached data to the disk, for a specific file.
     *
     * @param fs Filesystem object.
     * @param file File handle to sync.
     * @return LOGFS_ERR_OK if successful, or an error code.
     */
    LogFsErr logfs_sync(LogFs *fs, LogFsFile *file);

    /**
     * Write data to a file.
     *
     * @param fs Filesystem object.
     * @param file File handle to write to.
     * @param buf Pointer to data buffer to write from.
     * @param size Size of data to write, in bytes.
     * @return LOGFS_ERR_OK if successful, or an error code.
     */
    LogFsErr logfs_write(LogFs *fs, LogFsFile *file, void *buf, uint32_t size);

    /**
     * Read data from a file.
     *
     * Reads can be performed either from the end of the file (read last N bytes) or iteratively
     * (read next N bytes). Reading an entire file involves performing a single read from the end to reset the iterator,
     * and then call iterative reads until the entire file is read (`*num_read` is zero, indicating no more data is
     * left).
     *
     * @param fs Filesystem object.
     * @param file File handle to read from.
     * @param buf Pointer to data buffer to read to.
     * @param size Size of data to read, in bytes.
     * @param flags One of `LogFsReadFlags`.
     * @param num_read Pointer to a variable which will be populated with the number of read bytes, after a successful
     * read.
     * @return LOGFS_ERR_OK if successful, or an error code.
     */
    LogFsErr logfs_read(LogFs *fs, LogFsFile *file, void *buf, uint32_t size, LogFsReadFlags flags, uint32_t *num_read);

    /**
     * Write data to the metadata section of a file.
     *
     * To support high-bandwidth logging, regular writes/reads are not
     * safe, and there is no way to overwrite data. To get around these limitations, all files also provide a metadata
     * section, which are safe and are written to from the start. However, they are only 1 block in size. If that
     * doesn't work, use a different filesystem!
     *
     * @param fs Filesystem object.
     * @param file File handle to write to.
     * @param buf Pointer to data buffer to write from.
     * @param size Size of data to write, in bytes.
     * @return LOGFS_ERR_OK if successful, or an error code.
     */
    LogFsErr logfs_writeMetadata(LogFs *fs, LogFsFile *file, void *buf, uint32_t size);

    /**
     * Read data from the metadata section of a file.
     *
     * To support high-bandwidth logging, regular writes/reads are not
     * safe, and there is no way to overwrite data. To get around these limitations, all files also provide a metadata
     * section, which are safe and are written to from the start. However, they are only 1 block in size. If that
     * doesn't work, use a different filesystem!
     *
     * @param fs Filesystem object.
     * @param file File handle to write to.
     * @param buf Pointer to data buffer to read to.
     * @param size Size of data to read, in bytes.
     * @return LOGFS_ERR_OK if successful, or an error code.
     */
    LogFsErr logfs_readMetadata(LogFs *fs, LogFsFile *file, void *buf, uint32_t size, uint32_t *num_read);

    /**
     * Get the path to the first file on the filesystem.
     *
     * Finding paths on the filesystem is done iteratively. Files are internally stored as a linked-list, so to list all
     * files on the filesystem first call `logfs_firstPath` and call `logfs_nextPath` to traverse all files, until
     * LOGFS_ERR_NO_MORE_FILES is returned.
     *
     * @param fs Filesystem object.
     * @param path Filesystem path object, represents path to the next file after (if successful).
     * @return LOGFS_ERR_OK if successful, or an error code.
     */
    LogFsErr logfs_firstPath(LogFs *fs, LogFsPath *path);

    /**
     * Given a file path, get the path to the next file on the filesystem (the file after the one you pass in).
     *
     * Finding paths on the filesystem is done iteratively. Files are internally stored as a linked-list, so to list all
     * files on the filesystem first call `logfs_firstPath` and call `logfs_nextPath` to traverse all files, until
     * LOGFS_ERR_NO_MORE_FILES is returned.
     *
     * @param fs Filesystem object.
     * @param path Filesystem path object, represents path to the next file (if successful).
     * @return LOGFS_ERR_OK if successful, LOGFS_ERR_NO_MORE_FILES if traversed all files, or an error code.
     */
    LogFsErr logfs_nextPath(LogFs *fs, LogFsPath *path);

#ifdef __cplusplus
}
#endif
