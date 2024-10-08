#pragma once
#include <cstdint>

namespace io::time
{
/**
 * Get the time passed since startup, in milliseconds.
 * @return Time in ms.
 */
uint32_t getCurrentMs();
} // namespace io::time
