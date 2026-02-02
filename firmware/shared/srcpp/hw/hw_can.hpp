#pragma once

#include "io_canMsg.hpp"
#include "util_errorCodes.hpp"
#include "hw_hal.h"
#include "cmsis_os.h"

#if defined(STM32H733xx)
#include "stm32h7xx_hal_fdcan.h"
#elif defined(STM32H562xx)
#include "stm32h5xx_hal_fdcan.h"
#endif

#include <cassert>

namespace hw
{

class BaseCan
{
  protected:
    mutable bool  ready = false;
    const uint8_t bus_num;

  public:
    virtual ~BaseCan() = default;
    void (*const receive_callback)(const io::CanMsg &rx_msg);
    TaskHandle_t transmit_task = nullptr;

    consteval explicit BaseCan(const uint8_t bus_num_in, void (*const receive_callback_in)(const io::CanMsg &rx_msg))
      : bus_num(bus_num_in), receive_callback(receive_callback_in){};

    /**
     * Initialize CAN driver.
     * @param can_handle STM32 HAL CAN handle.
     */
    virtual void init() const = 0;

    /**
     * Stop and deinitialize the CAN peripheral.
     */
    virtual void deinit() const = 0;

    /**
     * Transmit a CAN msg on the bus, blocking until completed.
     * @param msg CAN msg to be TXed.
     * @return Whether or not the transmission was successful.
     */
    virtual std::expected<void, ErrorCode> can_transmit(const io::CanMsg &msg) = 0;

    /**
     * Receive a CAN msg from the bus, returning whether or not a message is available.
     * This function also passes up the CanMsg to a callback function.
     * @param msg CAN msg to be RXed.
     * @param rx_fifo Which RX FIFO to receive a message from.
     * @return Whether or not the reception was successful.
     */
    virtual std::expected<void, ErrorCode> receive(uint32_t rx_fifo, io::CanMsg &msg) const = 0;

    constexpr uint8_t getBusNum() const { return bus_num; }
};
/**
 * @attention THIS MUST BE DEFINED IN YOUR CONFIGURATIONS
 * @param hcan takes a handle to a STM32 HAL CAN object
 * @returns a pointer to a CanHandle object (the metadata associated with the STM32 HAL CAN object)
 */

#if defined(STM32F412Rx)
class can final : public BaseCan
{
    CAN_HandleTypeDef *const hcan;

  private:
    std::expected<void, ErrorCode> tx(const CAN_TxHeaderTypeDef &tx_header, const io::CanMsg &msg);

  public:
    consteval explicit can(
        CAN_HandleTypeDef &hcan_in,
        const uint8_t      bus_num_in,
        void (*const receive_callback_in)(const io::CanMsg &rx_msg))
      : BaseCan(bus_num_in, receive_callback_in), hcan(&hcan_in){};

    constexpr CAN_HandleTypeDef *getHcan() const { return hcan; }

    void init() const override;

    void deinit() const override;

    std::expected<void, ErrorCode> can_transmit(const io::CanMsg &msg) override;

    std::expected<void, ErrorCode> receive(uint32_t rx_fifo, io::CanMsg &msg) const override;
};

const can &can_getHandle(const CAN_HandleTypeDef *hcan);
#elif defined(STM32H733xx) or defined(STM32H562xx)
class fdcan final : public BaseCan
{
    FDCAN_HandleTypeDef *const hfdcan;

  private:
    std::expected<void, ErrorCode> tx(const FDCAN_TxHeaderTypeDef &tx_header, const io::CanMsg &msg);

  public:
    consteval explicit fdcan(
        FDCAN_HandleTypeDef &hfdcan_in,
        const uint8_t        bus_num_in,
        void (*const receive_callback_in)(const io::CanMsg &rx_msg))
      : BaseCan(bus_num_in, receive_callback_in), hfdcan(&hfdcan_in){};

    constexpr FDCAN_HandleTypeDef *getHfdcan() const { return hfdcan; }

    void init() const override;

    void deinit() const override;

    std::expected<void, ErrorCode> can_transmit(const io::CanMsg &msg) override;

    std::expected<void, ErrorCode> fdcan_transmit(const io::CanMsg &msg);

    std::expected<void, ErrorCode> receive(uint32_t rx_fifo, io::CanMsg &msg) const override;
};

const fdcan &fdcan_getHandle(const FDCAN_HandleTypeDef *hfdcan);
#endif
} // namespace hw