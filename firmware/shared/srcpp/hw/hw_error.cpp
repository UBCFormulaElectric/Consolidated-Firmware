#include "io_log.hpp"
#include "hw_error.hpp"
#include "hw_utils.hpp"

// Override the default verbose terminate handler to prevent libstdc++ from
// pulling in __cxa_demangle (cp-demangle.o, ~28 KB) via vterminate.o.
namespace __gnu_cxx
{
[[noreturn]] void __verbose_terminate_handler()
{
    BREAK_IF_DEBUGGER_CONNECTED();
    for (;;)
    {
    }
}
} // namespace __gnu_cxx

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
