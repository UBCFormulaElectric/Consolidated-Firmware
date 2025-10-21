#include <cassert>

extern "C"
{
#include "io/bootHandler.h"
    void io_bootHandler_processBootRequest(const CanMsg *msg)
    {
        (void)msg;
    }
}