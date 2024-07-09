#pragma once

#include <cstdint>
#include <utility>
#include "hw_hal.h"

namespace hw
{
#ifdef CANFD
// STM32 HAL CAN FD handle.
typedef FDCAN_HandleTypeDef USEDCAN_HandleTypeDef;
#else
// STM32 HAL CAN handle.
typedef CAN_HandleTypeDef USEDCAN_HandleTypeDef;
#endif
#define CAN_PAYLOAD_BYTES 8

struct CanMsg
{
    uint32_t std_id;
    uint32_t dlc; // data length range : [0, 8]
    uint8_t  data[CAN_PAYLOAD_BYTES];
};

class can
{
    void (*const can_msg_received_callback)(const CanMsg *rx_msg);
    USEDCAN_HandleTypeDef *const handle;

  public:
    /**
     * Initialize CAN driver.
     * @param can_handle STM32 HAL CAN handle.
     * @param can_msg_received_callback Callback function to be called when a CAN message is received.
     */
    explicit can(USEDCAN_HandleTypeDef *can_handle_in, void (*can_msg_received_callback_in)(const CanMsg *))
      : can_msg_received_callback(can_msg_received_callback_in), handle(can_handle_in)
    {
    }
    /**
     * Stop and deinitialize the CAN peripheral.
     */
    ~can();

    void init() const;

    /**
     * Transmit a CAN msg on the bus, blocking until completed.
     * @param msg CAN msg to be TXed.
     * @return Whether or not the transmission was successful.
     */
    bool transmit(const CanMsg *msg) const;
    /**
     * Receive a CAN msg from the bus, returning whether or not a message is available.
     * This function also passes up the CanMsg to a callback function.
     * @param rx_fifo Which RX FIFO to receive a message from.
     */
    void receive(uint32_t rx_fifo) const;
};
} // namespace hw
