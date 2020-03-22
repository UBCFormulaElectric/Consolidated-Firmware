#pragma once

#include <stm32f3xx_hal.h>

#include "auto_generated/App_CanTx.h"
#include "Io_SharedCanMsg.h"

// clang-format off
#define CAN_PAYLOAD_MAX_NUM_BYTES 8 // Maximum number of bytes in a CAN payload
#define CAN_ExtID_NULL 0 // Set CAN Extended ID to 0 because we are not using it

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
