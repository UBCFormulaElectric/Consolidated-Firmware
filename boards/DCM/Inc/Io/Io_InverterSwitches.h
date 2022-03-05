#pragma once

#include <stdbool.h>

/**
 * Turn on LV (+12V) to the left inverter.
 */
void Io_InverterSwitches_TurnOnLeft();

/**
 * Turn off LV (+12V) to the left inverter.
 */
void Io_InverterSwitches_TurnOffLeft();

/**
 * Turn on LV (+12V) to the right inverter.
 */
void Io_InverterSwitches_TurnOnRight();

/**
 * Turn off LV (+12V) to the right inverter.
 */
void Io_InverterSwitches_TurnOffRight();

/**
 * Check if the right inverter LV switch is on.
 * @return true if the right inverter LV switch is on, false otherwise.
 */
bool Io_InverterSwitches_IsRightInverterOn();

/**
 * Check if the left inverter LV switch is on.
 * @return true if the left inverter LV switch is on, false otherwise.
 */
bool Io_InverterSwitches_IsLeftInverterOn();
