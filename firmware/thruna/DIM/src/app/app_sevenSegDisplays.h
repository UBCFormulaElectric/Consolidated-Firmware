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
 * Set the fault code recieved from the board
 * @param group The group to write to.
 * @param board the board we are getting the fault form.
 * @param fault the fault or warning that was called.
 * @return If the fault code could be written sucessfully.
 */
bool app_sevenSegDisplays_setFaultCode(SevenSegGroup group, uint16_t FaultCode);

/**
 * Disable the specified group (turn off).
 * @param group The group to disable.
 */
void app_sevenSegDisplays_disableGroup(SevenSegGroup group);
