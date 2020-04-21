#pragma once

typedef enum
{
    EXIT_CODE_OK = 0,
    EXIT_CODE_INVALID_ARGS,
    NUM_EXIT_STATUSES,
} ExitCode;

#define EXIT_CODE_OK(code) (EXIT_CODE_OK == (code))
