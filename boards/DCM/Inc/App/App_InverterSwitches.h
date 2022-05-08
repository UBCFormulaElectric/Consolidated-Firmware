#pragma once

/**
 * IMPORTANT! LV (+12 V) to the PM100DZ inverters must only be provided once the
 * AIRs are closed. Otherwise, the inverters will enter the fault state on
 * power-up.
 */

/**
 * Allocate and initialize an InverterSwitches struct
 * @param turn_on_right_inverter A function that can be called to enable LV to
 * the right inverter
 * @param turn_off_right_inverter A function that can be called to disable LV
 * the right inverter
 * @param turn_on_left_inverter A function that can be called to enable LV to
 * the left inverter
 * @param turn_off_left_inverter A function that can be called to disable LV to
 * the left inverter
 * @param is_right_inverter_on A function that can be called to check if the
 * right inverter LV switch is on
 * @param is_left_inverter_on A function that can be called to check if the left
 * inverter LV switch is on
 * @return The created brake light, whose ownership is given to the caller
 */
struct InverterSwitches *App_InverterSwitches_Create(
    void (*turn_on_right_inverter)(void),
    void (*turn_off_right_inverter)(void),
    void (*turn_on_left_inverter)(void),
    void (*turn_off_left_inverter)(void),
    bool (*is_right_inverter_on)(void),
    bool (*is_left_inverter_on)(void));

/**
 * Deallocate the memory used by the given InverterSwitches struct
 * @param inverter_switches The InverterSwitches struct to deallocate
 */
void App_InverterSwitches_Destroy(struct InverterSwitches *inverter_switches);

/**
 * Turn on LV (+12 V) to the right inverter
 * @param inverter_switches The InverterSwitches struct to turn on the right
 * inverter
 */
void App_InverterSwitches_TurnOnRight(struct InverterSwitches *inverter_switches);

/**
 * Turn off LV (+12 V) to the right inverter
 * @param inverter_switches The InverterSwitches struct to turn off the right
 * inverter
 */
void App_InverterSwitches_TurnOffRight(struct InverterSwitches *inverter_switches);

/**
 * Turn on LV (+12 V) to the left inverter
 * @param inverter_switches The InverterSwitches struct to turn on the left
 * inverter
 */
void App_InverterSwitches_TurnOnLeft(struct InverterSwitches *inverter_switches);

/**
 * Turn off LV (+12 V) to the left inverter
 * @param inverter_switches The InverterSwitches struct to turn off the left
 * inverter
 */
void App_InverterSwitches_TurnOffLeft(struct InverterSwitches *inverter_switches);

/**
 * Check if the right inverter's LV switch is on
 * @param inverter_switches The InverterSwitches struct to check the status of
 * the switch
 * @return true if the right inverter LV switch is on, false otherwise.
 */
bool App_InverterSwitches_IsRightOn(struct InverterSwitches *inverter_switches);

/**
 * Check if the left inverter's LV switch is on
 * @param inverter_switches The InverterSwitches struct to check the status of
 * the switch
 * @return true if the right inverter LV switch is on, false otherwise.
 */
bool App_InverterSwitches_IsLeftOn(struct InverterSwitches *inverter_switches);
