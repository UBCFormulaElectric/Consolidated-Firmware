/**
	@file	   	CAN.c
	@brief	  	CAN Controller Header using STM32F0 HAL 1.9.0 version
	@author	 	UBC Formula Electric
	@version	v1.0.00
	@copyright  GNU General Public License v3
*/


#include "SharedCAN_1.9.0.h"

// Tx and Rx message structures
static CanTxMsg CANTxMessage = {0};
static CanRxMsg CANRxMessage = {0};

// Create a default message
static CanTxMsg CANDefaultMessage = {0};

// CAN Queue
static CanTxMsg CANQueue[QUEUE_SIZE] = {0};
static __IO uint8_t tail = 0;
static __IO uint8_t head = 0;

// Heartbeat and WDT setup
const int WATCHDOG_TIMER_PERIOD = 200;			// Period in ms

#ifndef DEBUG

__IO uint16_t HeartbeatCount[Systems_Count] = {0};
const int HEARTBEAT_TICK_PERIOD = 1000;			// Period in ms
const int HEARTBEAT_BROADCAST_PERIOD = 300;		// Period in ms

#endif

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

/**
	@brief	  	Sends error CAN messages based on error ID and module
	@param		ErrorID			Error ID
	@param		Module			Name of module (Module_Names)
	@param		ErrorNumber		Error type
	@param 		ErrorData		Error data
	@return	 	None
*/
void TransmitCANError(uint32_t Error_StandardID, Module_Name Module, uint8_t ErrorNumber, uint32_t ErrorData)
{
	uint64_t Data = ((uint64_t) Module << 40) + ((uint64_t) ErrorNumber << 32) + ErrorData;
	uint32_t Error_ExtendedID = 0;
	uint8_t Error_DLC = 0;

	switch(Error_StandardID)
	{
		case AIR_Shutdown_Error_StandardID:
			Error_ExtendedID = AIR_Shutdown_Error_ExtendedID;
			Error_DLC = AIR_Shutdown_Error_DLC;
			break;
		case Motor_Shutdown_Error_StandardID:
			Error_ExtendedID = Motor_Shutdown_Error_ExtendedID;
			Error_DLC = Motor_Shutdown_Error_DLC;
			break;
		case General_Error_StandardID:
			Error_ExtendedID = General_Error_StandardID;
			Error_DLC = General_Error_DLC;
			break;
		default:
			Error_ExtendedID = 0;
			Error_DLC = 4;
			break;
	}

	TransmitDataCAN(Error_StandardID , Error_ExtendedID, Error_DLC , Data);
}

/**
	@brief	  	Broadcasts a heartbeat message on the CAN bus
	@param		Module			Name of module (Module_Names)
	@return		None
*/
void BroadcastHeartbeat(Module_Name Module)
{
	TransmitDataCAN(Heartbeat_StandardID, Heartbeat_ExtendedID, Heartbeat_DLC, (uint64_t) Module);
}

/**
	@brief	  	Broadcasts a cell voltage message on the CAN bus
	@param		Module	Module number
	@param      Cell    Cell number
	@param      adc_value		
	@return		None
*/
void BroadcastCellVoltage(uint8_t module, uint8_t cell, uint16_t adc_value)
{
	uint8_t data[4];
	uint64_t CANData;
	data[0] = module;
	data[1] = cell; 
	memcpy(&data[2], &adc_value, sizeof(adc_value));
	memcpy(&CANData, data, sizeof(data));
	TransmitDataCAN(CellVoltage_StandardID, CellVoltage_ExtendedID, CellVoltage_DLC, CANData);
}

/**
	@brief	  	Broadcasts cell balancing MOSFET statuses
	@param		uint8_t			BMS Module number
	@param		uint32_t		Onehot encoding of MOSFET status
	@return		None
*/
void BroadcastBalanceMosfetStatus(uint8_t module, uint32_t onehot_mosfet)
{
	uint8_t data[5];
	uint64_t CANData;
	data[0] = module;
	memcpy(&data[1], &onehot_mosfet, sizeof(onehot_mosfet));
	memcpy(&CANData, data, sizeof(data));
	TransmitDataCAN(BalanceMosfet_StandardID, BalanceMosfet_ExtendedID, BalanceMosfet_DLC, CANData);
}

/**
	@brief	  	Broadcasts Pre Charge successful message
	@return		None
*/
void BroadcastPreChargeComplete(void)
{
	TransmitDataCAN(PreChargeComplete_StandardID, PreChargeComplete_ExtendedID, PreChargeComplete_DLC, 0);
}

