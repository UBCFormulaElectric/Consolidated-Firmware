#include <stdint.h>
#include <stddef.h>

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

// init the file system
// config the file system
// mount the file system
FileSystemError io_fileSystem_init(void);

// open a file and return the file descriptor
int io_fileSystem_open(const char *path);

// write return error code
FileSystemError io_fileSystem_write(int fd, void *buf, size_t size);

// given fd, buf, size, return error code
FileSystemError io_fileSystem_read(int fd, void *buf, size_t size);
// close file
FileSystemError io_fileSystem_close(int fd);

uint32_t io_fileSystem_getBootCount(void);

// do the concrete write operation to the hardware
FileSystemError io_fileSystem_sync(int fd);
