#include "fake_io_rgbLed.hpp"
#include <string.h>
#include <unordered_map>

extern "C"
{
#include "io_rgbLed.h"
}

using namespace std;

// 
// Fakes for: io_rgbLed_enable
//

// Class representing the parameters for the function being faked.
class io_rgbLed_enable_Params
{
  public:
    const RgbLed* arg0;
    bool arg1;
    bool arg2;
    bool arg3;

    io_rgbLed_enable_Params(
        const RgbLed* arg0, 
        bool arg1, 
        bool arg2, 
        bool arg3
    ) :
        arg0(arg0), 
        arg1(arg1), 
        arg2(arg2), 
        arg3(arg3) { }

    bool operator==(const io_rgbLed_enable_Params &other) const
    {
        return
            arg0 == other.arg0 &&
            arg1 == other.arg1 &&
            arg2 == other.arg2 &&
            arg3 == other.arg3;
    }
};

// Create a custom hashing function so we can use unordered_map (a hash table under the hood).
template <>
struct std::hash<io_rgbLed_enable_Params>
{
    std::size_t operator()(const io_rgbLed_enable_Params &params) const
    {
        return
            hash<const RgbLed*>()(params.arg0) ^
            hash<bool>()(params.arg1) ^
            hash<bool>()(params.arg2) ^
            hash<bool>()(params.arg3);
    }
};

// State variables.
static uint32_t io_rgbLed_enable_call_count;
static unordered_map<io_rgbLed_enable_Params, uint32_t> io_rgbLed_enable_call_count_map;

extern "C" 
{
    void io_rgbLed_enable(
        const RgbLed* arg0, 
        bool arg1, 
        bool arg2, 
        bool arg3
    )
    {
        // Create an instance of this function's parameters.
        io_rgbLed_enable_Params params(
            arg0, 
            arg1, 
            arg2, 
            arg3
        );

        // Increment call count.
        io_rgbLed_enable_call_count++;
        if(io_rgbLed_enable_call_count_map.count(params)) {
            io_rgbLed_enable_call_count_map[params]++;
        }
        else {
            io_rgbLed_enable_call_count_map[params] = 1;
        }
    }
}

void fake_io_rgbLed_enable_reset() 
{
    io_rgbLed_enable_call_count = 0;
    io_rgbLed_enable_call_count_map.clear();
}

uint32_t fake_io_rgbLed_enable_callCount(void)
{
    return io_rgbLed_enable_call_count;
}

uint32_t fake_io_rgbLed_enable_callCountForArgs(
    const RgbLed* arg0, 
    bool arg1, 
    bool arg2, 
    bool arg3
)
{
    // Create an instance of this function's parameters.
    io_rgbLed_enable_Params params(
        arg0, 
        arg1, 
        arg2, 
        arg3
    );

    // Return call count from map.
    if(io_rgbLed_enable_call_count_map.count(params)) {
        return io_rgbLed_enable_call_count_map[params];
    }
    else {
        return 0;
    }
}

// 
// Fakes for: io_rgbLed_disable
//

// Class representing the parameters for the function being faked.
class io_rgbLed_disable_Params
{
  public:
    const RgbLed* arg0;

    io_rgbLed_disable_Params(
        const RgbLed* arg0
    ) :
        arg0(arg0) { }

    bool operator==(const io_rgbLed_disable_Params &other) const
    {
        return
            arg0 == other.arg0;
    }
};

// Create a custom hashing function so we can use unordered_map (a hash table under the hood).
template <>
struct std::hash<io_rgbLed_disable_Params>
{
    std::size_t operator()(const io_rgbLed_disable_Params &params) const
    {
        return
            hash<const RgbLed*>()(params.arg0);
    }
};

// State variables.
static uint32_t io_rgbLed_disable_call_count;
static unordered_map<io_rgbLed_disable_Params, uint32_t> io_rgbLed_disable_call_count_map;

extern "C" 
{
    void io_rgbLed_disable(
        const RgbLed* arg0
    )
    {
        // Create an instance of this function's parameters.
        io_rgbLed_disable_Params params(
            arg0
        );

        // Increment call count.
        io_rgbLed_disable_call_count++;
        if(io_rgbLed_disable_call_count_map.count(params)) {
            io_rgbLed_disable_call_count_map[params]++;
        }
        else {
            io_rgbLed_disable_call_count_map[params] = 1;
        }
    }
}

void fake_io_rgbLed_disable_reset() 
{
    io_rgbLed_disable_call_count = 0;
    io_rgbLed_disable_call_count_map.clear();
}

uint32_t fake_io_rgbLed_disable_callCount(void)
{
    return io_rgbLed_disable_call_count;
}

uint32_t fake_io_rgbLed_disable_callCountForArgs(
    const RgbLed* arg0
)
{
    // Create an instance of this function's parameters.
    io_rgbLed_disable_Params params(
        arg0
    );

    // Return call count from map.
    if(io_rgbLed_disable_call_count_map.count(params)) {
        return io_rgbLed_disable_call_count_map[params];
    }
    else {
        return 0;
    }
}

