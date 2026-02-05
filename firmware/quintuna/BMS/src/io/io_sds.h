#pragma once
#include "cmsis_os.h"
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
bool io_sds_checkSdPresent(void);
bool io_sds_readSocFromSd(float *saved_soc_c);
bool io_sds_writeSocToSd(float soc);
