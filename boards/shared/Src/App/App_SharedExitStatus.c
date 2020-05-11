#ifdef __arm__
#include <FreeRTOS.h>
#include <task.h>
#else
#include <pthread.h>
#endif

#include <assert.h>
#include "App_SharedExitStatus.h"

static ExitStatusCallback exit_status_callback;

static struct ExitStatus *App_GetThreadLocalExitStatus(void)
{
#ifdef __arm__
    // Use this exit status before the scheduler starts
    static struct ExitStatus main_thread_exit_status = {
        .code = EXIT_CODE_OK, .source = "", .caller = "", .message = ""
    };

    if (xTaskGetSchedulerState() == taskSCHEDULER_NOT_STARTED)
    {
        return &main_thread_exit_status;
    }
    else
    {
        // Once the scheduler has started, we use the thread-local exit status
        return (struct ExitStatus *)pvTaskGetThreadLocalStoragePointer(NULL, 0);
    }
#else
    static __thread struct ExitStatus thread_local_exit_status = {
        .code = EXIT_CODE_OK, .source = "", .caller = "", .message = ""
    };
    return &thread_local_exit_status;
#endif
}

ExitCode App_SharedExitStatus_Update(
    ExitCode    code,
    const char *source,
    const char *caller,
    const char *message)
{
    // We are not interested in handling non-errors
    if (EXIT_CODE_OK(code))
        return code;

    struct ExitStatus *exit_status = App_GetThreadLocalExitStatus();
    assert(exit_status != NULL);

    exit_status->code    = code;
    exit_status->source  = source;
    exit_status->caller  = caller;
    exit_status->message = message;

    if (exit_status_callback != NULL)
    {
        exit_status_callback(*exit_status);
    }

    return code;
}

struct ExitStatus App_SharedExitStatus_Get(void)
{
    return *App_GetThreadLocalExitStatus();
}

void App_SharedExitStatus_SetUpdateCallback(ExitStatusCallback callback)
{
    exit_status_callback = callback;
}
