#pragma once

#include <stdbool.h>

/**
 * Initialize buzzer module.
 */
void App_Buzzer_Init();

/**
 * Turn on the given buzzer
 */
void App_Buzzer_TurnOn();

/**
 * Turn off the given buzzer
 */
void App_Buzzer_TurnOff();

/**
 * Check if the given buzzer is turned on
 * @return true if the buzzer is on, false if it is not
 */
bool App_Buzzer_IsOn();
