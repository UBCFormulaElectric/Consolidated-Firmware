#pragma once

namespace io::pcm
{

/**
 * Enable the PCM.
 * @param enable Whether or not to enable the PCM.
 */
void set(bool enable);

/**
 * Check if the PCM is enabled.
 * @return true if the PCM is enabled, false otherwise.
 */
bool enabled();

}; // namespace io::pcm