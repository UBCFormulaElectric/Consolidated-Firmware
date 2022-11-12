#pragma once

/**
 * IMPORTANT! LV (+12 V) to the PM100DZ inverters must only be provided once the
 * AIRs are closed. Otherwise, the inverters will enter the fault state on
 * power-up.
 */

/**
 * Turn on LV (+12 V) to the right inverter
 */
void App_InverterSwitches_TurnOnRight();

/**
 * Turn off LV (+12 V) to the right inverter
 */
void App_InverterSwitches_TurnOffRight();

/**
 * Turn on LV (+12 V) to the left inverter
 */
void App_InverterSwitches_TurnOnLeft();

/**
 * Turn off LV (+12 V) to the left inverter
 */
void App_InverterSwitches_TurnOffLeft();

/**
 * Check if the right inverter's LV switch is on
 * @return true if the right inverter LV switch is on, false otherwise.
 */
bool App_InverterSwitches_IsRightOn();

/**
 * Check if the left inverter's LV switch is on
 * @return true if the right inverter LV switch is on, false otherwise.
 */
bool App_InverterSwitches_IsLeftOn();
