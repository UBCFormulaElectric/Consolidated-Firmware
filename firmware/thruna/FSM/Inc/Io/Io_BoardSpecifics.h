/**
 * @brief Board-specific information required by shared libraries
 */
#pragma once

#define BOARD_NAME_LOWERCASE fsm
#define BOARD_NAME_UPPERCASE FSM
#define PCB_HEARTBEAT_LISTENER BMS_HEARTBEAT_ONE_HOT
// TODO (#309): Correct this
#define NUM_ADC_CHANNELS (uint32_t)(8)
