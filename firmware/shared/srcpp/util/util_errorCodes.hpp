#pragma once
#include "io_log.hpp"
#include <expected>
#include <source_location>

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

constexpr const char *error_code_to_string(const ErrorCode code)
{
    switch (code)
    {
        case ErrorCode::INVALID_ARGS:
            return "Invalid arguments";
        case ErrorCode::OUT_OF_RANGE:
            return "Out of range";
        case ErrorCode::TIMEOUT:
            return "Timeout";
        case ErrorCode::ERROR:
            return "Error";
        case ErrorCode::BUSY:
            return "Busy";
        case ErrorCode::UNIMPLEMENTED:
            return "Unimplemented";
        case ErrorCode::RETRY_FAILED:
            return "Retry failed";
        case ErrorCode::CHECKSUM_FAIL:
            return "Checksum fail";
        case ErrorCode::ERROR_INDETERMINATE:
            return "Indeterminate error";
        case ErrorCode::NUM_EXIT_CODES:
        default:
            return "Unknown error code";
    }
}

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

template <typename T>
void _log_if_err(
    const std::expected<T, ErrorCode> out,
    const char                       *err_expr,
    const std::source_location        loc = std::source_location::current())
{
    if (not out)
    {
        LOG_ERROR({ "%s exited with an error: %s", loc }, err_expr, error_code_to_string(out.error()));
    }
}
// this needs to be a macro because it takes an expression and converts it to string
// i think at some point we will have strong enough reflection to handle this, but not yet
#define LOG_IF_ERR(err_expr) _log_if_err(err_expr, #err_expr)