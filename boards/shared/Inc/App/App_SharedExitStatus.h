#pragma once

struct ExitStatus *App_GetThreadLocalExitStatus(void);

/**
 * TODO:
 */

#include "App_SharedMacros.h"

typedef enum
{
    EXIT_CODE_OK = 0,
    EXIT_CODE_INVALID_ARGS,
    EXIT_CODE_OUT_OF_RANGE,
    NUM_EXIT_CODES,
} ExitCode;

struct ExitStatus
{
    ExitCode    code;
    const char *source;
    const char *caller;
    const char *message;
};

typedef void (*ExitStatusCallback)(struct ExitStatus *status);

// clang-format off
#define EXIT_CODE(code)                  \
    App_SharedExitStatus_Update(         \
        code, __BASENAME__(__FILE__) , __func__, "")

#define EXIT_MSG(code, message)          \
    App_SharedExitStatus_Update(         \
        code, __BASENAME__(__FILE__) , __func__, message)

#define EXIT_CODE_OK(code) (EXIT_CODE_OK == (code))
// clang-format on

/**
 * Update the thread-local exit status with the given information
 * @note If the given code is ERROR_CODE_OK, then nothing gets updated as we are
 *       only interested in actual errors.
 * @note This function should only be used through the provided macros. Do not
 *       call this function directly.
 * @param code The exit code
 * @param source The source file and line number
 * @param caller The caller function
 * @param message The message describing the exit status
 * @return The exit code
 */
ExitCode App_SharedExitStatus_Update(
    ExitCode    code,
    const char *source,
    const char *caller,
    const char *message);

/**
 * Set the callback function, which is called whenever the thread-local
 * exit status changes
 * @note The exit statuses share the same callback, so make sure the
 * callback function is reentrant in multi-threaded environment
 * @param callback The callback function to set
 */
void App_SharedExitStatus_SetUpdateCallback(ExitStatusCallback callback);

/**
 * Return a copy of thread-local the exit status for the caller to use
 * @return A copy of the exit status
 */
struct ExitStatus App_SharedExitStatus_Get(void);
