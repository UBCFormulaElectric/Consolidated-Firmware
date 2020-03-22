/**
 * @brief CAN controller library for STM32F0 and STM32F3
 */

#ifndef SHARED_CAN_H
#define SHARED_CAN_H

/******************************************************************************
 * Includes
 ******************************************************************************/
#include "auto_generated/App_CanTx.h"
#include "Io_SharedCanMsg.h"

// Check for STM32 microcontroller family
#ifdef STM32F302x8
// Used in DCM 2017, BMS 2017, and PDM 2018
#include "stm32f3xx_hal.h"
#elif STM32F042x6
// Used in FSM 2017 (Shared CAN Library doesn't yet support this)
#include "stm32f0xx_hal.h"
#else
#error \
    "No valid architecture selected - unable to determine what HAL library to use"
#endif

/******************************************************************************
 * Fixes to allow us to use this library with STM32F0xx Boards
 ******************************************************************************/
#ifdef STM32F042x6
typedef CanRxMsgTypeDef CAN_RxHeaderTypeDef;
typedef CanTxMsgTypeDef CAN_TxHeaderTypeDef;
#endif

/******************************************************************************
 * Preprocessor Constants
 ******************************************************************************/
// clang-format off
#define CAN_PAYLOAD_MAX_NUM_BYTES 8 // Maximum number of bytes in a CAN payload
#define CAN_ExtID_NULL 0 // Set CAN Extended ID to 0 because we are not using it

/******************************************************************************
 * Typedefs
 ******************************************************************************/
// clang-format on

/******************************************************************************
 * Function Prototypes
 ******************************************************************************/
/**
 * @brief  Initialize CAN interrupts before starting the CAN
 *         module. After this, the node is active on the bus: it receive
 *         messages, and can send messages. This should be placed inside
 *         MX_CAN_Init() and in the USER CODE BLOCK after HAL_CAN_Init().
 * @param hcan Pointer to a CAN_HandleTypeDef structure that contains
 *         the configuration information for the specified CAN.
 */
void Io_SharedCan_Init(CAN_HandleTypeDef *hcan);

/**
 * @brief Send a message to the back of the CAN TX queue
 * @param message CAN message to send
 */
void Io_SharedCan_TxMessageQueueSendtoBack(struct CanMsg *message);

/**
 * @brief For messages that we couldn't handle in ISR context, read them into
 *        memory at the task level.
 */
void Io_SharedCan_ReadRxMessagesIntoTableFromTask(void);

/**
 * @brief Transmit messages in the CAN TX queue over CAN bus
 */
void Io_SharedCan_TransmitEnqueuedCanTxMessagesFromTask(void);

#endif /* SHARED_CAN_H */
