#pragma once

#include <stdbool.h>

/**
 * Check if the encoder alarm for the PAPPS is active
 * @return true if the encoder alarm for the PAPPS is active, else false
 */
bool Io_AcceleratorPedals_IsPappsEncoderAlarmActive(void);

/**
 * Check if the encoder alarm for the SAPPS is active
 * @return true if the encoder alarm for the SAPPS is active, else false
 */
bool Io_AcceleratorPedals_IsSappsEncoderAlarmActive(void);
