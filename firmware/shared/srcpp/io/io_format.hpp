#pragma once

#include "io_log.hpp"
#include <cstdint>

namespace io {

/**
 * Lightweight formatting without linking svfprintf
 * Trades flexibility for minimal code size
 */

// Simple uint to string without snprintf
inline void log_error_handler(const char *file, int line, const char *func)
{
    // Avoid snprintf - pass values directly to RTT
    // Format: "[filename]:[line] [func]: Called Error_Handler()."
    LOG_ERROR("Called Error_Handler()");
    LOG_ERROR("  at %s:%d in %s", file, line, func);
}

// Simple uint to string for assertions
inline void log_assert_failed(const char *file, int line, const char *func, const char *failedexpr)
{
    LOG_ERROR("Assertion failed: %s", failedexpr);
    LOG_ERROR("  at %s:%d in %s", file, line, func);
}

// Simple path formatting for logging (avoids integer formatting)
inline void log_path_info(const char *prefix, uint32_t bootcount)
{
    LOG_INFO("Logging to: %s_%03lu", prefix, bootcount);
}

} // namespace io