/****************************** CAN library for the STM32F0 (FSM_2017) *********************************/
#ifdef STM32F042x6

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
		
		#endif
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
#endif

/********************************* CAN code for the STM32F302x8 for the BMS, DCM ***************************************/
#ifdef STM32F302x8

#include "stm32f30x.h"
#include "GPIO.h"
#include "SysTick.h"

#define MCR_DBF		   ((uint32_t)0x00010000) /* software master reset */

#ifdef BMS

/**
@brief	  	Configures the CAN controller
@param	  	None
@return	 	None
*/
void ConfigureCAN(void)
{
	CAN_InitTypeDef CAN_InitStructure;
	CAN_FilterInitTypeDef CAN_FilterInitStructure;

	// Enable peripheral clock to CAN1 controller
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);

	// Reset CAN1 configuration registers to default values
	CAN_DeInit(CAN1);

	// Configure CAN1 parameters
	CAN_InitStructure.CAN_Prescaler = CAN_Prescaler_Value;	  	// Specify length of a time quantum
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
	CAN_InitStructure.CAN_SJW = CAN_SJW_Value;				  	// Specify maximum number of time quanta the CAN controller is allowed to lengthen or shorten a bit to perform resynchronization
	CAN_InitStructure.CAN_BS1 = CAN_BS1_Value;				  	// Specify number of time quanta in bit segment 1
	CAN_InitStructure.CAN_BS2 = CAN_BS2_Value;				  	// Specify number of time quanta in bit segment 2
	CAN_InitStructure.CAN_TTCM = ENABLE;						// Enable time-triggered communication mode
	CAN_InitStructure.CAN_ABOM = DISABLE;
	CAN_InitStructure.CAN_AWUM = DISABLE;
	CAN_InitStructure.CAN_NART = DISABLE;
	CAN_InitStructure.CAN_RFLM = ENABLE;						// Enable Rx FIFO locked mode
	CAN_InitStructure.CAN_TXFP = ENABLE;						// Enable Tx FIFO priority
	CAN_Init(CAN1, &CAN_InitStructure);

	// Configure CAN filter
	CAN_FilterInitStructure.CAN_FilterNumber = 0;
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_16bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh = Heartbeat_StandardID << 5;
	CAN_FilterInitStructure.CAN_FilterIdLow = 0;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);

	// Disable debug freeze
	CAN_DBGFreeze(CAN1, DISABLE);

	// Enable FIFO 0 message pending interrupt
	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);

	// Enable transmit mailbox empty interrupt for CAN queue
	CAN_ITConfig(CAN1, CAN_IT_TME, ENABLE);

	// Initialize CAN queue
	CANResetQueue();
}

/**
	@brief	  	Transmits a CAN message
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

	Mailbox = CAN_Transmit(CAN1, &CANTxMessage);

	// Check if transmit mailboxes are full
	if(Mailbox == CAN_TxStatus_NoMailBox)
	{
		while(CANEnqueue(CANTxMessage) != QUEUE_OP_SUCCESS);
	}
	else
	{
		if(CAN_TransmitStatus(CAN1, Mailbox) != CAN_TxStatus_Ok)
		{
			// Handle CAN transmission error
		}
	}
}

/**
	@brief	  	IRQ handler for RX CAN controller
				1. Handles the reception of data over CAN
	@param	  	None
	@return	 	None
*/
void USB_LP_CAN_RX0_IRQHandler(void)
{
	uint8_t Board;
	
	if(CAN_GetITStatus(CAN1, CAN_IT_FMP0) != RESET)
	{
		CAN_Receive(CAN1, CAN_FIFO0, &CANRxMessage);
		CAN_FIFORelease(CAN1, CAN_FIFO0);
		
		#ifndef DEBUG

		// CAN message begins at Data[0], ends at Data[7]
		switch(CANRxMessage.StdId)
		{
			case Heartbeat_StandardID:
				// Extract the board and increment its heartbeat count
				Board = CANRxMessage.Data[0];
				// Case statement used in case data is outside of array bounds. Special cases / behaviour can also be handled here. 
				switch(Board)
				{
					case Drive_Control_Module:
						HeartbeatCount[Board]++;
						break;
					case Power_Distribution_Module:
						HeartbeatCount[Board]++;
						break;
					case Front_Sensor_Module:
						HeartbeatCount[Board]++;
						break;
					default:
						// Log error
						break;
				}
				break;
			default:
				// Log error
				break;
		}
		
		#endif
	}
}

