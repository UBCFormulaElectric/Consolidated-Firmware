#include "io_log.h"
#include "hw_error.h"
#include "hw_utils.h"
#include <stdio.h>

static char buffer[100];

_Noreturn void hw_error(const char *file, int line, const char *func)
{
    // Store the message into a buffer, so we can easily inspect it using a
    // debugger even without SEGGER RTT set up
    snprintf(buffer, sizeof(buffer), "%s:%d %s: Called Error_Handler().", __BASENAME__(file), line, func);
    LOG_ERROR("%s", buffer);

    BREAK_IF_DEBUGGER_CONNECTED();

    for (;;)
    {
        // Trap in a infinite loop.
    }
}
