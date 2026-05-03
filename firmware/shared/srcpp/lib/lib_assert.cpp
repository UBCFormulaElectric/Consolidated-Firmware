#include "hw_utils.hpp"
#include "io_log.hpp"
#include "util_utils.hpp"

// Override libstdc++'s internal assert to prevent it pulling in fprintf/vfprintf
// (~17 KB) and the locale/unicode cascade it drags in.
namespace std
{
[[noreturn]] void
    __glibcxx_assert_fail(const char * /*file*/, int /*line*/, const char * /*func*/, const char * /*cond*/) noexcept
{
    BREAK_IF_DEBUGGER_CONNECTED();
    for (;;)
    {
    }
}
} // namespace std

extern "C" [[noreturn]] void
    __assert_func(const char *file, int line, const char *func, const char *failedexpr) // NOLINT(*-reserved-identifier)
{
    // Log directly through RTT without heavy snprintf
    LOG_ERROR("Assertion failed: %s at %s:%d in %s", failedexpr, __BASENAME__(file), line, func);

    BREAK_IF_DEBUGGER_CONNECTED();

    for (;;)
    {
        // Trap in a infinite loop, waiting for the hardware watchdog to reset
    }
}
