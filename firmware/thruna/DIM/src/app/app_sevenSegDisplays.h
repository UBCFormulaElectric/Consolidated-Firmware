#pragma once

#include <stdint.h>
#include <stddef.h>
#include "io_sevenSegDisplays.h"

/**
 * Initialize the 7-seg app module.
 */
void app_sevenSegDisplays_init(void);

/**
 * Write a number to the specified group.
 * @param group The group to write to.
 * @return True if number could be written successfully, else false.
 */
bool app_sevenSegDisplays_setGroup(SevenSegGroup group, float value);

/**
 * Disable the specified group (turn off).
 * @param group The group to disable.
 */
void app_sevenSegDisplays_disableGroup(SevenSegGroup group);

/**
 * Set the fault code recieved from the board
 * @param group The group to write to
 * @param board the board we are getting the fault form
 * @param fault the fault or warning that was called  
*/
bool app_sevenSegDisplays_setFaultCode(SevenSegGroup group, HexDigit board, HexDigit fault);
