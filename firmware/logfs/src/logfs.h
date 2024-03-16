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
        LOGFS_ERR_INVALID_BLOCK, // Requested an operator on an invalid block
        LOGFS_ERR_UNMOUNTED,     // Filesystem hasn't been successfully mounted
        LOGFS_ERR_NOMEM,         // Filesystem is full (no more memory)
        LOGFS_ERR_UNIMPLEMENTED, // Feature not implemented
    } LogFsErr;

    typedef enum
    {
        LOGFS_READ_START,
        LOGFS_READ_ITER,
    } LogFsReadMode;

    /* Config structs */

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

    /* Block definitions */
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

    /* Internal struct definitions */

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
    LogFsErr logfs_read(LogFs *fs, LogFsFile *file, void *buf, uint32_t size, LogFsReadMode mode, uint32_t *num_read);

    LogFsErr logfs_firstPath(LogFs *fs, LogFsPath *path);
    LogFsErr logfs_nextPath(LogFs *fs, LogFsPath *path);

#ifdef __cplusplus
}
#endif