/**
	@brief	  	IRQ handler for TX CAN controller
				1. Transmits messages in CAN queue
	@param	  	None
	@return	 	None
*/
void USB_HP_CAN_TX_IRQHandler(void)
{
	if(CAN_GetITStatus(CAN1, CAN_IT_TME) != RESET)
	{
		if(CANDequeue() != QUEUE_OP_SUCCESS)
		{
			CANResetQueue();
		}

		CAN_ClearITPendingBit(CAN1, CAN_IT_TME);
	}
}

#endif


#ifdef DCM

// External globals
extern __IO float FLWheelSpeed;
extern __IO float FRWheelSpeed;
extern __IO float FLWheelRPM;
extern __IO float FRWheelRPM;
extern __IO float LeftMotorRPM;
extern __IO float RightMotorRPM;
extern __IO uint16_t AcceleratorPedalPosition;
extern __IO float SteeringAngle;
extern __IO uint8_t TractionControlEnable;
extern __IO uint8_t TorqueVectoringEnable;
extern __IO float DischargePowerLimit;
extern __IO float RegenPowerLimit;
extern const float MOTOR_SPEED_TO_RPM;
extern const float WHEEL_SPEED_TO_WHEEL_RPM;
extern __IO Regen_States Regen_State;

// Globals
__IO CAN_Motor_Shutdown_Handler Motor_Handler;

/**
	@brief	  	Configures the CAN controller
	@param	  	None
	@return	 	None
*/
void ConfigureCAN(void)
{
	CAN_InitTypeDef CAN_InitStructure;
	CAN_FilterInitTypeDef CAN_FilterInitStructure;

	// Enable peripheral clock to CAN1 controller
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);

	// Reset CAN1 configuration registers to default values
	CAN_DeInit(CAN1);

	// Configure CAN1 parameters
	CAN_InitStructure.CAN_Prescaler = CAN_Prescaler_Value;	  	// Specify length of a time quantum
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
	CAN_InitStructure.CAN_SJW = CAN_SJW_Value;				  	// Specify maximum number of time quanta the CAN controller is allowed to lengthen or shorten a bit to perform resynchronization
	CAN_InitStructure.CAN_BS1 = CAN_BS1_Value;				  	// Specify number of time quanta in bit segment 1
	CAN_InitStructure.CAN_BS2 = CAN_BS2_Value;				  	// Specify number of time quanta in bit segment 2
	CAN_InitStructure.CAN_TTCM = ENABLE;						// Enable time-triggered communication mode
	CAN_InitStructure.CAN_ABOM = DISABLE;
	CAN_InitStructure.CAN_AWUM = DISABLE;
	CAN_InitStructure.CAN_NART = DISABLE;
	CAN_InitStructure.CAN_RFLM = ENABLE;						// Enable Rx FIFO locked mode
	CAN_InitStructure.CAN_TXFP = ENABLE;						// Enable Tx FIFO priority
	CAN_Init(CAN1, &CAN_InitStructure);

	// Configure CAN filter
	CAN_FilterInitStructure.CAN_FilterNumber = 0;
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_16bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh = BrakeAPPS_StandardID << 5;
	CAN_FilterInitStructure.CAN_FilterIdLow = SteeringAngle_StandardID << 5;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = RightWheelSpeed_StandardID << 5;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = LeftWheelSpeed_StandardID << 5;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);

	CAN_FilterInitStructure.CAN_FilterNumber = 1;
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_16bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh = AccumulatorDischargePowerLimit_StandardID << 5;
	CAN_FilterInitStructure.CAN_FilterIdLow = Motor_Shutdown_Error_StandardID << 5;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = General_Error_StandardID << 5;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = Motor_ReEnable_StandardID << 5;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);

	CAN_FilterInitStructure.CAN_FilterNumber = 2;
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_16bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh = AccumulatorRegenPowerLimit_StandardID << 5;
	CAN_FilterInitStructure.CAN_FilterIdLow = LeftInverterTX_StandardID << 5;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = RightInverterTX_StandardID << 5;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = Latch_Reset_StandardID << 5;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);
	
	CAN_FilterInitStructure.CAN_FilterNumber = 3;
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_16bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh = Heartbeat_StandardID << 5;
	CAN_FilterInitStructure.CAN_FilterIdLow = 0;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);

	// Disable debug freeze
	CAN_DBGFreeze(CAN1, DISABLE);

	// Enable FIFO 0 message pending interrupt
	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);

	// Enable transmit mailbox empty interrupt for CAN queue
	CAN_ITConfig(CAN1, CAN_IT_TME, ENABLE);

	// Initialize CAN queue
	CANResetQueue();
}

