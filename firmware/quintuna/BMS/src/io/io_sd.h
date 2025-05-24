#pragma once

#include <stdbool.h>

#ifdef TARGET_EMBEDDED
#else
#include "app_utils.h"
EMPTY_STRUCT(SdCard);
#endif

/**
 * Return whether or not a SD Card is present
 * @param SdGpio instance
 * @return true if SD card is present
 */
bool io_sdGpio_checkSdPresent();