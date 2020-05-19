#pragma once

#include <stdbool.h>

/**
 * Disable BMS_OK
 */
void Io_Latches_EnableOnBmsOk(void);

/**
 * Enable BMS_OK
 */
void Io_Latches_DisableBmsOk(void);

/**
 * Check if BMS_OK is enabled
 * @return true if BMS_OK is enable, else false
 */
bool Io_Latches_IsBmsOkEnabled(void);

/**
 * Check if IMD_OK is enabled
 * @return true if IMD_OK is enable, else false
 */
bool Io_Latches_IsImdOkEnabled(void);

/**
 * Check if BSPD_OK is enabled
 * @return true if BSPD_OK is enable, else false
 */
bool Io_Latches_IsBspdOkEnabled(void);
