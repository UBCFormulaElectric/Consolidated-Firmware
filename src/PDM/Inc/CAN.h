/**
 *   @file       CAN.h
 *   @brief      Function library header for the CAN controllers on the PDM
 *   @author     UBC Formula Electric
**/

#ifndef CAN_H
#define CAN_H

#include "GPIO.h"
#include <string.h> // For memcpy()
#include "Debug.h"
#include "SharedCAN.h"

// Function declarations
/**
 * 	@brief Initialize CAN 
 *	@param None
 *	@return None
**/
void InitCAN(void);
/**
 *   @brief	  	Sends error CAN messages based on error ID and module
 *  @param		Error_StandardID	Error ID
 *  @param		PCB_Enum			Name of module (Module_Names)
 *  @param		ErrorNumber			Error type
 *  @param 		ErrorData			Error data
 *  @return	 	None
**/
void TransmitCANError(uint32_t Error_StandardID,
                      PCB_Enum Module,
                      uint8_t ErrorNumber,
                      uint32_t ErrorData);

#endif
