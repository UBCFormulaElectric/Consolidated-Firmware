#include "fake_io_switch.hpp"
#include <string.h>
#include <unordered_map>

extern "C"
{
#include "io_switch.h"
}

using namespace std;

// 
// Fakes for: io_switch_isClosed
//

// Class representing the parameters for the function being faked.
class io_switch_isClosed_Params
{
  public:
    const Switch* arg0;

    io_switch_isClosed_Params(
        const Switch* arg0
    ) :
        arg0(arg0) { }

    bool operator==(const io_switch_isClosed_Params &other) const
    {
        return
            arg0 == other.arg0;
    }
};

// Create a custom hashing function so we can use unordered_map (a hash table under the hood).
template <>
struct std::hash<io_switch_isClosed_Params>
{
    std::size_t operator()(const io_switch_isClosed_Params &params) const
    {
        return
            hash<const Switch*>()(params.arg0);
    }
};

// State variables.
static unordered_map<io_switch_isClosed_Params, bool> io_switch_isClosed_return_value_map;
static uint32_t io_switch_isClosed_call_count;
static unordered_map<io_switch_isClosed_Params, uint32_t> io_switch_isClosed_call_count_map;

extern "C"
{
    bool io_switch_isClosed(
        const Switch* arg0
    )
    {
        // Create an instance of this function's parameters.
        io_switch_isClosed_Params params(
            arg0
        );

        // Increment call count.
        io_switch_isClosed_call_count++;
        if(io_switch_isClosed_call_count_map.count(params)) {
            io_switch_isClosed_call_count_map[params]++;
        }
        else {
            io_switch_isClosed_call_count_map[params] = 1;
        }

        // Return custom return value.
        if(io_switch_isClosed_return_value_map.count(params)) {
            return io_switch_isClosed_return_value_map[params];
        }
        else {
            bool default_return_value;
            memset(&default_return_value, 0U, sizeof(bool));
            return default_return_value;
        }
    }
}

void fake_io_switch_isClosed_reset() 
{
    io_switch_isClosed_return_value_map.clear();
    io_switch_isClosed_call_count = 0;
    io_switch_isClosed_call_count_map.clear();
}

void fake_io_switch_isClosed_returnsForArgs(
    const Switch* arg0,
    bool return_value
)
{
    // Create an instance of this function's parameters.
    io_switch_isClosed_Params params(
        arg0
    );

    // Update return value map.
    io_switch_isClosed_return_value_map[params] = return_value;
}

uint32_t fake_io_switch_isClosed_callCount(void)
{
    return io_switch_isClosed_call_count;
}

uint32_t fake_io_switch_isClosed_callCountForArgs(
    const Switch* arg0
)
{
    // Create an instance of this function's parameters.
    io_switch_isClosed_Params params(
        arg0
    );

    // Return call count from map.
    if(io_switch_isClosed_call_count_map.count(params)) {
        return io_switch_isClosed_call_count_map[params];
    }
    else {
        return 0;
    }
}

