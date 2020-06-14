#pragma once

#include <stdbool.h>

/**
 * Check if the encoder alarm for the primary APPS is active
 * @return true if the encoder alarm for the primary APPS is active, else false
 */
bool Io_AcceleratorPedals_IsPappsEncoderAlarmActive(void);

/**
 * Check if the encoder alarm for the secondary APPS is active
 * @return true if the encoder alarm for the secondary APPS is active, else
 *         false
 */
bool Io_AcceleratorPedals_IsSappsEncoderAlarmActive(void);
