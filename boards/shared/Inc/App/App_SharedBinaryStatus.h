#pragma once

#include <stdbool.h>

struct SharedBinaryStatus;

/**
 * Allocate and initialize a binary status indicator
 * @param is_status_active A function that returns a boolean indicating whether
 * the binary status is active or inactive
 * @return The created binary status, whose ownership is given to the caller
 */
struct SharedBinaryStatus *
    App_SharedBinaryStatus_Create(bool (*is_status_active)(void));

/**
 * Deallocate the memory used by the given binary status
 * @param shared_binary_status The binary status to deallocate
 */
void App_SharedBinaryStatus_Destroy(
    struct SharedBinaryStatus *shared_binary_status);

/**
 * Check if the given binary status is active
 * @param shared_binary_status The binary status to check if it is active
 * @return True if the given binary status is turned on, else false
 */
bool App_SharedBinaryStatus_IsStatusActive(
    const struct SharedBinaryStatus *const shared_binary_status);
