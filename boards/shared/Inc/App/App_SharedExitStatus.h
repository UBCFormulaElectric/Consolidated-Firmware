#pragma once

typedef enum
{
    EXIT_CODE_OK = 0,
    EXIT_CODE_INVALID_ARGS,
    EXIT_CODE_OUT_OF_RANGE,
    NUM_EXIT_CODES,
} ExitCode;

struct ExitStatus {
    ExitCode code;
    const char *source;
    const char *caller;
    const char *message;
};


rdefine EXIT_CODE(code) exit_status_impl_update((code), (__FILE__ ":" STRINGIFY(__LINE__)), (__FUNCTION__), "")
#define EXIT_MSG(code, message) exit_status_impl_update((code), (__FILE__ ":" STRINGIFY(__LINE__)), (__FUNCTION__), (message))
#define EXIT_CODE_OK(code) (EXIT_CODE_OK == (code))
