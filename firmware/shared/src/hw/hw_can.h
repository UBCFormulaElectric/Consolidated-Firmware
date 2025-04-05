#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "io_canMsg.h"
#include "main.h"

#ifdef CANFD
#define STM32_CAN_HANDLE FDCAN_HandleTypeDef
#else
#define STM32_CAN_HANDLE CAN_HandleTypeDef
#endif

typedef struct
{
    STM32_CAN_HANDLE *const hcan;
    const uint8_t           bus_num; // TODO change this to jsoncan bus enum when jiajun is done
    void (*const receive_callback)(const CanMsg *rx_msg);
    bool ready;
} CanHandle;

/**
 * @attention THIS MUST BE DEFINED IN YOUR CONFIGURATIONS
 * @param hcan takes a handle to a STM32 HAL CAN object
 * @returns a pointer to a CanHandle object (the metadata associated with the STM32 HAL CAN object)
 */
const CanHandle *hw_can_getHandle(const STM32_CAN_HANDLE *hcan);

/**
 * Initialize CAN driver.
 * @param can_handle STM32 HAL CAN handle.
 */
void hw_can_init(CanHandle *can_handle);

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
