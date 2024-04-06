#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "hw_hal.h"

#define CAN_PAYLOAD_BYTES 8

typedef struct
{
    uint32_t std_id;
    uint32_t dlc; // data length range : [0, 8]
    uint8_t  data[CAN_PAYLOAD_BYTES];
} CanMsg;

typedef void (*MsgReceivedCallback)(CanMsg *rx_msg);

#ifdef CANFD
// STM32 HAL CAN FD handle.
typedef struct
{
    FDCAN_HandleTypeDef *can;
    MsgReceivedCallback  can_msg_received_callback;
} CanHandle;
#else
// STM32 HAL CAN handle.
typedef struct
{
    CAN_HandleTypeDef  *can;
    MsgReceivedCallback can_msg_received_callback;
} CanHandle;
#endif

/**
 * Initialize CAN driver.
 * @param can_handle STM32 HAL CAN handle.
 */
void hw_can_init(const CanHandle *can_handle);

/**
 * Stop and deinitialize the CAN peripheral.
 */
void hw_can_deinit(void);

/**
 * Transmit a CAN msg on the bus, blocking until completed.
 * @param msg CAN msg to be TXed.
 * @return Whether or not the transmission was successful.
 */
bool hw_can_transmit(const CanMsg *msg);

/**
 * Receive a CAN msg from the bus, returning whether or not a message is available.
 * @param msg CAN msg to be RXed.
 * @param rx_fifo Which RX FIFO to receive a message from.
 * @return Whether or not the reception was successful.
 */
bool hw_can_receive(uint32_t rx_fifo, CanMsg *msg);
