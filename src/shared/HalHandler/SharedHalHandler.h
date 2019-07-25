/**
 * @file  SharedHalHandler.h
 * @brief Shared library with callback function for HAL library
 * @note  Include this header file in main.h, above the auto-generated function
 *        prototype `void ErrorHandler(void)`. Then, fill in the function
 *        definition in main.c as if the function prototype is
 *        `void ErrorHandler(char *file, uint32_t line)`
 */
#ifndef SHARED_HAL_HANDLER_H
#define SHARED_HAL_HANDLER_H

/******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdint.h>

/******************************************************************************
 * Preprocessor Macros
 ******************************************************************************/
// clang-format off
// __FILE__ contains the absolute path whereas __FILENAME__ contains the base
// filename. __FILENAME__ is preferred over __FILE__ to provide a cleaner
// debugging output in RTT viewer.
#define __FILENAME__                                                         \
    (__builtin_strrchr(__FILE__, '/') ? __builtin_strrchr(__FILE__, '/') + 1 \
                                      : __FILE__)

// This is a "fix" taken from the ST forum (shorturl.at/iqDKV) to 
//
// 1. Replace "Error_Handler(void)" with "Error_Handler(char * file, uint32_t line)"
// 2. Replace calls to "Error_Handler()" with "Error_Handler(__FILENAME__, __LINE__)"
//
// This allows us to pass in file and line number to the callback functions,
// and transmit them over the debugging interface (Segger RTT).
#define GET_MACRO(_0, _1, NAME, ...) NAME
#define Error_Handler(...) \
    GET_MACRO(_0, ##__VA_ARGS__, Error_Handler1, Error_Handler0)()
#define Error_Handler0() _Error_Handler(__FILENAME__, __LINE__)
#define Error_Handler1(unused) _Error_Handler(char *file, uint32_t line)
void _Error_Handler(char *, uint32_t);
// clang-format on
//
/******************************************************************************
 * Function Prototypes
 ******************************************************************************/
/**
 * @brief Callback function for HAL library's Error_Handler()
 * @param file: pointer to the source file name.
 * @param line: assert_param error line source number.
 */
void SharedHalHandler_ErrorHandler(char *file, uint32_t line);

/**
 * @brief Callback function for HAL library's assert_failed()
 * @param file: pointer to the source file name.
 * @param line: assert_param error line source number.
 */
void SharedHalHandler_AssertFailed(char *file, uint32_t line);

#endif // SHARED_HAL_HANDLER_H
