/**
	@file	   	CAN.c
	@brief	  	CAN Controller Header using STM32F0 HAL 1.9.0 version
	@author	 	UBC Formula Electric
	@version	v1.0.00
	@copyright  GNU General Public License v3
*/


#include "SharedCAN_1.9.0.h"

/* This file should only be compiled for the STM32F0 board, whose HAL library lags behind that
   available for STM32F3. 
*/
#ifdef STM32F042x6

// Tx and Rx message structures
static CanTxMsg CANTxMessage = {0};
static CanRxMsg CANRxMessage = {0};

// Create a default message
static CanTxMsg CANDefaultMessage = {0};

// CAN Queue
static CanTxMsg CANQueue[QUEUE_SIZE] = {0};
static __IO uint8_t tail = 0;
static __IO uint8_t head = 0;


/**
	@brief	  	Dequeue function for CAN queue
				1. Handles transmitting overflow CAN messages
	@param	  	None
	@return	 	0		Failed dequeue
				1		Successful dequeue
*/
uint8_t CANDequeue(void)
{
	uint8_t status = 0, items = 0;
	uint64_t CANData = 0x0;

	// Check if queue is empty
	items = CANGetItemsInQueue();
	if(items != 0)
	{
		// Parse CAN message in queue, dequeue a CAN message, and transmit
		memcpy(&CANData, &CANQueue[tail].Data, 8);

		TransmitDataCAN(CANQueue[tail].StdId, CANQueue[tail].ExtId, CANQueue[tail].DLC, CANData);
		CANQueue[tail] = CANDefaultMessage;
		tail++;
		if(tail == QUEUE_SIZE)
		{
			tail = 0;
		}
		status = 1;
	}
	// If queue is empty, don't dequeue
	else if(items == 0)
	{
		status = 1;
	}
	else
	{
		status = 0;
	}

	return status;
}

/**
	@brief	  	Enqueue function for CAN queue
				1. Saves overflow CAN transmission messages
	@param	  	CANMsg		CAN message to be queued
	@return	 	0			Failed enqueue
				1			Successful enqueue
*/
uint8_t CANEnqueue(CanTxMsg CANMsg)
{
	uint8_t status = 0, items = 0;

	// Check if the queue is full
	items = CANGetItemsInQueue();
	if(items < QUEUE_SIZE)
	{
		// Save CAN message in FIFO queue
		CANQueue[head] = CANMsg;

		head++;
		// Check if head is outside bounds of queue
		if(head == QUEUE_SIZE)
		{
			head = 0;
		}
		status = 1;
	}
	// If queue is full, don't enqueue
	else if(items == QUEUE_SIZE)
	{
		status = 1;
	}
	else
	{
		status = 0;
	}


	return status;
}

/**
	@brief	  	Resets the CAN queue
	@param	  	None
	@return	 	None
*/
void CANResetQueue(void)
{
	uint8_t i;
	for(i = 0; i < QUEUE_SIZE; i++)
	{
		CANQueue[i] = CANDefaultMessage;
	}
}

/**
	@brief	  	Gets the amount of messages saved in the CAN queue
	@param	  	None
	@return	 	Number of items in the queue
*/
uint8_t CANGetItemsInQueue(void)
{
	uint8_t i, items = 0;

	for(i = 0; i < QUEUE_SIZE; i++)
	{
		if(CANQueue[i].StdId != 0)
		{
			items++;
		}
	}

	return items;
}

/****************************** CAN library for the STM32F0 (FSM_2017) *********************************/

