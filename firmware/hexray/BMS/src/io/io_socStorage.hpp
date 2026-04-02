#pragma once

namespace io::socStorage
{

/**
 * Initialize SoC persistence storage using the filesystem layer.
 */
void init();

/**
 * Return whether or not SoC persistence storage is available.
 */
bool isAvailable();

/**
 * Read the latest stored SoC value.
 * @param saved_soc_percent output SoC value in percent
 * @return true if the read succeeded
 */
bool read(float &saved_soc_percent);

/**
 * Persist the SoC value to filesystem storage.
 * @param soc_percent SoC value in percent
 * @return true if write and sync succeeded
 */
bool write(float soc_percent);

} // namespace io::socStorage

