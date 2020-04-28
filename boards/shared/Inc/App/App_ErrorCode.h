#pragma once

typedef enum
{
    EXIT_CODE_OK = 0,
    EXIT_CODE_INVALID_ARGS,
    EXIT_CODE_OUT_OF_RANGE,
    NUM_EXIT_CODES,
} ExitCode;

#define EXIT_CODE_OK(code) (EXIT_CODE_OK == (code))