/**
	@brief	  	Configures the CAN controller on the STM32F0
	@param	  	None
	@return	 	None
*/
void ConfigureCAN(void)
{
	CAN_InitTypeDef CAN_InitStructure;
	CAN_FilterInitTypeDef CAN_FilterInitStruct;

	// Enable peripheral clock to CAN controller
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN, ENABLE);

	// Reset CAN configuration registers to default values
	CAN_DeInit(CAN);

	// Configure CAN parameters
	CAN_InitStructure.CAN_Prescaler = CAN_Prescaler_Value;		  	// Specify length of a time quantum
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
	CAN_InitStructure.CAN_SJW = CAN_SJW_Value;					  	// Specify maximum number of time quanta the CAN controller is allowed to lengthen or shorten a bit to perform resynchronization
	CAN_InitStructure.CAN_BS1 = CAN_BS1_Value;					  	// Specify number of time quanta in bit segment 1
	CAN_InitStructure.CAN_BS2 = CAN_BS2_Value;					  	// Specify number of time quanta in bit segment 2
	CAN_InitStructure.CAN_TTCM = ENABLE;							// Enable time-triggered communication mode
	CAN_InitStructure.CAN_ABOM = DISABLE;
	CAN_InitStructure.CAN_AWUM = DISABLE;
	CAN_InitStructure.CAN_NART = DISABLE;
	CAN_InitStructure.CAN_RFLM = ENABLE;							// Enable Rx FIFO locked mode
	CAN_InitStructure.CAN_TXFP = ENABLE;							// Enable Tx FIFO priority
	CAN_Init(CAN, &CAN_InitStructure);

	// Configure CAN filter
	CAN_FilterInitStruct.CAN_FilterNumber = 0;
	CAN_FilterInitStruct.CAN_FilterMode = CAN_FilterMode_IdMask;
	CAN_FilterInitStruct.CAN_FilterScale = CAN_FilterScale_32bit;
	CAN_FilterInitStruct.CAN_FilterIdHigh = Heartbeat_StandardID << 5;
	CAN_FilterInitStruct.CAN_FilterIdLow = 0;
	CAN_FilterInitStruct.CAN_FilterMaskIdHigh = 0;
	CAN_FilterInitStruct.CAN_FilterMaskIdLow = 0;
	CAN_FilterInitStruct.CAN_FilterFIFOAssignment = 0;
	CAN_FilterInitStruct.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStruct);

	// Disable debug freeze
	CAN_DBGFreeze(CAN, DISABLE);

	// Enable FIFO 0 message pending interrupt and transmit mailbox empty interrupt
	CAN_ITConfig(CAN, CAN_IT_FMP0, ENABLE);
	CAN_ITConfig(CAN, CAN_IT_TME, ENABLE);

	// Initialize CAN queue
	CANResetQueue();
}

/**
	@brief	  	Transmits a CAN message from the STM32F0
	@param	  	StandardID	  	Standard CAN message ID
	@param	  	ExtendedID	  	Extended CAN message ID
	@param	  	DLC			 	Data length code (number of data bytes to be transmitted)
	@para,	  	Data			Data to be transmitted (up to 8 bytes)
	@return	 	None
*/
void TransmitDataCAN(uint32_t StandardID, uint32_t ExtendedID, uint8_t DLC, uint64_t Data)
{
	uint8_t Mailbox = 0;

	CANTxMessage.StdId = StandardID;
	CANTxMessage.ExtId = ExtendedID;
	CANTxMessage.IDE = CAN_ID_STD;
	CANTxMessage.RTR = CAN_RTR_Data;
	CANTxMessage.DLC = DLC;

	memcpy(&CANTxMessage.Data, &Data, 8);

	Mailbox = CAN_Transmit(CAN, &CANTxMessage);

	// Check if transmit mailboxes are full
	if(Mailbox == CAN_TxStatus_NoMailBox)
	{
		while(CANEnqueue(CANTxMessage) != QUEUE_OP_SUCCESS);
	}
	else
	{
		if(CAN_TransmitStatus(CAN, Mailbox) != CAN_TxStatus_Ok)
		{
			// Handle CAN transmission error
		}
	}
}

/**
	@brief	  	IRQ handler for the CAN controller
				1. Handles the reception of data over CAN
	@param	  	None
	@return	 	None
*/
void CEC_CAN_IRQHandler(void)
{
	Module_Name Board;
	
	// Check if we received a CAN message
	if(CAN_GetITStatus(CAN, CAN_IT_FMP0) != RESET)
	{
		CAN_Receive(CAN, CAN_FIFO0, &CANRxMessage);
		CAN_FIFORelease(CAN, CAN_FIFO0);
		
		#ifndef DEBUG

		switch(CANRxMessage.StdId)
		{
			case Heartbeat_StandardID:
				// Process Heartbeat info here
				Board = (Module_Name)CANRxMessage.Data[0];

				// Case statement used in case data is outside of array bounds. Special cases / behaviour can also be handled here. 
				switch(Board)
				{
					case Battery_Management_System:
						HeartbeatCount[Battery_Management_System]++;
					default:
						// Log error
						break;
				}
				break;
			default:
				// Handle error
				break;
		}
		
		#endif //DEBUG
	}
	else if(CAN_GetITStatus(CAN, CAN_IT_TME) != RESET)
	{
		if(CANDequeue() != QUEUE_OP_SUCCESS)
		{
			CANResetQueue();
		}
		
		CAN_ClearITPendingBit(CAN, CAN_IT_TME);
	}
}
#endif // STM32F042x6
