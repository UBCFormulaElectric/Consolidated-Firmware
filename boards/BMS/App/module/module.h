#pragma once

typedef struct
{
    void (*init)(void);
    void (*tick_1Hz)(void);
    void (*tick_10Hz)(void);
    void (*tick_100Hz)(void);
    void (*tick_1kHz)(void);
} module_s;

void module_1HzTick(void);
void module_10HzTick(void);
void module_100HzTick(void);
void module_1kHzTick(void);
