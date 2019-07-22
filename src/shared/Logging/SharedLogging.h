/**
 * @file  SharedLogging.h
 * @brief Library for logging information
 */

#ifndef SHARED_LOGGING_H
#define SHARED_LOGGING_H
/******************************************************************************
 * Includes
 ******************************************************************************/
#include "SEGGER_RTT.h"

/******************************************************************************
 * Preprocessor Macros
 ******************************************************************************/
// clang-format off

// __FILE__ contains the absolute path whereas __FILENAME__ contains the base
// filename. __FILENAME__ is preferred over __FILE__ to provide a cleaner
// debugging output in RTT viewer.
#define __FILENAME__ (__builtin_strrchr(__FILE__, '/') ? __builtin_strrchr(__FILE__, '/') + 1 : __FILE__)

#define LOG_INFO(fmt, ...) \
    SharedLogging_Printf("[INFO] %s::%s(%u) : " fmt "\n", __FILENAME__, __FUNCTION__, __LINE__, __VA_ARGS__)
#define LOG_ERROR(error_code, fmt, ...) \
    SharedLogging_Printf("[ERROR %u] %s::%s(%u) : " fmt "\n", (ErrorCode_e)error_code, __FILENAME__, __FUNCTION__, __LINE__, __VA_ARGS__)

// clang-format on

/******************************************************************************
 * Function Prototypes
 ******************************************************************************/
/**
 * @brief  Printf-style function to print data over Segger RTT
 * @param  sFormat Pointer to format string, followed by the arguments for
 *         conversion
 */
void SharedLogging_Printf(const char *sFormat, ...);

#endif // SHARED_LOGGING_H
