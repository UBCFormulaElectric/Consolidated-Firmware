#include "fake_io_sevenSegDisplays.hpp"
#include <string.h>
#include <unordered_map>

extern "C"
{
#include "io_sevenSegDisplays.h"
}

using namespace std;

// 
// Fakes for: io_sevenSegDisplays_init
//

// Class representing the parameters for the function being faked.
class io_sevenSegDisplays_init_Params
{
  public:
    const SevenSegsConfig* arg0;

    io_sevenSegDisplays_init_Params(
        const SevenSegsConfig* arg0
    ) :
        arg0(arg0) { }

    bool operator==(const io_sevenSegDisplays_init_Params &other) const
    {
        return
            arg0 == other.arg0;
    }
};

// Create a custom hashing function so we can use unordered_map (a hash table under the hood).
template <>
struct std::hash<io_sevenSegDisplays_init_Params>
{
    std::size_t operator()(const io_sevenSegDisplays_init_Params &params) const
    {
        return
            hash<const SevenSegsConfig*>()(params.arg0);
    }
};

// State variables.
static uint32_t io_sevenSegDisplays_init_call_count;
static unordered_map<io_sevenSegDisplays_init_Params, uint32_t> io_sevenSegDisplays_init_call_count_map;

extern "C" 
{
    void io_sevenSegDisplays_init(
        const SevenSegsConfig* arg0
    )
    {
        // Create an instance of this function's parameters.
        io_sevenSegDisplays_init_Params params(
            arg0
        );

        // Increment call count.
        io_sevenSegDisplays_init_call_count++;
        if(io_sevenSegDisplays_init_call_count_map.count(params)) {
            io_sevenSegDisplays_init_call_count_map[params]++;
        }
        else {
            io_sevenSegDisplays_init_call_count_map[params] = 1;
        }
    }
}

void fake_io_sevenSegDisplays_init_reset() 
{
    io_sevenSegDisplays_init_call_count = 0;
    io_sevenSegDisplays_init_call_count_map.clear();
}

uint32_t fake_io_sevenSegDisplays_init_callCount(void)
{
    return io_sevenSegDisplays_init_call_count;
}

uint32_t fake_io_sevenSegDisplays_init_callCountForArgs(
    const SevenSegsConfig* arg0
)
{
    // Create an instance of this function's parameters.
    io_sevenSegDisplays_init_Params params(
        arg0
    );

    // Return call count from map.
    if(io_sevenSegDisplays_init_call_count_map.count(params)) {
        return io_sevenSegDisplays_init_call_count_map[params];
    }
    else {
        return 0;
    }
}

// 
// Fakes for: io_sevenSegDisplays_writeCommands
//

static uint32_t io_sevenSegDisplays_writeCommands_call_count;

extern "C" 
{
    void io_sevenSegDisplays_writeCommands(void)
    {
        // Increment call count.
        io_sevenSegDisplays_writeCommands_call_count++;
    }
}

void fake_io_sevenSegDisplays_writeCommands_reset() 
{
    io_sevenSegDisplays_writeCommands_call_count = 0;
}

uint32_t fake_io_sevenSegDisplays_writeCommands_callCount(void)
{
    return io_sevenSegDisplays_writeCommands_call_count;
}

// 
// Fakes for: io_sevenSegDisplays_setValue
//

// Class representing the parameters for the function being faked.
class io_sevenSegDisplays_setValue_Params
{
  public:
    SevenSegGroup arg0;
    SevenSegSubposition arg1;
    HexDigit arg2;
    bool arg3;

    io_sevenSegDisplays_setValue_Params(
        SevenSegGroup arg0, 
        SevenSegSubposition arg1, 
        HexDigit arg2, 
        bool arg3
    ) :
        arg0(arg0), 
        arg1(arg1), 
        arg2(arg2), 
        arg3(arg3) { }

