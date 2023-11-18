#pragma once

#include <stdint.h>

extern "C"
{
#include "test_header.h"
}

// 
// Fakes for: voidFuncNoParams
//

/**
 * Reset the fake for voidFuncNoParams.
 */
void fake_voidFuncNoParams_reset(void);

/**
 * Return the number of times voidFuncNoParams has been invoked.
 */
uint32_t fake_voidFuncNoParams_callCount(void);

// 
// Fakes for: valueReturningFuncNoParams
//

/**
 * Reset the fake for valueReturningFuncNoParams.
 */
void fake_valueReturningFuncNoParams_reset(void);

/**
 * Set the value to be returned when valueReturningFuncNoParams is invoked.
 */
void fake_valueReturningFuncNoParams_returns(int return_value);

/**
 * Return the number of times valueReturningFuncNoParams has been invoked.
 */
uint32_t fake_valueReturningFuncNoParams_callCount(void);

// 
// Fakes for: voidFuncParams
//

/**
 * Reset the fake for voidFuncParams.
 */
void fake_voidFuncParams_reset(void);

/**
 * Return the number of times voidFuncParams has been invoked.
 */
uint32_t fake_voidFuncParams_callCount(void);

/**
 * Return the number of times voidFuncParams has been invoked with a specific set of arguments.
 */
uint32_t fake_voidFuncParams_callCountForArgs(
    int arg0, 
    float arg1, 
    char arg2
);

// 
// Fakes for: valueReturningFuncParams
//

/**
 * Reset the fake for valueReturningFuncParams.
 */
void fake_valueReturningFuncParams_reset(void);

/**
 * Set the value to be returned by valueReturningFuncParams when invoked with a specific set of arguments.
 */
void fake_valueReturningFuncParams_returnsForArgs(
    int arg0, 
    float arg1,
    int return_value
);

/**
 * Return the number of times valueReturningFuncParams has been invoked.
 */
uint32_t fake_valueReturningFuncParams_callCount(void);

/**
 * Return the number of times valueReturningFuncParams has been invoked with a specific set of arguments.
 */
uint32_t fake_valueReturningFuncParams_callCountForArgs(
    int arg0, 
    float arg1
);

