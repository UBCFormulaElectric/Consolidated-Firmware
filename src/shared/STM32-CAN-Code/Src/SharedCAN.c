/**
	@file	   	SharedCAN.c
	@brief	  	Function library for the CAN controllers on the STM32F0 and STM32F3
	@author	 	UBC Formula Electric
	@version	v1.0.00
	@copyright  GNU General Public License v3
*/
#include <string.h>
#include "SharedCAN.h"

// CAN Tx Message Fifo
static const uint32_t CAN_TX_MSG_FIFO_SIZE = 20;
static CanTxMsg_Struct can_tx_msg_fifo[CAN_TX_MSG_FIFO_SIZE];
static __IO uint8_t tail = 0;
static __IO uint8_t head = 0;

CAN_TxHeaderTypeDef can_headers[CAN_NODES_COUNT];

#define INIT_MASK_FILTER(ID, MASK) {.id = ID, .mask = MASK}
#define INIT_CAN_HEADER(std_id, dlc) {.StdId = std_id, .ExtId = CAN_ExtID_NULL, .IDE = CAN_ID_STD, .RTR = CAN_RTR_DATA, .DLC = dlc, .TransmitGlobalTime = DISABLE}

#if defined(PDM)
	CanMaskFilterConfig_Struct mask_filters[] = \
	{ \
		INIT_MASK_FILTER(IDMASK_16BIT_FILTER_ID_DCM, IDMASK_16BIT_FILTER_MASK_DCM) \
	};
    CAN_TxHeaderTypeDef can_headers[] = \
    {
        INIT_CAN_HEADER(0x00, 6),
        INIT_CAN_HEADER(0x01, 7)
    };

#endif

void SharedCAN_InitializeCanMaskFilters(void)
{
	/*#ifdef BMS
		mask_filters[0].id = IDMASK_16BIT_FILTER_ID_DCM;
		mask_filters[0].mask = IDMASK_16BIT_FILTER_MASK_DCM;
		mask_filters[1].id = IDMASK_16BIT_FILTER_ID_FSM;
		mask_filters[1].mask = IDMASK_16BIT_FILTER_MASK_FSM;
		mask_filters[2].id = IDMASK_16BIT_FILTER_ID_PDM;
		mask_filters[2].mask = IDMASK_16BIT_FILTER_MASK_PDM;
	#elif DCM
		mask_filters[0].id = IDMASK_16BIT_FILTER_ID_BMS;
		mask_filters[0].mask = IDMASK_16BIT_FILTER_ID_BMS;
		mask_filters[0].id = IDMASK_16BIT_FILTER_ID_FSM;
		mask_filters[0].mask = IDMASK_16BIT_FILTER_ID_FSM;
	#elif PDM
	*/
	//#endif
}


/**
 * @brief  Transmit CAN message and remove it from the CAN queue
 * @param  None
 * @return FIFO_IS_EMPTY Failed dequeue due to empty queue 
 *         FIFO_SUCCESS Successful dequeue
 */
Fifo_Status_Enum SharedCAN_DequeueCanTxMessageFifo(void)
{
	if(!SharedCAN_CanTxMessageFifoIsEmpty())
	{
		// Transmit one CAN message in queue
		SharedCAN_TransmitDataCAN(&can_tx_msg_fifo[tail].TxHeader, can_tx_msg_fifo[tail].Data);

		// Remove the transmitted CAN message from queue
		memset(&can_tx_msg_fifo[tail], 0, sizeof(can_tx_msg_fifo[tail]));

		// Increment tail and make sure it wraps around to 0
		tail++;
		if(tail == CAN_TX_MSG_FIFO_SIZE)
		{
			tail = 0;
		}
		return FIFO_SUCCESS;
	}
	else
	{
		return FIFO_IS_EMPTY;
	}

}

/**
 * @brief  Add CAN message overflow to CAN queue
 * @param  CANMsg Pointer of CAN message to be queued
 * @return FIFO_IS_FULL Failed enqueue due to full queue
 *         FIFO_SUCCESS Successful enqueue 
 */
Fifo_Status_Enum SharedCAN_EnqueueCanTxMessageFifo(CanTxMsg_Struct* CANMsg)
{
	if(!SharedCAN_CanTxMessageFifoIsFull())
	{
		// Add CAN message to queue
		can_tx_msg_fifo[head] = *CANMsg; 

		// Increment head and make sure it wraps around to 0
		head++;
		if(head == CAN_TX_MSG_FIFO_SIZE)
		{
			head = 0;
		}
		return FIFO_SUCCESS;
	}
	else
	{
		return FIFO_IS_FULL;
	}
}

