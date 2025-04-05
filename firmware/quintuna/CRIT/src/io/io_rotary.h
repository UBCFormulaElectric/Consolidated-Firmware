#pragma once

#include <stdint.h>

typedef void (*RotaryCallback)(void);

void    io_rotary_init(void);
int32_t io_rotary_get_position(void);
void    io_rotary_reset(void);
void    io_rotary_setClockwiseCallback(RotaryCallback cb);
void    io_rotary_setCounterClockwiseCallback(RotaryCallback cb);
void    io_rotary_setPushCallback(RotaryCallback cb);
void    io_roatary_rotA_IRQHandler(void);
void    io_rotary_rotB_IRQHandler(void);
void    io_rotary_push_IRQHandler(void);
