/**
 * @brief Board-specific information required by shared libraries
 */
#ifndef APP_BOARD_SPECIFICS_H
#define APP_BOARD_SPECIFICS_H

/******************************************************************************
 * Preprocessor Constants
 ******************************************************************************/
#define BOARD_NAME_LOWERCASE fsm
#define BOARD_NAME_UPPERCASE FSM
#define PCB_HEARTBEAT_LISTENER BMS_HEARTBEAT_ENCODING
// TODO (#309): Correct this
#define NUM_ADC_CHANNELS (uint32_t)(8)

#endif /* APP_BOARD_SPECIFICS_H */