/**
	@brief	  	Transmits a CAN message
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

	Mailbox = CAN_Transmit(CAN1, &CANTxMessage);

	// Check if transmit mailboxes are full
	if(Mailbox == CAN_TxStatus_NoMailBox)
	{
		while(CANEnqueue(CANTxMessage) != QUEUE_OP_SUCCESS);
	}
	else
	{
		if(CAN_TransmitStatus(CAN1, Mailbox) != CAN_TxStatus_Ok)
		{
			// Handle CAN transmission error
		}
	}
}

void TransmitDataInverter(uint32_t InverterID, uint8_t DestinationRegister, uint8_t DLC, uint64_t Data)
{
	uint8_t Mailbox = 0;
	uint64_t DataHolder;

	CANTxMessage.StdId = InverterID;
	CANTxMessage.ExtId = 0;
	CANTxMessage.IDE = CAN_ID_STD;
	CANTxMessage.RTR = CAN_RTR_Data;
	CANTxMessage.DLC = DLC;

	DataHolder = (uint64_t)((Data << 8) + DestinationRegister);
	memcpy(&CANTxMessage.Data, &DataHolder, 8);

	Mailbox = CAN_Transmit(CAN1, &CANTxMessage);

	// Check if transmit mailboxes are full
	if(Mailbox == CAN_TxStatus_NoMailBox)
	{
		while(CANEnqueue(CANTxMessage) != QUEUE_OP_SUCCESS);
	}
	else
	{
		if(CAN_TransmitStatus(CAN1, Mailbox) != CAN_TxStatus_Ok)
		{
			// Handle CAN transmission error
		}
	}
}

/**
	@brief	  	IRQ handler for TX CAN controller
				1. Transmits messages in CAN queue
	@param	  	None
	@return	 	None
*/
void USB_HP_CAN_TX_IRQHandler(void)
{
	if(CAN_GetITStatus(CAN1, CAN_IT_TME) != RESET)
	{
		if(CANDequeue() != QUEUE_OP_SUCCESS)
		{
			CANResetQueue();
		}

		CAN_ClearITPendingBit(CAN1, CAN_IT_TME);
	}
}