/**
 * @brief  Clear the CAN queue
 * @param  None 
 * @return None
 */
void SharedCAN_ClearCanTxMessageFifo(void)
{
	memset(can_tx_msg_fifo, 0, sizeof(can_tx_msg_fifo));
}

/**
 * @brief  Check if the CAN queue is full 
 * @param  None
 * @return 0 CAN queue is not full
 *         1 CAN queue is full
 */
uint32_t SharedCAN_CanTxMessageFifoIsFull(void)
{
	return ((head + 1) % CAN_TX_MSG_FIFO_SIZE) == tail;
}

/**
 * @brief  Check if the CAN queue is empty 
 * @param  None 
 * @return 0 CAN queue is not empty
 *         1 CAN queue is empty
 */
uint32_t SharedCAN_CanTxMessageFifoIsEmpty(void)
{
	return head == tail;
}

/**
 * @brief  Get the number of messages saved in the CAN queue
 * @param  None
 * @return Number of messages in the queue
 */
uint32_t SharedCAN_GetNumberOfItemsInCanTxMessageFifo(void)
{
	uint32_t MessageCount;

	if(head >= tail)
	{
		MessageCount = head - tail;
	}
	else
	{
		MessageCount = CAN_TX_MSG_FIFO_SIZE - tail + head;
	}
	return MessageCount;
}

/**
 * @brief  Broadcasts a heartbeat message on the CAN bus
 * @param  Module Name of PCB 
 * @return None
 */
void SharedCAN_BroadcastHeartbeat(PCB_Enum Module)
{
	//TODO: Implement when CAN Header structure is finalized
	//uint8_t Data[8] = {Module};
	//SharedCAN_TransmitDataCAN(can_headers[HEARTBEAT], Data);
}

/**
 * @brief  Transmits a CAN message 
 * @param  TxHeader CAN Tx header struct
 * @param  Data Data to be transmitted (up to 8 bytes)
 * @return None
 */ 
void SharedCAN_TransmitDataCAN(CAN_TxHeaderTypeDef *TxHeader, uint8_t * Data)
{
	#ifdef DEBUG
	static uint8_t OverflowCount; // Debug variable to keep track of overflow before a proper handling strategy is developed
	static uint8_t TransmitErrorCount; // Debug variable to keep track of transmit errors before a proper handling strategy is developed
	#endif
	uint32_t Mailbox = 0; // Indicates the mailbox used for tranmission, not currently used 

	// If a mailbox is not available or other error occurs
	if(HAL_CAN_AddTxMessage(&hcan, TxHeader, Data, &Mailbox) != HAL_OK)
	{
		// Populate CAN TX message with CAN header and data
		CanTxMsg_Struct queueMsg;
		queueMsg.TxHeader = *TxHeader;
		memcpy(&queueMsg.Data, Data,CAN_PAYLOAD_SIZE);

		if(SharedCAN_EnqueueCanTxMessageFifo(&queueMsg) != FIFO_SUCCESS) 
		{
			#ifdef DEBUG
			// TODO: Handle CAN queue overflow error
			OverflowCount++;
			#endif
		}
		#ifdef DEBUG
		//If error was not caused by full mailboxes
		if((hcan.ErrorCode & HAL_CAN_ERROR_PARAM) != HAL_CAN_ERROR_PARAM) 
		{
			TransmitErrorCount++;
		}
		#endif
	}	
}

/**
 * @brief Initialize CAN filter 
 * @param Board
 * @return None
 */
