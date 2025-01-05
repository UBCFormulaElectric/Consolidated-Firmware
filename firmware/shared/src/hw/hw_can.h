#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "hw_hal.h"
#include "io_canMsg.h"

#ifdef CANFD
// STM32 HAL CAN FD handle.
typedef struct
{
    FDCAN_HandleTypeDef *hcan;
} CanHandle;
#else
// STM32 HAL CAN handle.
typedef struct
{
    CAN_HandleTypeDef *hcan;
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
void hw_can_deinit(const CanHandle *can_handle);

/**
 * Transmit a CAN msg on the bus, blocking until completed.
 * @param can_handle Can handle to transmit from
 * @param msg CAN msg to be TXed.
 * @return Whether or not the transmission was successful.
 */
bool hw_can_transmit(const CanHandle *can_handle, CanMsg *msg);

/**
 * Receive a CAN msg from the bus, returning whether or not a message is available.
 * This function also passes up the CanMsg to a callback function.
 * @param can_handle Can handle to receive from
 * @param msg CAN msg to be RXed.
 * @param rx_fifo Which RX FIFO to receive a message from.
 * @return Whether or not the reception was successful.
 */
bool hw_can_receive(const CanHandle *can_handle, uint32_t rx_fifo, CanMsg *msg);
