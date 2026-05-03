#include "io_log.hpp"
#include "hw_error.hpp"
#include "hw_utils.hpp"

void hw_error(const char *file, int line, const char *func)
{
    // Log directly through RTT without heavy snprintf
    LOG_ERROR("Called Error_Handler() at %s:%d in %s", __BASENAME__(file), line, func);

    BREAK_IF_DEBUGGER_CONNECTED();

    for (;;)
    {
        // Trap in a infinite loop.
    }
}