    bool operator==(const io_sevenSegDisplays_setValue_Params &other) const
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
struct std::hash<io_sevenSegDisplays_setValue_Params>
{
    std::size_t operator()(const io_sevenSegDisplays_setValue_Params &params) const
    {
        return
            hash<SevenSegGroup>()(params.arg0) ^
            hash<SevenSegSubposition>()(params.arg1) ^
            hash<HexDigit>()(params.arg2) ^
            hash<bool>()(params.arg3);
    }
};

// State variables.
static uint32_t io_sevenSegDisplays_setValue_call_count;
static unordered_map<io_sevenSegDisplays_setValue_Params, uint32_t> io_sevenSegDisplays_setValue_call_count_map;

extern "C" 
{
    void io_sevenSegDisplays_setValue(
        SevenSegGroup arg0, 
        SevenSegSubposition arg1, 
        HexDigit arg2, 
        bool arg3
    )
    {
        // Create an instance of this function's parameters.
        io_sevenSegDisplays_setValue_Params params(
            arg0, 
            arg1, 
            arg2, 
            arg3
        );

        // Increment call count.
        io_sevenSegDisplays_setValue_call_count++;
        if(io_sevenSegDisplays_setValue_call_count_map.count(params)) {
            io_sevenSegDisplays_setValue_call_count_map[params]++;
        }
        else {
            io_sevenSegDisplays_setValue_call_count_map[params] = 1;
        }
    }
}

void fake_io_sevenSegDisplays_setValue_reset() 
{
    io_sevenSegDisplays_setValue_call_count = 0;
    io_sevenSegDisplays_setValue_call_count_map.clear();
}

uint32_t fake_io_sevenSegDisplays_setValue_callCount(void)
{
    return io_sevenSegDisplays_setValue_call_count;
}

uint32_t fake_io_sevenSegDisplays_setValue_callCountForArgs(
    SevenSegGroup arg0, 
    SevenSegSubposition arg1, 
    HexDigit arg2, 
    bool arg3
)
{
    // Create an instance of this function's parameters.
    io_sevenSegDisplays_setValue_Params params(
        arg0, 
        arg1, 
        arg2, 
        arg3
    );

    // Return call count from map.
    if(io_sevenSegDisplays_setValue_call_count_map.count(params)) {
        return io_sevenSegDisplays_setValue_call_count_map[params];
    }
    else {
        return 0;
    }
}

// 
// Fakes for: io_sevenSegDisplays_disable
//

// Class representing the parameters for the function being faked.
class io_sevenSegDisplays_disable_Params
{
  public:
    SevenSegGroup arg0;
    SevenSegSubposition arg1;

    io_sevenSegDisplays_disable_Params(
        SevenSegGroup arg0, 
        SevenSegSubposition arg1
    ) :
        arg0(arg0), 
        arg1(arg1) { }

    bool operator==(const io_sevenSegDisplays_disable_Params &other) const
    {
        return
            arg0 == other.arg0 &&
            arg1 == other.arg1;
    }
};

// Create a custom hashing function so we can use unordered_map (a hash table under the hood).
template <>
struct std::hash<io_sevenSegDisplays_disable_Params>
{
    std::size_t operator()(const io_sevenSegDisplays_disable_Params &params) const
    {
        return
            hash<SevenSegGroup>()(params.arg0) ^
            hash<SevenSegSubposition>()(params.arg1);
    }
};

// State variables.
static uint32_t io_sevenSegDisplays_disable_call_count;
static unordered_map<io_sevenSegDisplays_disable_Params, uint32_t> io_sevenSegDisplays_disable_call_count_map;

extern "C" 
{
    void io_sevenSegDisplays_disable(
        SevenSegGroup arg0, 
        SevenSegSubposition arg1
    )
    {
        // Create an instance of this function's parameters.
        io_sevenSegDisplays_disable_Params params(
            arg0, 
            arg1
        );

        // Increment call count.
        io_sevenSegDisplays_disable_call_count++;
        if(io_sevenSegDisplays_disable_call_count_map.count(params)) {
            io_sevenSegDisplays_disable_call_count_map[params]++;
        }
        else {
            io_sevenSegDisplays_disable_call_count_map[params] = 1;
        }
    }
}

void fake_io_sevenSegDisplays_disable_reset() 
{
    io_sevenSegDisplays_disable_call_count = 0;
    io_sevenSegDisplays_disable_call_count_map.clear();
}

uint32_t fake_io_sevenSegDisplays_disable_callCount(void)
{
    return io_sevenSegDisplays_disable_call_count;
}

uint32_t fake_io_sevenSegDisplays_disable_callCountForArgs(
    SevenSegGroup arg0, 
    SevenSegSubposition arg1
)
{
    // Create an instance of this function's parameters.
    io_sevenSegDisplays_disable_Params params(
        arg0, 
        arg1
    );

    // Return call count from map.
    if(io_sevenSegDisplays_disable_call_count_map.count(params)) {
        return io_sevenSegDisplays_disable_call_count_map[params];
    }
    else {
        return 0;
    }
}

