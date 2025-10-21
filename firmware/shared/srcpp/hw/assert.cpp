#include <cstdio>
#include "app/utils.hpp"
#include "hw/utils.hpp"
#include "io/log.hpp"

static char buffer[100];

void __assert_func(const char *file, int line, const char *func, const char *failedexpr)
{
    // Store the message into a buffer so we can easily inspect it using a
    // debugger even without SEGGER RTT set up
    snprintf(buffer, sizeof(buffer), "%s:%d %s: Assertion `%s' failed\r\n", __BASENAME__(file), line, func, failedexpr);
    LOG_ERROR("%s", buffer);

    BREAK_IF_DEBUGGER_CONNECTED();

    for (;;)
    {
        // Trap in a infinite loop, waiting for the hardware watchdog to reset
    }
}
