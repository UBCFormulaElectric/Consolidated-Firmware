#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef enum
{
    FILE_OK            = 0,
    FILE_ERROR         = -1, // general error
    FILE_CORRUPTED     = -2, // file system corrupted
    FILE_NOT_FOUND     = -3, // file not found
    FILE_NO_SPACE      = -4, // no space left
    FILE_ERROR_IO      = -5, // io error
    FILE_ERROR_BAD_ARG = -6, // Invalid argument passed in
    FILE_MOUNT_FAILED  = -7  // Initial mount failed so filesystem is unavailable for this boot cycle
} FileSystemError;

/**
 * init the file system
 * config the file system
 * mount the file system
 * @return success of operation
 */
FileSystemError io_fileSystem_init(void);

/**
 * open a file and return the file descriptor
 * Note: Don't try opening the same file twice!
 * @param path path in question
 * @return a file descriptor
 */
FileSystemError io_fileSystem_open(const char *path, uint32_t *fd);

/**
 * writes to file descriptor for some size into the buffer
 * @param fd file descriptor in question
 * @param buf buffer in question
 * @param size size in questino
 * @return
 */
FileSystemError io_fileSystem_write(uint32_t fd, const void *buf, size_t size);

/**
 * writes to file descriptor for some metadata
 * @param fd file descriptor in question
 * @param buf buffer in question
 * @param size size in questino
 * @return
 */
FileSystemError io_fileSystem_writeMetadata(uint32_t fd, const void *buf, size_t size);

/**
 * reads from file descriptor for some size into the buffer
 * @param fd file descriptor in question
 * @param buf buffer in question
 * @param size size in questino
 * @return success of operation
 */
FileSystemError io_fileSystem_read(uint32_t fd, void *buf, size_t size);

/**
 * Closes file given file descriptor
 * @param fd file descriptor in question
 * @return success of operation
 */
FileSystemError io_fileSystem_close(uint32_t fd);

/**
 * @return The bootcount of a given file system
 */
FileSystemError io_fileSystem_getBootCount(uint32_t *bootcount);

/**
 * do the concrete write operation to the hardware
 * @param fd on this file descriptor
 * @return success of operation
 */
FileSystemError io_fileSystem_sync(uint32_t fd);

/**
 * Checks if the SD card is present.
 */
bool io_fileSystem_present(void);
