/**
    @file       ErrorHandling.h
    @brief      PDM 2018 Code
    @author     UBC Formula Electric
    @version    v1.0.00
    @copyright  GNU General Public License v3
*/

#ifndef ERRORHANDLING_H
#define ERRORHANDLING_H

#include "main.h"
#include "CANDefinitions.h"

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
// clang-format on

// Function Declarations
void IncrementNumberOfConsecutiveErrors(Error_Enum Error);
void ErrorHandlingRoutine(void);
void HandleError(Error_Enum Error);

#endif
