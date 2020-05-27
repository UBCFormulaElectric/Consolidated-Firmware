#pragma once

struct Buzzer;

/**
 * Allocate and initialize a buzzer
 * @param on A function that can be called to turn on the buzzer
 * @param off A function that can be called to turn off the buzzer
 * @return A pointer to the created buzzer, whose ownership is given to the
 *         caller
 */
struct Buzzer *App_Buzzer_Create(void (*on)(void), void (*off)(void));

/**
 * Deallocate the memory used by the given buzzer
 * @param buzzer The buzzer to deallocate
 */
void App_Buzzer_Destroy(struct Buzzer *buzzer);

/**
 * Turn on the given buzzer
 * @param buzzer The buzzer to turn on
 */
void App_Buzzer_On(const struct Buzzer *buzzer);

/**
 * Turn off the given buzzer
 * @param buzzer The buzzer to turn off
 */
void App_Buzzer_Off(const struct Buzzer *buzzer);
