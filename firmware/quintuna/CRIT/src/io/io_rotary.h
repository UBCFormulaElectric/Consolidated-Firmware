#pragma once

typedef void (*RotaryCallback)(void);

void io_rotary_init(void);
void io_rotary_setClockwiseCallback(RotaryCallback cb);
void io_rotary_setCounterClockwiseCallback(RotaryCallback cb);
void io_rotary_setPushCallback(RotaryCallback cb);
void io_rotary_rotA_rotB_IRQHandler(void);
void io_rotary_push_IRQHandler(void);
