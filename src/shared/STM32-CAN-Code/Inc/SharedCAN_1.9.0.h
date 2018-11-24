/**
	@file	   	SharedCAN_1.9.0.h
	@brief	  	CAN Controller Header using STM32F0 HAL 1.9.0 version
	@author	 	UBC Formula Electric
	@version	v1.0.00
	@copyright  GNU General Public License v3
*/

#ifdef STM32F042x6

// CAN Queue
static const uint8_t QUEUE_SIZE = 20;
static const uint8_t QUEUE_OP_SUCCESS = 1;
static const uint8_t QUEUE_OP_FAIL = 0;

#include "stm32f0xx_conf.h"

// Time quanta values for the 48MHz CAN clock on the STM32F042 (all calculations done at http://www.bittiming.can-wiki.info/)
static const uint16_t CAN_Prescaler_Value = 8;	  	// Clock prescaler
static const uint8_t CAN_SJW_Value = 2;			 	// Number of time quanta
static const uint8_t CAN_BS1_Value = 5;			 	// Number of time quanta for bit segment 1
static const uint8_t CAN_BS2_Value = 4;			 	// Number of time quanta for bit segment 2

// Function declarations
void ConfigureCAN(void);
void TransmitDataCAN(uint32_t StandardID, uint32_t ExtendedID, uint8_t DLC, uint64_t Data);
uint8_t CANDequeue(void);
uint8_t CANEnqueue(CanTxMsg CANMsg);
void CANResetQueue(void);
uint8_t CANGetItemsInQueue(void);
void TransmitCANError(uint32_t Error_StandardID, Module_Name Module, uint8_t ErrorNumber, uint32_t ErrorData);
void BroadcastHeartbeat(Module_Name Module);
void BroadcastCellVoltage(uint8_t module, uint8_t cell, uint16_t adc_value);
void BroadcastBalanceMosfetStatus(uint8_t module, uint32_t onehot_mosfet);
void BroadcastPreChargeComplete(void);

#endif//STM32F042x6