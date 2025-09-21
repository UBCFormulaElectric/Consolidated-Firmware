#pragma once
#include "io_log.h"
#include <assert.h>

enum class
{
    EXIT_CODE_OK = 0,
    EXIT_CODE_INVALID_ARGS,
    EXIT_CODE_OUT_OF_RANGE,
    EXIT_CODE_TIMEOUT,
    EXIT_CODE_ERROR,
    EXIT_CODE_BUSY,
    EXIT_CODE_UNIMPLEMENTED,
    EXIT_CODE_RETRY_FAILED,
    EXIT_CODE_CHECKSUM_FAIL,
    EXIT_INDETERMINATE, // use this for when you don't know what the exit code is YET
    NUM_EXIT_CODES,
} ExitCode;

#define IS_EXIT_OK(code) ((code) == EXIT_CODE_OK)
#define IS_EXIT_ERR(code) ((code) != EXIT_CODE_OK)
#define ASSERT_EXIT_OK(code) (assert(code == EXIT_CODE_OK))

#define RETURN_IF_ERR(err_expr)                                                \
    {                                                                          \
        const ExitCode exit = err_expr;                                        \
        if (IS_EXIT_ERR(exit))                                                 \
        {                                                                      \
            LOG_ERROR(#err_expr " exited with an error, returning: %d", exit); \
            return exit;                                                       \
        }                                                                      \
    }

#define RETURN_IF_ERR_SILENT(err_expr)  \
    {                                   \
        const ExitCode exit = err_expr; \
        if (IS_EXIT_ERR(exit))          \
        {                               \
            return exit;                \
        }                               \
    }

#define LOG_IF_ERR(err_expr)                                        \
    {                                                               \
        const ExitCode exit = err_expr;                             \
        if (IS_EXIT_ERR(exit))                                      \
        {                                                           \
            LOG_ERROR(#err_expr " exited with an error: %d", exit); \
        }                                                           \
    }
