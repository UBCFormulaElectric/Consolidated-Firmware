#include <stdint.h>

typedef enum
{
    FILE_OK    = 0,
    FILE_ERROR = -1, // general error
} FileSystemStatus;

// init the file system
// config the file system
// mount the file system
int io_fileSystem_init(void);

// open a file and return the file descriptor
int io_fileSystem_open(const char *path, int flags);


// write return error code
int io_fileSystem_wirte(int fd, void *buf, size_t size);

// given fd, buf, size, return error code
int io_fileSystem_read(int fd, void *buf, size_t size);
// close file
int io_fileSystem_close(int fd);

// seek to the position
int io_fileSystem_seek(void);

// do the concrete write operation to the hardware
int io_fileSystem_sync(int fd);
