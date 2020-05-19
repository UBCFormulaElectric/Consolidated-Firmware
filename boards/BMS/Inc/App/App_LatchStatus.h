#pragma once

#include <stdbool.h>

struct LatchStatus;

struct LatchStatus *App_LatchStatus_Create(bool (*is_enabled)(void));

bool App_LatchStatus_IsEnabled(const struct LatchStatus *latch);