ErrorStatus SharedCAN_InitializeFilters(void)
{
	static uint32_t filter_bank = 0;
	static uint32_t fifo = CAN_FILTER_FIFO0;
	uint32_t num_of_filters = sizeof(mask_filters) / sizeof(mask_filters[0]);
	uint32_t is_odd = num_of_filters % 2;

	CAN_FilterTypeDef can_filter;

	// Initialize two 16-bit filters for each filter bank
	for (uint32_t i = 0; i < num_of_filters / 2; i ++)
	{
		// Configure filter settings 
		can_filter.FilterIdLow = mask_filters[i].id;
		can_filter.FilterMaskIdLow = mask_filters[i].mask;
		can_filter.FilterIdHigh = mask_filters[i + 1].id;
		can_filter.FilterMaskIdHigh = mask_filters[i + 1].mask;
		can_filter.FilterFIFOAssignment = fifo;
		can_filter.FilterBank = filter_bank;
		can_filter.FilterMode = CAN_FILTERMODE_IDMASK;
		can_filter.FilterScale = CAN_FILTERSCALE_16BIT;
		can_filter.FilterActivation = CAN_FILTER_ENABLE;

		// Alternate between the two FIFOs
		fifo = !fifo;	

		// Update filter bank for next iteration
		filter_bank = filter_bank + 1;

		// Configure and initialize filter bank
		if (HAL_CAN_ConfigFilter(&hcan, &can_filter) != HAL_OK)
		{
			return ERROR;
		}
	}

	// For the odd-one-out filter, initialize two identical 16-bit filters 
	// because we each filter bank requires two 16-bit filters
	if (is_odd)
	{
		// Configure filter settings 
		uint32_t last_filter_index = num_of_filters - 1;
		can_filter.FilterIdLow = mask_filters[last_filter_index].id;
		can_filter.FilterMaskIdLow = mask_filters[last_filter_index].mask;
		can_filter.FilterIdHigh = mask_filters[last_filter_index].id;
		can_filter.FilterMaskIdHigh = mask_filters[last_filter_index].mask;
		can_filter.FilterFIFOAssignment = fifo;
		can_filter.FilterBank = filter_bank;
		can_filter.FilterMode = CAN_FILTERMODE_IDMASK;
		can_filter.FilterScale = CAN_FILTERSCALE_16BIT;
		can_filter.FilterActivation = CAN_FILTER_ENABLE;

		// Configure and initialize filter bank
		if (HAL_CAN_ConfigFilter(&hcan, &can_filter) != HAL_OK)
		{
			return ERROR;
		}
	}

	return SUCCESS;
}



/**
 * @brief  Initialize a single CAN header
 * @param  can_ids_enum This parameter can be a value of @ref CanIds_enum
 * @param  std_id Standard CAN ID
 * @param  DLC Data length Code 
 * @return None
 */ 
void SharedCAN_InitializeHeader(int can_ids_enum, uint32_t std_id, uint32_t DLC)
{	
	can_headers[can_ids_enum].StdId = std_id;
	can_headers[can_ids_enum].ExtId = CAN_ExtID_NULL;
	can_headers[can_ids_enum].IDE = CAN_ID_STD;
	can_headers[can_ids_enum].RTR = CAN_RTR_DATA;
	can_headers[can_ids_enum].DLC = DLC;
	can_headers[can_ids_enum].TransmitGlobalTime = DISABLE;
}

/**
 * @brief  Initialize all the CAN headers
 * @param  None
 * @return None
 */ 
void SharedCAN_InitializeHeaders(void){
	SharedCAN_InitializeHeader(BMS_ERROR, 0x00, 8);
	SharedCAN_InitializeHeader(BMS_HEARTBEAT, 0x01, 1);
	SharedCAN_InitializeHeader(BMS_ACCUMULATOR_STATUS, 0x02, 3);
	SharedCAN_InitializeHeader(BMS_ACCUMULATOR_DISCHARGE_POWER_LIMIT, 0x03, 4);
	SharedCAN_InitializeHeader(BMS_ACCUMULATOR_REGEN_POWER_LIMIT, 0x04, 4);
	SharedCAN_InitializeHeader(BMS_ACCUMULATOR_SOC, 0x05, 4);
	SharedCAN_InitializeHeader(BMS_TURN_ON_ALL_FETS, 0x06, 1);
	SharedCAN_InitializeHeader(BMS_CELL_VOLTAGE, 0x07, 4);
	SharedCAN_InitializeHeader(BMS_BALANCE_MOSFET, 0x08, 5);
	SharedCAN_InitializeHeader(BMS_LATCH_RESET, 0x09, 1);
	SharedCAN_InitializeHeader(BMS_MOTOR_REENABLE, 0x0A, 1);
	SharedCAN_InitializeHeader(BMS_AIR_SHUTDOWN_ERROR, 0x0B, 6);
	SharedCAN_InitializeHeader(DCM_ERROR, 0x20, 8);
	SharedCAN_InitializeHeader(DCM_HEARTBEAT, 0x21, 1);
	SharedCAN_InitializeHeader(DCM_LEFT_SLIP_RATIO, 0x22, 4);
	SharedCAN_InitializeHeader(DCM_RIGHT_SLIP_RATIO, 0x23, 4);
	SharedCAN_InitializeHeader(DCM_LEFT_MOTOR_OUTPUT, 0x24, 4);
	SharedCAN_InitializeHeader(DCM_RIGHT_MOTOR_OUTPUT, 0x25, 4);
	SharedCAN_InitializeHeader(DCM_LEFT_SLIP_ERROR, 0x26, 4);
	SharedCAN_InitializeHeader(DCM_RIGHT_SLIP_ERROR, 0x27, 4);
	SharedCAN_InitializeHeader(FSM_ERROR, 0x40, 8);
	SharedCAN_InitializeHeader(FSM_HEARTBEAT, 0x41, 1);
	SharedCAN_InitializeHeader(FSM_BRAKE_APPS, 0x42, 4);
	SharedCAN_InitializeHeader(FSM_STEERING_ANGLE, 0x43, 4);
	SharedCAN_InitializeHeader(FSM_LEFT_WHEEL_SPEED, 0x44, 4);
	SharedCAN_InitializeHeader(FSM_RIGHT_WHEEL_SPEED, 0x45, 4);
	SharedCAN_InitializeHeader(PDM_ERROR, 0x60, 8);
	SharedCAN_InitializeHeader(PDM_HEARTBEAT, 0x61, 1);
	SharedCAN_InitializeHeader(PDM_STARTUP_STATUS, 0x62, 1);
	SharedCAN_InitializeHeader(MOTOR_SHUTDOWN_ERROR, 0x80, 6);
	SharedCAN_InitializeHeader(LEFT_TORQUE_REQUEST_TX, 0x190, 3);
	SharedCAN_InitializeHeader(RIGHT_TORQUE_REQUEST_TX, 0x191, 3);
	SharedCAN_InitializeHeader(LEFT_TORQUE_REQUEST_RX, 0x210, 3);
	SharedCAN_InitializeHeader(RIGHT_TORQUE_REQUEST_RX, 0x211, 3);
}

