#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "hw_hal.h"

#define CAN_PAYLOAD_BYTES 8

typedef struct
{
    uint32_t std_id;
    uint32_t dlc;
    uint8_t  data[CAN_PAYLOAD_BYTES];
} CanMsg;

#ifdef CANFD
// STM32 HAL CAN FD handle.
typedef FDCAN_HandleTypeDef CanHandle;
#else
// STM32 HAL CAN handle.
typedef CAN_HandleTypeDef CanHandle;
#endif

/**
 * Initialize CAN driver.
 * @param can_handle STM32 HAL CAN handle.
 */
void hw_can_init(CanHandle * can_handle);

/**
 * Stop and deinitialize the CAN peripheral.
 */
void hw_can_deinit(void);

/**
 * Transmit a CAN msg on the bus, blocking until completed.
 * @param msg CAN msg to be TXed.
 * @return Whether or not the transmission was successful.
 */
bool hw_can_transmit(const CanMsg * msg);

/**
 * Receive a CAN msg from the bus, returning whether or not a message is available.
 * @param msg CAN msg to be RXed.
 * @param rx_fifo Which RX FIFO to receive a message from.
 * @return Whether or not the reception was successful.
 */
bool hw_can_receive(uint32_t rx_fifo, CanMsg * msg);
