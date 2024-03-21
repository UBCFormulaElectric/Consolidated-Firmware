#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

    /*
    Power loss resilience for critical data is guaranteed with "pairs." Pairs consist of 2 blocks: Block 0 and Block 1.
    Writes to pairs are done in an alternating order, so for ex. block 0, then block 1, then block 0, etc. This means
    that if a write fails due to power loss, we can roll back to the other pair block.

    How do we know which element in the pair is the most recent? If either element is invalid, we just have to
    pick the valid one. If both are valid, we use the "sequence number" stored in each block.

    Sequence numbers have 4 possible values, and are initialized to zero. Sequence number comparsions are made like:
    - 4 > 3
    - 3 > 2
    - 2 > 1
    - 0 > 4

    Each element of the pair has its own sequence number that is written to disk. Upon reading a pair with an unknown
    state, the two sequence numbers can be compared as above, and the element with the most significant number will be
    taken to be the most recent. So, when alternating between element writes, we need to always increment the sequence
    number by 1. This way, our most recent write will always have the greatest sequence number, which we want!

    A few optimizations:
    1. We can say that even sequence numbers correspond to Block 0 being the most recent, and odd correspond to Block 1
    being the most recent. Then we only need to keep track of the sequence number.
    2. We can cache sequence number for blocks we're going to want to use again. Then, we don't need to figure out which
    is the most recent for later reads/writes. Be careful! If cacheing is not done properly, you could easily use
    invalid/old data.
    */

    // TODO: If you write a piece of data that crosses a block boundary, the
    // second write could fail after the first succeeds. This could be bad,
    // because if you rely on the last N bytes (ex. boot count), it could
    // grab a partially-written byte sequence, and so carry invalid data.

    // TODO 2: Consider splitting logfs_open into a 2 functions, 1 that only
    // searches the disk for existing files, and another that just creates a
    // new file.

    // TODO 3: Figure out a better way of handling file cfg struct. Very easy for pointers to become dangling right now.

    // TODO 4: If a file has been closed, notify the user in some way rather than just saying "corrupt!".

    // TODO 5: Looks like after cacheing, the reads/writes are a lot lower, but the runtime is still slow! Maybe just

    // need to make some runtime optimizations... (fixed by increasing optimization level)

    /* TODO 6: How does a write operation fail? Let's say a block already has some data on it so the file block says its
     * the head. What happens if you then write some more data to the block, but while writing, power is lost? Then
     * theoretically the data block could be corrupted, but the file block still points to it, leading to a lost file?
     * Cacheing should help here, but should still be fixed. This could still happen after a sync... */
    // Soln: Just have a "previous block" address and update if the head is invalid.

    // TODO 7: Give each file a "metadata block" where the user can attach metadata for writes that can't fail. These
    // blocks will be a pair so they are resilient to power loss. Could also look into making them get evicted after N
    // writes.

#include <stdint.h>
#include <stdbool.h>

#define LOGFS_ORIGIN 0                 // Filesystem starts at address zero
#define LOGFS_INVALID_BLOCK 0xFFFFFFFF // Indicates invalid block
#define LOGFS_PATH_BYTES 128           // Max bytes to allocate for a path
#define LOGFS_PAIR_SIZE 2

    /* Enum definitions */

    typedef enum
    {
        LOGFS_ERR_OK,            // No error
        LOGFS_ERR_IO,            // Error during I/O operation
        LOGFS_ERR_CORRUPT,       // File system was corrupted (bad CRC)
        LOGFS_ERR_INVALID_ARG,   // Invalid argument
        LOGFS_ERR_INVALID_PATH,  // Invalid path
        LOGFS_ERR_UNMOUNTED,     // Filesystem hasn't been successfully mounted
        LOGFS_ERR_NOMEM,         // Filesystem is full (no more memory)
        LOGFS_ERR_NOT_OPEN,      // File hasn't been opened
        LOGFS_ERR_UNIMPLEMENTED, // Feature not implemented
    } LogFsErr;

    typedef enum
    {
        LOGFS_READ_MODE_END,  // Read from start of file
        LOGFS_READ_MODE_ITER, //
    } LogFsReadMode;

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
    } LogFsCfg;

    typedef struct
    {
        char *path;  // File path string
        void *cache; // Pointer to cache buffer (must be 1 block in size)
    } LogFsFileCfg;

    /* Block definitions */

    typedef struct
    {
        uint32_t crc;                    // Checksum must be first word in block
        uint8_t  seq_num;                // Sequence number (used to calculate most recent version of the pair)
        uint32_t next_file_addr;         // Address of the next file block
        uint32_t metadata_addr;          // Address of the file's metadata block
        uint32_t head_data_addr;         // Address of file's newest data block
        char     path[LOGFS_PATH_BYTES]; // File path string
    } LogFsBlock_File;

    typedef struct
    {
        uint32_t crc;       // Checksum must be first word in block
        uint8_t  seq_num;   // Sequence number (used to calculate most recent version of the pair)
        uint32_t num_bytes; // Number of data bytes in this block
        uint8_t  data;      // First data byte, used to get pointer to the actual data bytes
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
        uint32_t max_path_len_bytes;   // Max path length that can be stored in a block,
        bool     mounted;              // If filesystem has been mounted
        bool     out_of_memory;        // If the filesystem has run out of memory

        // Utility pointers to filesystem cache.
        LogFsBlock_File     *cache_file;     // File block pointer to the filesystem cache (for convenience)
        LogFsBlock_Metadata *cache_metadata; // Metadata block pointer to the filesystem cache (for convenience)
        LogFsBlock_Data     *cache_data;     // Data block pointer to the filesystem cache (for convenience)
    } LogFs;

    LogFsErr logfs_mount(LogFs *fs, const LogFsCfg *cfg);
    LogFsErr logfs_format(LogFs *fs, const LogFsCfg *cfg);

    LogFsErr logfs_open(LogFs *fs, LogFsFile *file, LogFsFileCfg *cfg);
    LogFsErr logfs_close(LogFs *fs, LogFsFile *file);
    LogFsErr logfs_sync(LogFs *fs, LogFsFile *file);

    LogFsErr logfs_write(LogFs *fs, LogFsFile *file, void *buf, uint32_t size);
    LogFsErr logfs_read(LogFs *fs, LogFsFile *file, void *buf, uint32_t size, LogFsReadMode mode, uint32_t *num_read);
    LogFsErr logfs_writeMetadata(LogFs *fs, LogFsFile *file, void *buf, uint32_t size);
    LogFsErr logfs_readMetadata(LogFs *fs, LogFsFile *file, void *buf, uint32_t size, uint32_t *num_read);

    LogFsErr logfs_firstPath(LogFs *fs, LogFsPath *path);
    LogFsErr logfs_nextPath(LogFs *fs, LogFsPath *path);

#ifdef __cplusplus
}
#endif
