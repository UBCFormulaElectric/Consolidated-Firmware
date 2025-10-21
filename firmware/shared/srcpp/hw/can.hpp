#pragma once

#include "io/canMsg.hpp"
#include "util/errorCodes.hpp"
#include "hw/hal.h"
#include "cmsis_os.h"

// STM32 HAL CAN handle.
namespace hw
{
class can
{
    CAN_HandleTypeDef *const hcan;
    const uint8_t            bus_num;
    mutable bool             ready = false;

  public:
    void (*const receive_callback)(const io::CanMsg *rx_msg);
    TaskHandle_t transmit_task = nullptr;

    constexpr explicit can(
        CAN_HandleTypeDef &hcan_in,
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
    ExitCode transmit(const io::CanMsg &msg);

    /**
     * Receive a CAN msg from the bus, returning whether or not a message is available.
     * This function also passes up the CanMsg to a callback function.
     * @param can_handle Can handle to receive from
     * @param msg CAN msg to be RXed.
     * @param rx_fifo Which RX FIFO to receive a message from.
     * @return Whether or not the reception was successful.
     */
    ExitCode receive(uint32_t rx_fifo, io::CanMsg &msg) const;

    constexpr CAN_HandleTypeDef *getHcan() const { return hcan; }
};

/**
 * @attention THIS MUST BE DEFINED IN YOUR CONFIGURATIONS
 * @param hcan takes a handle to a STM32 HAL CAN object
 * @returns a pointer to a CanHandle object (the metadata associated with the STM32 HAL CAN object)
 */
const can &hw_can_getHandle(const CAN_HandleTypeDef *hcan);
} // namespace hw

// namespace hw::can
// {
// #define CAN_PAYLOAD_BYTES 8
//
// struct CanMsg
// {
//     uint32_t std_id;
//     uint32_t dlc; // data length range : [0, 8]
//     uint8_t  data[CAN_PAYLOAD_BYTES];
// };
//
// class CanBus
// {
//     void (*const can_msg_received_callback)(const CanMsg *rx_msg);
//
// #ifdef TARGET_EMBEDDED
// #ifdef CANFD
//     // STM32 HAL CAN FD handle.
//     typedef FDCAN_HandleTypeDef USEDCAN_HandleTypeDef;
// #else
//     // STM32 HAL CAN handle.
//     typedef CAN_HandleTypeDef USEDCAN_HandleTypeDef;
// #endif
//   private:
//     USEDCAN_HandleTypeDef *const handle;
//
//   public:
//     /**
//      * Initialize CAN driver.
//      * @param can_handle STM32 HAL CAN handle.
//      * @param can_msg_received_callback Callback function to be called when a CAN message is received.
//      */
//     explicit CanBus(USEDCAN_HandleTypeDef *can_handle_in, void (*can_msg_received_callback_in)(const CanMsg *))
//       : can_msg_received_callback(can_msg_received_callback_in), handle(can_handle_in)
//     {
//     }
// #endif
//   public:
//     /**
//      * Stop and deinitialize the CAN peripheral.
//      */
//     ~CanBus();
//
//     void init() const;
//
//     /**
//      * Transmit a CAN msg on the bus, blocking until completed.
//      * @param msg CAN msg to be TXed.
//      * @return Whether or not the transmission was successful.
//      */
//     bool transmit(const CanMsg *msg) const;
//     /**
//      * Receive a CAN msg from the bus, returning whether or not a message is available.
//      * This function also passes up the CanMsg to a callback function.
//      * @param rx_fifo Which RX FIFO to receive a message from.
//      */
//     void receive(uint32_t rx_fifo) const;
// };
// } // namespace hw::can
