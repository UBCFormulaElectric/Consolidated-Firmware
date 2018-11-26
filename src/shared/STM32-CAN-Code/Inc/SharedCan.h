/**
	@file	   	SharedCan.h
	@brief	  	Function library header for the CAN controllers on the STM32F0 and STM32F3
	@author	 	UBC Formula Electric
	@version	v1.0.00
	@copyright  GNU General Public License v3
*/

#ifndef SHAREDCAN_H
#define SHAREDCAN_H

#include "main.h"
#include "CANDefinitions.h"

#ifdef STM32F302x8
#include "stm32f3xx_hal.h"
#endif

#ifdef STM32F042x6 
#include "stm32f0xx_hal.h"
#endif
#define CAN_PAYLOAD_SIZE 8 // Maximum number of bytes in a CAN payload

#define CAN_ExtID_NULL 0

// Warning: The following filter IDs/masks must be used with 16-bit Filter Scale and Identifier Mask Mode (FSCx = 0, FBMx = 0):
// Create BMS filter - ID: 0x00 - 0x1F, RTR: Data Frame, IDE: Standard ID
#define IDMASK_16BIT_FILTER_ID_BMS		  (uint32_t)(0x0000)	// 0000 0000 0000 0000
#define IDMASK_16BIT_FILTER_MASK_BMS	  (uint32_t)(0xFC18)	// 1111 1100 0001 1000

// Create DCM filter - ID: 0x20 - 0x3F, RTR: Data Frame, IDE: Standard ID
#define IDMASK_16BIT_FILTER_ID_DCM		  (uint32_t)(0x0400)	// 0000 0100 0000 0000
#define IDMASK_16BIT_FILTER_MASK_DCM	  (uint32_t)(0xFC18)	// 1111 1100 0001 1000

// Create FSM filter - ID: 0x40 - 0x5F, RTR: Data Frame, IDE: Standard ID
#define IDMASK_16BIT_FILTER_ID_FSM		  (uint32_t)(0x0800)	// 0000 1000 0000 0000
#define IDMASK_16BIT_FILTER_MASK_FSM	  (uint32_t)(0xFC18)	// 1111 1100 0001 1000

// Create PDM filter - ID: 0x60 - 0x7F, RTR: Data Frame, IDE: Standard ID
#define IDMASK_16BIT_FILTER_ID_PDM		  (uint32_t)(0x0C00) 	// 0000 1100 0000 0000
#define IDMASK_16BIT_FILTER_MASK_PDM	  (uint32_t)(0xFC18) 	// 1111 1100 0001 1000

// Create shared filter - ID: 0x80 - 0x9F, RTR: Data Frame, IDE: Standard ID
#define IDMASK_16BIT_FILTER_ID_SHARED     (uint32_t)(0x0C00) 	// 0000 1100 0000 0000
#define IDMASK_16BIT_FILTER_MASK_SHARED	  (uint32_t)(0xFC18) 	// 1111 1100 0001 1000

// TODO: Create BACMO Filter - ID: 0x190 - 0x211, RTR: Data Frame, IDE: Standard ID
#define IDMASK_16BIT_FILTER_ID_BAMOCAR    (uint32_t)(0x0C00) 	// 0000 1100 0000 0000
#define IDMASK_16BIT_FILTER_MASK_BAMOCAR  (uint32_t)(0xFC18) 	// 1111 1100 0001 1000


typedef struct {
	uint32_t id;
	uint32_t mask;
} CanMaskFilterConfig_Struct;




typedef enum
{
	BMS_CAN_FILTER = 0,
	DCM_CAN_FILTER = 1,
	FSM_CAN_FILTER = 2,
	PDM_CAN_FILTER = 3,
	SHARED_CAN_FILTER = 4,
	BAMOCAR_CAN_FILTER = 5
} CanFilter_Enum;



