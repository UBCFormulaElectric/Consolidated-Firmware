#pragma once

#include <stdbool.h>
#include "App_SharedExitCode.h"

/**
 * Disable BMS_OK
 * @return EXIT_CODE_UNIMPLEMENTED if we can't disable BMS_OK
 */
ExitCode Io_OkStatuses_EnableBmsOk(void);

/**
 * Enable BMS_OK
 * @return EXIT_CODE_UNIMPLEMENTED if we can't disable BMS_OK
 */
ExitCode Io_OkStatuses_DisableBmsOk(void);

/**
 * Check if BMS_OK is enabled
 * @return true if BMS_OK is enable, else false
 */
bool Io_OkStatuses_IsBmsOkEnabled(void);

/**
 * Disable IMD_OK
 * @return EXIT_CODE_UNIMPLEMENTED if we can't disable IMD_OK
 */
ExitCode Io_OkStatuses_EnableImdOk(void);

/**
 * Enable IMD_OK
 * @return EXIT_CODE_UNIMPLEMENTED if we can't disable IMD_OK
 */
ExitCode Io_OkStatuses_DisableImdOk(void);

/**
 * Check if IMD_OK is enabled
 * @return true if IMD_OK is enable, else false
 */
bool Io_OkStatuses_IsImdOkEnabled(void);

/**
 * Disable BSPD_OK
 * @return EXIT_CODE_UNIMPLEMENTED if we can't disable BSPD_OK
 */
ExitCode Io_OkStatuses_EnableBspdOk(void);

/**
 * Enable BSPD_OK
 * @return EXIT_CODE_UNIMPLEMENTED if we can't disable BSPD_OK
 */
ExitCode Io_OkStatuses_DisableBspdOk(void);

/**
 * Check if BSPD_OK is enabled
 * @return true if BSPD_OK is enable, else false
 */
bool Io_OkStatuses_IsBspdOkEnabled(void);
