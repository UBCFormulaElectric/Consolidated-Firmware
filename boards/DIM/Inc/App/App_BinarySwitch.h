#pragma once

#include <stdbool.h>

struct BinarySwitch;

struct BinarySwitch *App_BinarySwitch_Create(bool (*is_turned_on)(void));
bool App_BinarySwitch_IsTurnedOn(const struct BinarySwitch *binary_switch);
void App_BinarySwitch_Destroy(struct BinarySwitch *binary_switch);
