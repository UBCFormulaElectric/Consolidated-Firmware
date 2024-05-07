#pragma once

#include <stdbool.h>
#include "app_utils.h"

#ifdef TARGET_EMBEDDED
#include "hw_gpio.h"

typedef struct
{
    const Gpio sd_present;
} SdGpio;
#else
EMPTY_STRUCT(SdGpio);
#endif

/**
 * @brief Initialize SD Gpio struct.
 * @param sdGpio SD GPIO struct.
 */
void io_sdGpio_init(const SdGpio *sdGpio);

/**
 * Return whether or not a SD Card is present
 * @param SdGpio instance
 * @return true if SD card is present
 */
bool io_sdGpio_checkSdPresent();