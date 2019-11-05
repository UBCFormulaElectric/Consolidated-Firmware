/**
 * @brief Board-specific information required by shared libraries
 */
#ifndef APP_BOARD_SPECIFICS_H
#define APP_BOARD_SPECIFICS_H

/******************************************************************************
 * Preprocessor Constants
 ******************************************************************************/
#define BOARD_NAME_LOWERCASE bms
#define BOARD_NAME_UPPERCASE BMS
#define PCB_HEARTBEAT_LISTENER 0
// TODO (#309): Correct this
#define NUM_ADC_CHANNELS (uint32_t)(8)

#endif /* APP_BOARD_SPECIFICS_H */
