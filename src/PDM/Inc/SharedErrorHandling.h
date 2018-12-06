/**
 * @file  SharedErrorHandling.h
 * @brief Shared Error Handling library
 */

#ifndef SHARED_ERRORHANDLING_H
#define SHARED_ERRORHANDLING_H

/******************************************************************************
* Includes
*******************************************************************************/
#include "main.h"
#include "CANDefinitions.h"

/******************************************************************************
* Preprocessor Constants
*******************************************************************************/
// clang-format off
#ifdef PDM
    #define PCB_ERROR_CAN_ID    PDM_ERRORS
    #define Error_Enum          PdmError_Enum
#elif DCM
    #define PCB_ERROR_CAN_ID    DCM_ERRORS
    #define Error_Enum          DcmError_Enum
#elif BMS
    #define PCB_ERROR_CAN_ID    BMS_ERRORS
    #define Error_Enum          BmsError_enum
#elif FSM
    #define PCB_ERROR_CAN_ID    FSM_ERRORS
    #define Error_Enum          FsmError_enum
#endif

/******************************************************************************
* Preprocessor Macros
*******************************************************************************/

/******************************************************************************
* Typedefs
*******************************************************************************/
// clang-format on

/******************************************************************************
* Global Variables
*******************************************************************************/

/******************************************************************************
* Function Prototypes
*******************************************************************************/
/**
 * @brief TODO: Complete this
 */
void IncrementNumberOfConsecutiveErrors(Error_Enum Error);

/**
 * @brief TODO: Complete this
 */
void ErrorHandlingRoutine(void);

/**
 * @brief TODO: Complete this
 */
void HandleError(Error_Enum Error);

#endif /* SHARED_ERRORHANDLING_H */
