#pragma once

#include <stdbool.h>

/**
 * Initialize SoC persistence storage using the shared filesystem layer.
 */
void io_socStorage_init(void);

/**
 * Return whether or not SoC persistence storage is available.
 * This checks both media presence and module health state.
 */
bool io_socStorage_isAvailable(void);

/**
 * Read the latest stored SoC value.
 * Integrity checking is handled by the filesystem layer.
 * @param saved_soc_c output SoC value in coulombs
 * @return true if the read succeeded
 */
bool io_socStorage_read(float *saved_soc_c);

/**
 * Persist the SoC value to filesystem storage.
 * @param soc SoC value in coulombs
 * @return true if write and sync succeeded
 */
bool io_socStorage_write(float soc);
