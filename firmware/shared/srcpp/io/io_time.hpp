#pragma once
#include <cstdint>

namespace io::time
{
/**
 * Get the time passed since startup, in milliseconds.
 * @return Time in ms.
 */
uint32_t getCurrentMs();
void     delay(uint32_t ms);
void     delayUntil(uint32_t time);
} // namespace io::time
