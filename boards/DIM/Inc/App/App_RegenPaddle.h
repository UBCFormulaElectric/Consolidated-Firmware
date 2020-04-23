#pragma once

#include <stdint.h>
#include "App_ErrorCode.h"

/**
 * Allocate and initialize a regen paddle
 * @param get_paddle_position A function that can be called to get the paddle
 *                            position as a percentage, where 0 means not
 *                            pressed at all and 100 means pressed all the way
 * @param lower_deadzone The paddle position threshold of lower deadzone
 * @param upper_deadzone The paddle position threshold of upper deadzone
 * @return The created regen paddle, whose ownership is given to the caller
 */
struct RegenPaddle *App_RegenPaddle_Create(
    uint32_t (*get_paddle_position)(void),
    uint32_t lower_deadzone,
    uint32_t upper_deadzone);
void      App_RegenPaddle_Destroy(struct RegenPaddle *regen_paddle);
ErrorCode App_RegenPaddle_Tick(struct RegenPaddle *regen_paddle);
uint32_t  App_RegenPaddle_GetRegen(const struct RegenPaddle *regen_paddle);
uint32_t  App_RegenPaddle_GetPosition(const struct RegenPaddle *regen_paddle);