/**
	@brief	  	IRQ handler for the CAN controller
				1. Handles the reception of data over CAN
	@param	  	None
	@return	 	None
*/
void USB_LP_CAN_RX0_IRQHandler(void)
{
	Module_Name Board;
	uint8_t Error_Number;
	uint32_t Error_Data;
	uint16_t Motor_Speed;

	if(CAN_GetITStatus(CAN1, CAN_IT_FMP0) != RESET)
	{
		CAN_Receive(CAN1, CAN_FIFO0, &CANRxMessage);
		CAN_FIFORelease(CAN1, CAN_FIFO0);

		// CAN message begins at Data[0], ends at Data[7]
		switch(CANRxMessage.StdId)
		{
			case BrakeAPPS_StandardID:
				// Must explicitly cast voltatile variable to a void pointer
				// Extract accelerator pedal position
				memcpy((void*)&AcceleratorPedalPosition, &CANRxMessage.Data, 2);
				break;
			case SteeringAngle_StandardID:
				// Extract steering angle
				memcpy((void*)&SteeringAngle, &CANRxMessage.Data, 4);
				break;
			case RightWheelSpeed_StandardID:
				// Extract front right wheel speeds
				memcpy((void*)&FRWheelSpeed, &CANRxMessage.Data, 4);
				FRWheelRPM = WHEEL_SPEED_TO_WHEEL_RPM * FRWheelSpeed;
			
				// As per EV2.2.7, regen is not allowed above 5km/h
				if (FRWheelSpeed <= 5.0f || FLWheelSpeed <= 5.0f)
				{
					Regen_State = REGEN_OFF;
				}
				else
				{
					Regen_State = REGEN_ON;
				}
				break;
			case LeftWheelSpeed_StandardID:
				// Extract front left wheel speeds
				memcpy((void*)&FLWheelSpeed, &CANRxMessage.Data, 4);
				FLWheelRPM = WHEEL_SPEED_TO_WHEEL_RPM * FLWheelSpeed;
			
				// As per EV2.2.7, regen is not allowed above 5km/h
				if (FRWheelSpeed <= 5.0f || FLWheelSpeed <= 5.0f)
				{
					Regen_State = REGEN_OFF;
				}
				else
				{
					Regen_State = REGEN_ON;
				}
				break;
			case AccumulatorDischargePowerLimit_StandardID:
				// Extract discharge power limit from message
				memcpy((void*)&DischargePowerLimit, CANRxMessage.Data, 4);
				break;
			case AccumulatorRegenPowerLimit_StandardID:
				// Extract regen limit from message
				memcpy((void*)&RegenPowerLimit, CANRxMessage.Data, 4);
				break;
			case LeftInverterTX_StandardID:
				// Check the register that the inverter is sending
				if (CANRxMessage.Data[0] == SPEED_ACTUAL)
				{
					// Extract motor RPM from message
					Motor_Speed = (CANRxMessage.Data[2] << 8) + CANRxMessage.Data[1];
					LeftMotorRPM = (float)(Motor_Speed * MOTOR_SPEED_TO_RPM);
				}
				break;
			case RightInverterTX_StandardID:
				// Check the register that the inverter is sending
				if (CANRxMessage.Data[0] == SPEED_ACTUAL)
				{
					// Extract motor RPM from message
					Motor_Speed = (CANRxMessage.Data[2] << 8) + CANRxMessage.Data[1];
					RightMotorRPM = (float)(Motor_Speed * MOTOR_SPEED_TO_RPM);
				}
				break;
			// Handle motor shutdown and AIR shutdown the same way:
			// - Enter error state, turn off the motors, set the sending board's motor status to OFF
			// - If the BMS is reset through the latch, it will send a motor on message
			case Motor_Shutdown_Error_StandardID:
			case AIR_Shutdown_Error_StandardID:
				// Set the motor handler
				Board = (Module_Name)CANRxMessage.Data[5];

				switch(Board)
				{
					case Front_Sensor_Module:
						Motor_Handler.FSM_Status = OFF;
						break;
					case Power_Distribution_Module:
						Motor_Handler.PDM_Status = OFF;
						break;
					case Battery_Management_System:
						Motor_Handler.BMS_Status = OFF;
						break;
					default:
						// Log error
						break;
				}

				// Run stop sequence and enter error state
				State_Machine_Event_Handler(EVENT_ERROR);
				break;
			case Motor_ReEnable_StandardID:
				// Set the motor handler
				Board = (Module_Name)CANRxMessage.Data[0];

				switch(Board)
				{
					case Front_Sensor_Module:
						Motor_Handler.FSM_Status = ON;
						break;
					case Power_Distribution_Module:
						Motor_Handler.PDM_Status = ON;
						break;
					case Battery_Management_System:
						Motor_Handler.BMS_Status = ON;
						break;
					default:
						// Log error
						break;
				}

				// Check that all three systems have not shut down the motor (or have re-enabled it)
				if (Motor_Handler.FSM_Status == ON &&
					Motor_Handler.PDM_Status == ON &&
					Motor_Handler.BMS_Status == ON)
				{
					// Run recovery sequence and enter previous state
					State_Machine_Event_Handler(EVENT_RECOVERY);
				}
				break;
			case General_Error_StandardID:
				// Extract board and error numbers from message
				Board = (Module_Name)CANRxMessage.Data[5];
				Error_Number = CANRxMessage.Data[4];
				memcpy(&Error_Data, CANRxMessage.Data, 4);

				// Process errors
				switch(Board)
				{
					case Front_Sensor_Module:
						if (Error_Number == FSM_SteeringBounds_Error)
						{
							TractionControlEnable = 0;
							TorqueVectoringEnable = 0;
						}
						else if (Error_Number == FSM_WheelSpeedBounds_Error)
						{
							TorqueVectoringEnable = 0;
						}
						break;
					case Power_Distribution_Module:
						if (Error_Number == PDM_Efuse_Error)
						{

						}
						else if (Error_Number == PDM_Misc_Error)
						{

						}
						break;
					case Battery_Management_System:
						break;
					default:
						// Log error
						break;
				}
				break;
			case Latch_Reset_StandardID:
				// Re-enable BMS status
				Motor_Handler.BMS_Status = ON;
				
				// Check that all three systems have not shut down the motor (or have re-enabled it)
				if (Motor_Handler.FSM_Status == ON &&
					Motor_Handler.PDM_Status == ON &&
					Motor_Handler.BMS_Status == ON)
				{
					// Run latch reset event
					State_Machine_Event_Handler(EVENT_LATCH_RESET);
				}
				break;
			
			#ifndef DEBUG
			
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
			
			#endif 
			
			default:
				// Handle error
				break;
		}
	}
}

#endif

#endif