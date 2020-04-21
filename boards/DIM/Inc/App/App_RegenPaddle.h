#pragma once

#include <stdint.h>

/**
 * Allocate and initialize a regen paddle
 * @param get_paddle_position A function that can be called to get the paddle
 *                            position as a percentage, where 0% means not
 *                            pressed at all and 100% means pressed all the way
 * @param lower_deadzone The paddle position threshold of lower deadzone
 * @param upper_deadzone The paddle position threshold of upper deadzone
 * @return The created regen paddle, whose ownership is given to the caller
 */
struct RegenPaddle *App_RegenPaddle_Create(
    float (*get_paddle_position)(void),
    float lower_deadzone,
    float upper_deadzone);
void  App_RegenPaddle_Destroy(struct RegenPaddle *regen_paddle);
void  App_RegenPaddle_Tick(struct RegenPaddle *regen_paddle);
float App_RegenPaddle_GetRegen(const struct RegenPaddle *regen_paddle);