// Index for array of CAN headers
typedef enum {
	// BMS CAN IDs
	BMS_ERROR = 0,
	BMS_HEARTBEAT,
	BMS_ACCUMULATOR_STATUS,
	BMS_ACCUMULATOR_DISCHARGE_POWER_LIMIT,
	BMS_ACCUMULATOR_REGEN_POWER_LIMIT,
	BMS_ACCUMULATOR_SOC,
	BMS_TURN_ON_ALL_FETS,
	BMS_BALANCE_MOSFET,
	BMS_CELL_VOLTAGE,
	BMS_LATCH_RESET,
	BMS_MOTOR_REENABLE,
	BMS_AIR_SHUTDOWN_ERROR,

	// DCM CAN IDs
	DCM_ERROR,
	DCM_HEARTBEAT,
	DCM_LEFT_SLIP_RATIO,
	DCM_RIGHT_SLIP_RATIO,
	DCM_LEFT_MOTOR_OUTPUT,
	DCM_RIGHT_MOTOR_OUTPUT,
	DCM_LEFT_SLIP_ERROR,
	DCM_RIGHT_SLIP_ERROR,

	// FSM CAN IDs
	FSM_ERROR,
	FSM_HEARTBEAT,
	FSM_BRAKE_APPS,
	FSM_STEERING_ANGLE,
	FSM_LEFT_WHEEL_SPEED,
	FSM_RIGHT_WHEEL_SPEED,

	// PDM CAN IDs
	PDM_ERROR,
	PDM_HEARTBEAT,
	PDM_STARTUP_STATUS,

	// Shared CAN IDs
	MOTOR_SHUTDOWN_ERROR,
	
	// BAMOCAR D3 CAN IDs
	LEFT_TORQUE_REQUEST_TX, // BAMOCAR D3 transmits on TX
	RIGHT_TORQUE_REQUEST_TX, // BAMOCAR D3 transmits on TX 
	LEFT_TORQUE_REQUEST_RX, // BAMOCAR D3 receives on RX
	RIGHT_TORQUE_REQUEST_RX, // BAMOCAR D3 receives on RX

	// Number of CAN IDS
	CAN_NODES_COUNT
} CanIds_Enum;


// CAN headers
extern CAN_TxHeaderTypeDef can_headers[CAN_NODES_COUNT];

/**
 * @brief Combine HAL Tx CAN header with CAN Payload
 */
typedef struct
{
	CAN_TxHeaderTypeDef TxHeader;
	uint8_t Data[8]; 
	
} CanTxMsg_Struct;

/**
 * @brief Combine HAL Rx CAN header with CAN payload
 */
typedef struct
{
	CAN_RxHeaderTypeDef RxHeader;
	uint8_t Data[8]; 
	
} CanRxMsg_Struct;

//CAN Handler
extern CAN_HandleTypeDef hcan;

// Queue operation status code
typedef enum
{
	FIFO_SUCCESS = 0,
	FIFO_IS_FULL = 1,
	FIFO_IS_EMPTY = 2,
	FIFO_ERROR = 3
} Fifo_Status_Enum;

// Function declarations
Fifo_Status_Enum SharedCAN_DequeueCanTxMessageFifo(void);
Fifo_Status_Enum SharedCAN_EnqueueCanTxMessageFifo(CanTxMsg_Struct *CANMsg);
void SharedCAN_ClearCanTxMessageFifo(void);
uint32_t SharedCAN_CanTxMessageFifoIsFull(void);
uint32_t SharedCAN_CanTxMessageFifoIsEmpty(void);
uint32_t SharedCAN_GetNumberOfItemsInCanTxMessageFifo(void);
void SharedCAN_BroadcastHeartbeat(PCB_Enum Module);
void SharedCAN_TransmitDataCAN(CAN_TxHeaderTypeDef *TxHeader, uint8_t *Data);
void SharedCAN_InitializeHeaders(void);
void CAN_RxCommonCallback(CAN_HandleTypeDef *hcan, uint32_t RxFifo);
void CAN_TxCommonCallback(CAN_HandleTypeDef *hcan);

#endif
