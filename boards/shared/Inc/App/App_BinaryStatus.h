#pragma once

#include <stdbool.h>

struct BinaryStatus;

struct BinaryStatus *App_BinaryStatus_Create(bool (*is_status_active)(void));

void App_BinaryStatus_Destroy(struct BinaryStatus *binary_status);

bool App_BinaryStatus_IsStatusActive(
    const struct BinaryStatus *const binary_status);
