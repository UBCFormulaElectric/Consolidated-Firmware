#pragma once

#include <stdbool.h>

struct BinaryStatus;

/**
 * Allocate and initialize a binary status
 * @param is_status_active A function that returns a boolean indicating whether the binary status is active or inactive
 * @return The created binary status, whose ownership is given to the caller
 */
struct BinaryStatus *App_BinaryStatus_Create(bool (*is_status_active)(void));

/**
 * Deallocate the memory used by the given binary status
 * @param binary_status The binary status to deallocate
 */
void App_BinaryStatus_Destroy(struct BinaryStatus *binary_status);

/**
 * Check if the given binary status is active
 * @param binary_status The binary status to check if it is active
 * @return True if the given binary status is turned on, else false
 */
bool App_BinaryStatus_IsStatusActive(
    const struct BinaryStatus *const binary_status);
