/**
 * @file  Can.h
 * @brief Board-specific CAN library
 */

#ifndef CAN_H
#define CAN_H
/******************************************************************************
 * Includes
 *****************************************************************************/
#include "Gpio.h"
#include <string.h>
#include "SharedCAN.h"

/******************************************************************************
 * Preprocessor Constants
 *****************************************************************************/
// clang-format off
#define Systems_Count 5
/******************************************************************************
* Preprocessor Macros
******************************************************************************/

/******************************************************************************
* Typedefs
******************************************************************************/

/******************************************************************************
* Global Variables
******************************************************************************/

/******************************************************************************
* Function Prototypes
******************************************************************************/
/**
 * @brief  Initialize CAN
 */
void InitCAN(void);

/**
 * @brief  Sends error CAN messages based on error ID and module
 * @param  Error_StandardID	Error ID
 * @param  PCB_Enum Name of module (Module_Names)
 * @param  ErrorNumber Error type
 * @param  ErrorData Error data
 */
void TransmitCANError(uint32_t Error_StandardID,
                      PCB_Enum Module,
                      uint8_t ErrorNumber,
                      uint32_t ErrorData);

#endif /* CAN_H */
