#pragma once

#include <stdint.h>
#include "App_SharedExitCode.h"

/**
 * Allocate and initialize a regen paddle
 * @param get_raw_paddle_position A function that can be called to get the raw
 *                                paddle position, where 0 means not pressed at
 *                                all and 100 means pressed all the way. Any
 *                                raw paddle position outside of this range is
 *                                considered invalid.
 * @param lower_deadzone Any raw paddle position >= 0 and less than or equal to
 *                       this value will map to a regen value of 0
 * @param upper_deadzone Any raw paddle position <= 100 and greater than or
 *                       equal to this value will map to a regen value of 100
 * @return The created regen paddle, whose ownership is given to the caller
 */
struct RegenPaddle *
    App_RegenPaddle_Create(uint32_t (*get_raw_paddle_position)(void), uint32_t lower_deadzone, uint32_t upper_deadzone);

/**
 * Deallocate the memory used by the given regen paddle
 * @param regen_paddle The regen paddle to deallocate
 */
void App_RegenPaddle_Destroy(struct RegenPaddle *regen_paddle);

/**
 * Get the raw paddle position for the given regen paddle, where 0 means not
 * pressed at all and 100 means pressed all the way
 * @param regen_paddle The regen paddle to get raw paddle position for
 * @param returned_raw_paddle_position This will be set to the raw paddle
 *                                     position
 * @return ERROR_CODE_OUT_OF_RANGE If we read a raw paddle position that is not
 *                                 between 0 to 100 inclusive
 */
ExitCode App_RegenPaddle_GetRawPaddlePosition(
    const struct RegenPaddle *regen_paddle,
    uint32_t *                returned_raw_paddle_position);

/**
 * Get the mapped paddle position for the given regen paddle, where 0 means no
 * regen and 100 means maximum regen
 * @param regen_paddle The regen paddle to get mapped paddle position for
 * @param returned_mapped_paddle_position This will be set to the mapped paddle
 *                                        position
 * @return ERROR_CODE_OUT_OF_RANGE If we read a raw paddle position that is not
 *                                 between 0 to 100 inclusive
 */
ExitCode App_RegenPaddle_GetMappedPaddlePosition(
    const struct RegenPaddle *regen_paddle,
    uint32_t *                returned_mapped_paddle_position);
