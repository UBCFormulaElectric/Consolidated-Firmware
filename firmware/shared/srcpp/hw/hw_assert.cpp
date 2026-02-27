#include <cstdio>
#include "hw_utils.hpp"
#include "io_log.hpp"

extern "C" [[noreturn]] void
    __assert_func(const char *file, int line, const char *func, const char *failedexpr) // NOLINT(*-reserved-identifier)
{
    // Store the message into a buffer so we can easily inspect it using a
    // debugger even without SEGGER RTT set up
    LOG_ERROR("%s:%d %s: Assertion `%s' failed\r\n", __BASENAME__(file), line, func, failedexpr);

    BREAK_IF_DEBUGGER_CONNECTED();

    for (;;)
    {
        // Trap in a infinite loop, waiting for the hardware watchdog to reset
    }
}
