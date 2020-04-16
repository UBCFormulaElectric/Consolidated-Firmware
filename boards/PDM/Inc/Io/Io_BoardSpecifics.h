/**
 * @brief Board-specific information required by shared libraries
 */
#pragma once

#define BOARD_NAME_LOWERCASE pdm
#define BOARD_NAME_UPPERCASE PDM
#define PCB_HEARTBEAT_LISTENER BMS_HEARTBEAT_ONE_HOT
#define NUM_ADC_CHANNELS (uint32_t)(9)
