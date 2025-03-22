#pragma once

#include "logfs2.h"
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define MAX(x, y) (((x) > (y)) ? (x) : (y))

#define IS_ERR(err) (err != LOGFS_ERR_OK)

#define RET_ERR(err_expr)              \
    {                                  \
        const LogFsErr err = err_expr; \
        if (IS_ERR(err))               \
        {                              \
            return err;                \
        }                              \
    }

// #define CHECK_ARG(arg)                \
//     if (arg == NULL)                  \
//     {                                 \
//         return LOGFS_ERR_INVALID_ARG; \
//     }

// #define CHECK_PATH(path)                                              \
//     CHECK_ARG(path);                                                  \
//     if (path[0] != '/' || (int)strlen(path) > fs->max_path_len_bytes) \
//     {                                                                 \
//         return LOGFS_ERR_INVALID_PATH;                                \
//     }

// #define CHECK_FILE(file)           \
//     if (!file->is_open)            \
//     {                              \
//         return LOGFS_ERR_NOT_OPEN; \
//     }

// #define CHECK_FS(fs)                \
//     if (!fs->mounted)               \
//     {                               \
//         return LOGFS_ERR_UNMOUNTED; \
//     }                               \
//     if (fs->out_of_memory)          \
//     {                               \
//         return LOGFS_ERR_NOMEM;     \
//     }

// #define INC_HEAD(fs, num)                                \
//     {                                                    \
//         if (fs->head_addr + num >= fs->cfg->block_count) \
//         {                                                \
//             fs->out_of_memory = true;                    \
//             return LOGFS_ERR_NOMEM;                      \
//         }                                                \
//         fs->head_addr += num;                            \
//     }

    LogFsErr utils_findMaxInSortedCircularBuffer(
        void *context,
        int   size,
        LogFsErr (*compare_items)(void *context, size_t idx1, size_t idx2, bool* out),
        bool (*is_item_corrupted)(void *context, size_t idx),
        uint32_t* out
    );

#ifdef __cplusplus
}
#endif
