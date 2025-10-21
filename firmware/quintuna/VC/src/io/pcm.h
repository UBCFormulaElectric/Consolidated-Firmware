#pragma once
#include <stdbool.h>

/**
 * Enable the PCM.
 * @param enable Whether or not to enable the PCM.
 */
void io_pcm_set(bool enable);

/**
 * Toggle the PCM.
 */
void io_pcm_toggle();

/**
 * Check if the PCM is enabled.
 * @return true if the PCM is enabled, false otherwise.
 */
bool io_pcm_enabled();