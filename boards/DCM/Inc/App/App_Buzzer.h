#pragma once

struct Buzzer;

/**
 * Allocate and initialize a buzzer
 * @param turn_on A function that can be called to turn on the buzzer
 * @param turn_off A function that can be called to turn off the buzzer
 * @return A pointer to the created buzzer, whose ownership is given to the
 *         caller
 */
struct Buzzer *App_Buzzer_Create(void (*turn_on)(void), void (*turn_off)(void));

/**
 * Deallocate the memory used by the given buzzer
 * @param buzzer The buzzer to deallocate
 */
void App_Buzzer_Destroy(struct Buzzer *buzzer);

/**
 * Turn on the given buzzer
 * @param buzzer The buzzer to turn on
 */
void App_Buzzer_TurnOn(const struct Buzzer *buzzer);

/**
 * Turn off the given buzzer
 * @param buzzer The buzzer to turn off
 */
void App_Buzzer_TurnOff(const struct Buzzer *buzzer);
