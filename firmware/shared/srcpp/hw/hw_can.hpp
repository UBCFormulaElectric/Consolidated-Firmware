#pragma once

#include "io_canMsg.hpp"
#include "util_errorCodes.hpp"
#include "hw_hal.h"
#include "cmsis_os.h"

// STM32 HAL CAN handle.
#if defined(STM32H733xx) || defined(STM32H562xx)
    // STM32 HAL CAN FD handle.
    typedef FDCAN_HandleTypeDef USEDCAN_HandleTypeDef;
    typedef FDCAN_TxHeaderTypeDef USEDCAN_TxHeaderTypeDef;
#elif defined(STM32F412Rx)
    // STM32 HAL CAN handle.
    typedef CAN_HandleTypeDef USEDCAN_HandleTypeDef;
    typedef CAN_TxHeaderTypeDef USEDCAN_TxHeaderTypeDef;
#endif

namespace hw
{
class can
{
    USEDCAN_HandleTypeDef *const hcan;
    const uint8_t            bus_num;
    mutable bool             ready = false;
  private:
    ExitCode tx(USEDCAN_TxHeaderTypeDef &tx_header,io::CanMsg *msg);
  public:
    void (*const receive_callback)(const io::CanMsg *rx_msg);
    TaskHandle_t transmit_task = nullptr;

    constexpr explicit can(
        USEDCAN_HandleTypeDef &hcan_in,
        const uint8_t      bus_num_in,
        void (*const receive_callback_in)(const io::CanMsg *rx_msg))
      : hcan(&hcan_in), bus_num(bus_num_in), receive_callback(receive_callback_in){};

    /**
     * Initialize CAN driver.
     * @param can_handle STM32 HAL CAN handle.
     */
    void init() const;

    /**
     * Stop and deinitialize the CAN peripheral.
     */
    void deinit() const;

    /**
     * Transmit a CAN msg on the bus, blocking until completed.
     * @param can_handle Can handle to transmit from
     * @param msg CAN msg to be TXed.
     * @return Whether or not the transmission was successful.
     */
    ExitCode can_transmit(const io::CanMsg &msg);


    /**
     * Transmit a FD CAN msg on the bus, blocking until completed.
     * @param can_handle Can handle to transmit from  
     * @param msg FD CAN msg to be TXed.
     * @return Whether or not the transmission was successful.
     */

    ExitCode fdcan_transmit(const io::CanMsg &msg);

    /**
     * Receive a CAN msg from the bus, returning whether or not a message is available.
     * This function also passes up the CanMsg to a callback function.
     * @param can_handle Can handle to receive from
     * @param msg CAN msg to be RXed.
     * @param rx_fifo Which RX FIFO to receive a message from.
     * @return Whether or not the reception was successful.
     */
    ExitCode receive(uint32_t rx_fifo, io::CanMsg &msg) const;

    constexpr USEDCAN_HandleTypeDef *getHcan() const { return hcan; }
    constexpr uint8_t               getBusNum() const { return bus_num; }
};

/**
 * @attention THIS MUST BE DEFINED IN YOUR CONFIGURATIONS
 * @param hcan takes a handle to a STM32 HAL CAN object
 * @returns a pointer to a CanHandle object (the metadata associated with the STM32 HAL CAN object)
 */
const can &hw_can_getHandle(const USEDCAN_HandleTypeDef *hcan);
} // namespace hw