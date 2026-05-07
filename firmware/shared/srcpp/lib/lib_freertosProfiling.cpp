#include <cassert>

extern "C"
{
    __weak void configureTimerForRunTimeStats()
    {
        assert(false);
    };
    __weak unsigned long getRunTimeCounterValue()
    {
        assert(false);
    };
}
