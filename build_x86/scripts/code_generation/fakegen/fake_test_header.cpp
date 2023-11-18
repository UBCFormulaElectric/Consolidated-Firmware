#include "fake_test_header.hpp"
#include <string.h>
#include <unordered_map>

extern "C"
{
#include "test_header.h"
}

using namespace std;

// 
// Fakes for: voidFuncNoParams
//

static uint32_t voidFuncNoParams_call_count;

extern "C" 
{
    void voidFuncNoParams(void)
    {
        // Increment call count.
        voidFuncNoParams_call_count++;
    }
}

void fake_voidFuncNoParams_reset() 
{
    voidFuncNoParams_call_count = 0;
}

uint32_t fake_voidFuncNoParams_callCount(void)
{
    return voidFuncNoParams_call_count;
}

// 
// Fakes for: valueReturningFuncNoParams
//

static int valueReturningFuncNoParams_return_value;
static uint32_t valueReturningFuncNoParams_call_count;

extern "C" 
{
    // Faked void function with no params.
    int valueReturningFuncNoParams(void)
    {
        // Increment call count.
        valueReturningFuncNoParams_call_count++;

        // Return custom return value.
        return valueReturningFuncNoParams_return_value;
    }
}

void fake_valueReturningFuncNoParams_returns(int return_value)
{
    valueReturningFuncNoParams_return_value = return_value;
}

void fake_valueReturningFuncNoParams_reset() 
{
    memset(&valueReturningFuncNoParams_return_value, 0U, sizeof(int));
    valueReturningFuncNoParams_call_count = 0;
}

uint32_t fake_valueReturningFuncNoParams_callCount(void)
{
    return valueReturningFuncNoParams_call_count;
}

// 
// Fakes for: voidFuncParams
//

// Class representing the parameters for the function being faked.
class voidFuncParams_Params
{
  public:
    int arg0;
    float arg1;
    char arg2;

    voidFuncParams_Params(
        int arg0, 
        float arg1, 
        char arg2
    ) :
        arg0(arg0), 
        arg1(arg1), 
        arg2(arg2) { }

    bool operator==(const voidFuncParams_Params &other) const
    {
        return
            arg0 == other.arg0 &&
            arg1 == other.arg1 &&
            arg2 == other.arg2;
    }
};

// Create a custom hashing function so we can use unordered_map (a hash table under the hood).
template <>
struct std::hash<voidFuncParams_Params>
{
    std::size_t operator()(const voidFuncParams_Params &params) const
    {
        return
            hash<int>()(params.arg0) ^
            hash<float>()(params.arg1) ^
            hash<char>()(params.arg2);
    }
};

// State variables.
static uint32_t voidFuncParams_call_count;
static unordered_map<voidFuncParams_Params, uint32_t> voidFuncParams_call_count_map;

extern "C" 
{
    void voidFuncParams(
        int arg0, 
        float arg1, 
        char arg2
    )
    {
        // Create an instance of this function's parameters.
        voidFuncParams_Params params(
            arg0, 
            arg1, 
            arg2
        );

        // Increment call count.
        voidFuncParams_call_count++;
        if(voidFuncParams_call_count_map.count(params)) {
            voidFuncParams_call_count_map[params]++;
        }
        else {
            voidFuncParams_call_count_map[params] = 1;
        }
    }
}

void fake_voidFuncParams_reset() 
{
    voidFuncParams_call_count = 0;
    voidFuncParams_call_count_map.clear();
}

uint32_t fake_voidFuncParams_callCount(void)
{
    return voidFuncParams_call_count;
}

uint32_t fake_voidFuncParams_callCountForArgs(
    int arg0, 
    float arg1, 
    char arg2
)
{
    // Create an instance of this function's parameters.
    voidFuncParams_Params params(
        arg0, 
        arg1, 
        arg2
    );

    // Return call count from map.
    if(voidFuncParams_call_count_map.count(params)) {
        return voidFuncParams_call_count_map[params];
    }
    else {
        return 0;
    }
}

// 
// Fakes for: valueReturningFuncParams
//

// Class representing the parameters for the function being faked.
class valueReturningFuncParams_Params
{
  public:
    int arg0;
    float arg1;

    valueReturningFuncParams_Params(
        int arg0, 
        float arg1
    ) :
        arg0(arg0), 
        arg1(arg1) { }

    bool operator==(const valueReturningFuncParams_Params &other) const
    {
        return
            arg0 == other.arg0 &&
            arg1 == other.arg1;
    }
};

// Create a custom hashing function so we can use unordered_map (a hash table under the hood).
template <>
struct std::hash<valueReturningFuncParams_Params>
{
    std::size_t operator()(const valueReturningFuncParams_Params &params) const
    {
        return
            hash<int>()(params.arg0) ^
            hash<float>()(params.arg1);
    }
};

// State variables.
static unordered_map<valueReturningFuncParams_Params, int> valueReturningFuncParams_return_value_map;
static uint32_t valueReturningFuncParams_call_count;
static unordered_map<valueReturningFuncParams_Params, uint32_t> valueReturningFuncParams_call_count_map;

extern "C"
{
    int valueReturningFuncParams(
        int arg0, 
        float arg1
    )
    {
        // Create an instance of this function's parameters.
        valueReturningFuncParams_Params params(
            arg0, 
            arg1
        );

        // Increment call count.
        valueReturningFuncParams_call_count++;
        if(valueReturningFuncParams_call_count_map.count(params)) {
            valueReturningFuncParams_call_count_map[params]++;
        }
        else {
            valueReturningFuncParams_call_count_map[params] = 1;
        }

        // Return custom return value.
        if(valueReturningFuncParams_return_value_map.count(params)) {
            return valueReturningFuncParams_return_value_map[params];
        }
        else {
            int default_return_value;
            memset(&default_return_value, 0U, sizeof(int));
            return default_return_value;
        }
    }
}

void fake_valueReturningFuncParams_reset() 
{
    valueReturningFuncParams_return_value_map.clear();
    valueReturningFuncParams_call_count = 0;
    valueReturningFuncParams_call_count_map.clear();
}

void fake_valueReturningFuncParams_returnsForArgs(
    int arg0, 
    float arg1,
    int return_value
)
{
    // Create an instance of this function's parameters.
    valueReturningFuncParams_Params params(
        arg0, 
        arg1
    );

    // Update return value map.
    valueReturningFuncParams_return_value_map[params] = return_value;
}

uint32_t fake_valueReturningFuncParams_callCount(void)
{
    return valueReturningFuncParams_call_count;
}

uint32_t fake_valueReturningFuncParams_callCountForArgs(
    int arg0, 
    float arg1
)
{
    // Create an instance of this function's parameters.
    valueReturningFuncParams_Params params(
        arg0, 
        arg1
    );

    // Return call count from map.
    if(valueReturningFuncParams_call_count_map.count(params)) {
        return valueReturningFuncParams_call_count_map[params];
    }
    else {
        return 0;
    }
}

