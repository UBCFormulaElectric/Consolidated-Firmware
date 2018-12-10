/**
 * @file  Can.h
 * @brief Board-specific CAN library
 */

#ifndef CAN_H
#define CAN_H
/******************************************************************************
 * Includes
 *******************************************************************************/
#include "SharedCan.h"

/******************************************************************************
 * Preprocessor Constants
 *******************************************************************************/
// clang-format off
/******************************************************************************
* Preprocessor Macros
*******************************************************************************/

/******************************************************************************
* Typedefs
*******************************************************************************/

/******************************************************************************
* Global Variables
*******************************************************************************/

/******************************************************************************
* Function Prototypes
*******************************************************************************/
/**
 * @brief Send CAN message one-hot encoded for one or more errors
 * @param PdmError_Enum One or more errors OR'd together
 */
void Can_BroadcastPdmErrors(PdmError_Enum error);

/**
 * @brief Broadcast the AIR shutdown error
 */
void Can_BroadcastAirShutdownError(void);

/**
 * @brief Broadcast the motor shutdown error
 */
void Can_BroadcastMotorShutdownError(void);

#endif /* CAN_H */
