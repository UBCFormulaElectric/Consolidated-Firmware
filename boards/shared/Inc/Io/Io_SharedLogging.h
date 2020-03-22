#pragma once

#include <SEGGER_RTT.h>

/**
 * @brief  Printf-style function to print data to the terminal using Segger RTT
 * @param  sFormat Pointer to format string, followed by the arguments for
 *         conversion
 */
void Io_SharedLogging_Printf(const char *sFormat, ...);
