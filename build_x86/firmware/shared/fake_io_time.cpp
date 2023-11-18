#include "fake_io_time.hpp"
#include <string.h>
#include <unordered_map>

extern "C"
{
#include "io_time.h"
}

using namespace std;

// 
// Fakes for: io_time_getCurrentMs
//

static uint32_t io_time_getCurrentMs_return_value;
static uint32_t io_time_getCurrentMs_call_count;

extern "C" 
{
    // Faked void function with no params.
    uint32_t io_time_getCurrentMs(void)
    {
        // Increment call count.
        io_time_getCurrentMs_call_count++;

        // Return custom return value.
        return io_time_getCurrentMs_return_value;
    }
}

void fake_io_time_getCurrentMs_returns(uint32_t return_value)
{
    io_time_getCurrentMs_return_value = return_value;
}

void fake_io_time_getCurrentMs_reset() 
{
    memset(&io_time_getCurrentMs_return_value, 0U, sizeof(uint32_t));
    io_time_getCurrentMs_call_count = 0;
}

uint32_t fake_io_time_getCurrentMs_callCount(void)
{
    return io_time_getCurrentMs_call_count;
}

