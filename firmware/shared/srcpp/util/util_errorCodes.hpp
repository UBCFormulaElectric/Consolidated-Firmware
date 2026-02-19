#pragma once
#include "io_log.hpp"
#include <expected>

enum class ErrorCode
{
    INVALID_ARGS = 0,
    OUT_OF_RANGE,
    TIMEOUT,
    ERROR,
    BUSY,
    UNIMPLEMENTED,
    RETRY_FAILED,
    CHECKSUM_FAIL,
    ERROR_INDETERMINATE, // use this for when you don't know what the exit code is YET
    NUM_EXIT_CODES,
};

#define RETURN_IF_ERR(err_expr)                                                \
    {                                                                          \
        if (const auto res = err_expr; not res)                                \
        {                                                                      \
            LOG_ERROR(#err_expr " exited with an error, returning: %d", exit); \
            return std::unexpected(res.error());                               \
        }                                                                      \
    }

#define RETURN_IF_ERR_SILENT(err_expr)           \
    {                                            \
        if (const auto res = err_expr; not res)  \
        {                                        \
            return std::unexpected(res.error()); \
        }                                        \
    }

template <typename T> void _log_if_err(std::expected<T, ErrorCode> out, const char *err_expr)
{
    if (not out)
    {
        LOG_ERROR("%s exited with an error: %d", err_expr, static_cast<int>(out.error()));
    }
}
#define LOG_IF_ERR(err_expr) _log_if_err(err_expr, #err_expr)