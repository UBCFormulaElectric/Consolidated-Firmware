#include "hw_utils.hpp"

// Override the default verbose terminate handler
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