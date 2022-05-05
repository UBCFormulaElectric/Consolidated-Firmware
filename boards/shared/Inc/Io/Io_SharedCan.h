#pragma once

#include <stm32f3xx_hal.h>

#include "App_CanTx.h"
#include "Io_SharedCanMsg.h"

#define CAN_PAYLOAD_MAX_NUM_BYTES 8 // Maximum number of bytes in a CAN payload
#define CAN_ExtID_NULL 0            // Set CAN Extended ID to 0 because we are not using it

/**
 * Initialize CAN interrupts before starting the CAN module. After this, the
 * node is active on the bus: it receive messages, and can send messages. This
 * should be placed inside MX_CAN_Init() and in the USER CODE BLOCK after
 * HAL_CAN_Init().
 * @param hcan Pointer to a CAN_HandleTypeDef structure that contains
 *         the configuration information for the specified CAN.
 * @param tx_overflow_callback A function that will be called with the current
 *                             overflow count when the tx queue overflows
 * @param rx_overflow_callback A function that will be called with the current
 *                             overflow count when the rx queue overflows
 */
void Io_SharedCan_Init(
    CAN_HandleTypeDef *hcan,
    void (*tx_overflow_callback)(size_t),
    void (*rx_overflow_callback)(size_t));

/**
 * Send a message to the back of the CAN TX queue
 * @param message CAN message to send
 */
void Io_SharedCan_TxMessageQueueSendtoBack(const struct CanMsg *message);

/**
 * Read a message from the CAN RX queue
 * @note If there is no message in the CAN RX queue, this function will block
 *       indefinitely until a message becomes available
 */
void Io_SharedCan_DequeueCanRxMessage(struct CanMsg *message);

/**
 * Transmit messages in the CAN TX queue over CAN bus
 */
void Io_SharedCan_TransmitEnqueuedCanTxMessagesFromTask(void);
