#include "App_SharedExitStatus.h"

static struct ExitStatus exit_status  =
{
        .code = EXIT_CODE_OK,
        .source = "",
        .caller = "",
        .message = ""
};


ExitCode

ExitCode App_SharedExitStatus_Update(struct ExitStatus status, ExitCode code, const char* source, const char* caller,  const char* message)
{
    exit_status =
}

struct ExitStatus App_SharedExitStatus_Get(void)
{
    // The exit status holds pointers so it's fine to just return a safe copy
    // for the caller to use
    return exit_status;
}

