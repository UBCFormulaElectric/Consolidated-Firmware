#pragma once

#include <stdint.h>
#include "App_ErrorCode.h"

struct RotarySwitch;

struct RotarySwitch *App_RotarySwitch_Create(
    uint32_t (*get_position)(void),
    uint32_t num_positions);
void     App_RotarySwitch_Destroy(struct RotarySwitch *rotary_switch);
ExitCode App_RotarySwitch_GetPosition(
    const struct RotarySwitch *rotary_switch,
    uint32_t *                 returned_position);
