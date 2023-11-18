#pragma once

#include <stdint.h>

extern "C"
{
#include "io_sevenSegDisplays.h"
}

// 
// Fakes for: io_sevenSegDisplays_init
//

/**
 * Reset the fake for io_sevenSegDisplays_init.
 */
void fake_io_sevenSegDisplays_init_reset(void);

/**
 * Return the number of times io_sevenSegDisplays_init has been invoked.
 */
uint32_t fake_io_sevenSegDisplays_init_callCount(void);

/**
 * Return the number of times io_sevenSegDisplays_init has been invoked with a specific set of arguments.
 */
uint32_t fake_io_sevenSegDisplays_init_callCountForArgs(
    const SevenSegsConfig* arg0
);

// 
// Fakes for: io_sevenSegDisplays_writeCommands
//

/**
 * Reset the fake for io_sevenSegDisplays_writeCommands.
 */
void fake_io_sevenSegDisplays_writeCommands_reset(void);

/**
 * Return the number of times io_sevenSegDisplays_writeCommands has been invoked.
 */
uint32_t fake_io_sevenSegDisplays_writeCommands_callCount(void);

// 
// Fakes for: io_sevenSegDisplays_setValue
//

/**
 * Reset the fake for io_sevenSegDisplays_setValue.
 */
void fake_io_sevenSegDisplays_setValue_reset(void);

/**
 * Return the number of times io_sevenSegDisplays_setValue has been invoked.
 */
uint32_t fake_io_sevenSegDisplays_setValue_callCount(void);

/**
 * Return the number of times io_sevenSegDisplays_setValue has been invoked with a specific set of arguments.
 */
uint32_t fake_io_sevenSegDisplays_setValue_callCountForArgs(
    SevenSegGroup arg0, 
    SevenSegSubposition arg1, 
    HexDigit arg2, 
    bool arg3
);

// 
// Fakes for: io_sevenSegDisplays_disable
//

/**
 * Reset the fake for io_sevenSegDisplays_disable.
 */
void fake_io_sevenSegDisplays_disable_reset(void);

/**
 * Return the number of times io_sevenSegDisplays_disable has been invoked.
 */
uint32_t fake_io_sevenSegDisplays_disable_callCount(void);

/**
 * Return the number of times io_sevenSegDisplays_disable has been invoked with a specific set of arguments.
 */
uint32_t fake_io_sevenSegDisplays_disable_callCountForArgs(
    SevenSegGroup arg0, 
    SevenSegSubposition arg1
);

