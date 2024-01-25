#pragma once

typedef enum
{
    EXIT_CODE_OK = 0,
    EXIT_CODE_INVALID_ARGS,
    EXIT_CODE_OUT_OF_RANGE,
    EXIT_CODE_TIMEOUT,
    EXIT_CODE_ERROR,
    EXIT_CODE_UNIMPLEMENTED,
    NUM_EXIT_CODES,
} ExitCode;

#define EXIT_OK(code) ((code) == EXIT_CODE_OK)

#define RETURN_CODE_IF_EXIT_NOT_OK(code) \
    if ((code) != EXIT_CODE_OK)          \
    return (code)
