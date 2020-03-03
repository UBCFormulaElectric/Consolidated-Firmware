/**
 * @brief Board-specific information required by shared libraries
 */
#ifndef APP_BOARD_SPECIFICS_H
#define APP_BOARD_SPECIFICS_H

#include <stm32f302x8.h>

/******************************************************************************
 * Preprocessor Constants
 ******************************************************************************/
#define BOARD_NAME_LOWERCASE pdm
#define BOARD_NAME_UPPERCASE PDM
#define PCB_HEARTBEAT_LISTENER BMS_HEARTBEAT_ENCODING
#define NUM_ADC_CHANNELS (uint32_t)(9)

#endif /* APP_BOARD_SPECIFICS_H */
