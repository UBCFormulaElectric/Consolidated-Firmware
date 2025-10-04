#include <cassert>

extern "C"
{
#include "io_bootHandler.h"
    void io_bootHandler_processBootRequest(const CanMsg *msg)
    {
        (void)msg;
        assert(false); // not allowed
    }
}