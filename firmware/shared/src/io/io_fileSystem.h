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
 * @param path path in question
 * @return a file descriptor
 */
int io_fileSystem_open(const char *path);

/**
 * writes to file descriptor for some size into the buffer
 * @param fd file descriptor in question
 * @param buf buffer in question
 * @param size size in questino
 * @return
 */
FileSystemError io_fileSystem_write(int fd, void *buf, size_t size);

/**
 * reads from file descriptor for some size into the buffer
 * @param fd file descriptor in question
 * @param buf buffer in question
 * @param size size in questino
 * @return success of operation
 */
FileSystemError io_fileSystem_read(int fd, void *buf, size_t size);

/**
 * Closes file given file descriptor
 * @param fd file descriptor in question
 * @return success of operation
 */
FileSystemError io_fileSystem_close(int fd);

/**
 * @return The bootcount of a given file system
 */
uint32_t io_fileSystem_getBootCount(void);

/**
 * do the concrete write operation to the hardware
 * @param fd on this file descriptor
 * @return success of operation
 */
FileSystemError io_fileSystem_sync(int fd);

bool io_fileSystem_ready();
