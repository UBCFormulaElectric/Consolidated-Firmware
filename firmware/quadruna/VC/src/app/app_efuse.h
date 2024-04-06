#pragma once
#include <stdbool.h>
#include "io_efuse.h"

void app_efuse_init(
    void (*enabled_setters[NUM_EFUSE_CHANNELS])(bool),
    void (*current_setters[NUM_EFUSE_CHANNELS])(float));

void app_efuse_broadcast(void);