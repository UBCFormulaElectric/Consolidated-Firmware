#pragma once

#include <stdbool.h>
#include "App_SharedExitCode.h"

struct OkStatus;

/**
 * Allocate and initialize a OK status
 * @param enable A function that can be called to enable the OK status
 * @param disable A function that can be called to disable the OK status
 * @param is_enabled A function that returns whether the OK status is enabled
 *                   or not
 * @return The created OK status, whose ownership is given to the caller
 */
struct OkStatus *App_OkStatus_Create(ExitCode (*enable)(void), ExitCode (*disable)(void), bool (*is_enabled)(void));

/**
 * Deallocate the memory used by the given OK status
 * @param ok_status The OK status to deallocate
 */
void App_OkStatus_Destroy(struct OkStatus *ok_status);

/**
 * Enable the given OK status
 * @param ok_status The OK status to enable
 * @return EXIT_CODE_UNIMPLEMENTED if the given OK status can't be enabled
 */
ExitCode App_OkStatus_Enable(struct OkStatus *ok_status);

/**
 * Disable the given OK status
 * @param ok_status The OK status to disable
 * @return EXIT_CODE_UNIMPLEMENTED if the given OK status can't be disabled
 */
ExitCode App_OkStatus_Disable(struct OkStatus *ok_status);

/**
 * Check if the given OK status is enabled
 * @param ok_status The OK status to check
 * @return true if the OK status is enabled, else false
 */
bool App_OkStatus_IsEnabled(const struct OkStatus *ok_status);
