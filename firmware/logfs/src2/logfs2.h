#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define BLOCK_SIZE (512U)

typedef enum {
    LOGFS_ERR_OK,
    LOGFS_ERR_CORRUPT
} LogFsErr;


typedef uint8_t BlockBuf[BLOCK_SIZE];

typedef uint32_t BlockAddr;

#define BLOCK_ADDR_INVALID ((BlockAddr)0xFFFFFFFF)

typedef struct {

} LogFs;


#ifdef __cplusplus
}
#endif
