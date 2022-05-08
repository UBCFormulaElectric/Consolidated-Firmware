/**
 * @brief Override the function prototype of Error_Handler()
 * @note  Include this header file in main.h, above the auto-generated function
 *        prototype `void ErrorHandler(void)`. Then, fill in the function
 *        definition in main.c as if the function prototype is
 *        `void ErrorHandler(char *file, uint32_t line)`
 */
#pragma once

#include <stdint.h>
#include "App_SharedMacros.h"

// This is a "hack" taken from the ST forum (shorturl.at/iqDKV) to do the
// following:
//
// - Replace "Error_Handler(void)" with
//           "Error_Handler(char * file, uint32_t line)"
// - Replace calls to "Error_Handler()" with
//                    "Error_Handler(__FILE__, __LINE__)"
//
// This allows us to pass in file and line number to the error
// callback functions and transmit them over the debugging interface
// (e.g. Segger RTT or CAN).
#define GET_MACRO(_0, _1, NAME, ...) NAME
#define Error_Handler(...) GET_MACRO(_0, ##__VA_ARGS__, Error_Handler1, Error_Handler0)()
#define Error_Handler0() _Error_Handler(__FILE__, __LINE__)
#define Error_Handler1(unused) _Error_Handler(char *file, int line)
void _Error_Handler(char *, int);
