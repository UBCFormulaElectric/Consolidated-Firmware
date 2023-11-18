#include "fake_io_led.hpp"
#include <string.h>
#include <unordered_map>

extern "C"
{
#include "io_led.h"
}

using namespace std;

// 
// Fakes for: io_led_enable
//

// Class representing the parameters for the function being faked.
class io_led_enable_Params
{
  public:
    const BinaryLed* arg0;
    bool arg1;

    io_led_enable_Params(
        const BinaryLed* arg0, 
        bool arg1
    ) :
        arg0(arg0), 
        arg1(arg1) { }

    bool operator==(const io_led_enable_Params &other) const
    {
        return
            arg0 == other.arg0 &&
            arg1 == other.arg1;
    }
};

// Create a custom hashing function so we can use unordered_map (a hash table under the hood).
template <>
struct std::hash<io_led_enable_Params>
{
    std::size_t operator()(const io_led_enable_Params &params) const
    {
        return
            hash<const BinaryLed*>()(params.arg0) ^
            hash<bool>()(params.arg1);
    }
};

// State variables.
static uint32_t io_led_enable_call_count;
static unordered_map<io_led_enable_Params, uint32_t> io_led_enable_call_count_map;

extern "C" 
{
    void io_led_enable(
        const BinaryLed* arg0, 
        bool arg1
    )
    {
        // Create an instance of this function's parameters.
        io_led_enable_Params params(
            arg0, 
            arg1
        );

        // Increment call count.
        io_led_enable_call_count++;
        if(io_led_enable_call_count_map.count(params)) {
            io_led_enable_call_count_map[params]++;
        }
        else {
            io_led_enable_call_count_map[params] = 1;
        }
    }
}

void fake_io_led_enable_reset() 
{
    io_led_enable_call_count = 0;
    io_led_enable_call_count_map.clear();
}

uint32_t fake_io_led_enable_callCount(void)
{
    return io_led_enable_call_count;
}

uint32_t fake_io_led_enable_callCountForArgs(
    const BinaryLed* arg0, 
    bool arg1
)
{
    // Create an instance of this function's parameters.
    io_led_enable_Params params(
        arg0, 
        arg1
    );

    // Return call count from map.
    if(io_led_enable_call_count_map.count(params)) {
        return io_led_enable_call_count_map[params];
    }
    else {
        return 0;
    }
}

