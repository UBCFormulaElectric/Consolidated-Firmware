/**
 * @brief Board-specific information required by shared libraries
 */
#pragma once

#include <stm32f302x8.h>

#define BOARD_NAME_LOWERCASE bms
#define BOARD_NAME_UPPERCASE BMS
#define PCB_HEARTBEAT_LISTENER 0
// TODO (#309): Correct this
#define NUM_ADC_CHANNELS (uint32_t)(8)