/**
 * @brief  Shared callback function for Rx FIFO 0 and 1
 * @param  hcan pointer to a CAN_HandleTypeDef structure that contains
 *         the configuration information for the specified CAN.
 * @param  RxFifo The Rx FIFO that triggered the callback
 * @return None
 */ 
__weak void CAN_RxCommonCallback(CAN_HandleTypeDef * hcan, uint32_t RxFifo){
    /* NOTE : This function Should not be modified, when the callback is needed,
              the CAN_RxCommonCallback could be implemented in the CAN.c file
    */
}
/**
 * @brief  Rx FIFO 0 message pending callback.
 * @param  hcan pointer to a CAN_HandleTypeDef structure that contains
 *         the configuration information for the specified CAN.
 * @retval None
 */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan){
	CAN_RxCommonCallback(hcan, CAN_RX_FIFO0);
}

/**
 * @brief  Rx FIFO 1 message pending callback.
 * @param  hcan pointer to a CAN_HandleTypeDef structure that contains
 *         the configuration information for the specified CAN.
 * @retval None
 */
void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan){
	CAN_RxCommonCallback(hcan, CAN_RX_FIFO1);
}

/**
 * @brief  Shared callback function for transmission mailbox 0, 1, and 2
 * @param  hcan pointer to a CAN_HandleTypeDef structure that contains
 *         the configuration information for the specified CAN.
 * @param  RxFifo The Rx FIFO that triggered the callback
 * @return None
 */ 
__weak void CAN_TxCommonCallback(CAN_HandleTypeDef * hcan){
    /* NOTE : This function Should not be modified, when the callback is needed,
              the CAN_RxCommonCallback could be implemented in the CAN.c file
    */

}
/**
 * @brief  Transmission Mailbox 0 complete callback.
 * @param  hcan pointer to a CAN_HandleTypeDef structure that contains
 *         the configuration information for the specified CAN.
 * @retval None
 */
void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef *hcan){
	CAN_TxCommonCallback(hcan);
}

/**
 * @brief  Transmission Mailbox 1 complete callback.
 * @param  hcan pointer to a CAN_HandleTypeDef structure that contains
 *         the configuration information for the specified CAN.
 * @retval None
 */
void HAL_CAN_TxMailbox1CompleteCallback(CAN_HandleTypeDef *hcan){
	CAN_TxCommonCallback(hcan);
}

/**
 * @brief  Transmission Mailbox 2 complete callback.
 * @param  hcan pointer to a CAN_HandleTypeDef structure that contains
 *         the configuration information for the specified CAN.
 * @retval None
 */
void HAL_CAN_TxMailbox2CompleteCallback(CAN_HandleTypeDef *hcan){
	CAN_TxCommonCallback(hcan);
}

