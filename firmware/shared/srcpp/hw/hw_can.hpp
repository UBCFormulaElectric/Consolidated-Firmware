#pragma once

#include "util_errorCodes.hpp"
#include "cmsis_os.h"
#include "hw_hal.hpp"

#include <array>

// #if defined(STM32H733xx)
// #include "stm32h7xx_hal_fdcan.h"
// #elif defined(STM32H562xx)
// #include "stm32h5xx_hal_fdcan.h"
// #elif defined(STM32F412Rx)
// #include <stm32f4xx_hal_can.h>
// #else
// #error "Unsupported MCU for CAN driver"
// #endif

namespace hw
{
#if defined(STM32F412Rx)
inline constexpr size_t CAN_PAYLOAD_BYTES = 8;
#elif defined(STM32H733xx) or defined(STM32H562xx)
inline constexpr size_t CAN_PAYLOAD_BYTES = 64;
#else
#error "Please define what MCU is used."
#endif
struct CanMsg
{
    CanMsg(const uint32_t _std_id, const uint32_t _dlc, const std::array<uint8_t, CAN_PAYLOAD_BYTES> &_data)
      : std_id(_std_id), dlc(_dlc), data(_data)
    {
    }

    explicit CanMsg() = default;

    uint32_t                               std_id = 0;
    uint32_t                               dlc    = 0;
    std::array<uint8_t, CAN_PAYLOAD_BYTES> data{};
};

class BaseCan
{
  protected:
    mutable bool ready = false;

  public:
    virtual ~BaseCan() = default;
    void (*const receive_callback)(const CanMsg &rx_msg);
    mutable TaskHandle_t transmit_task = nullptr;

    consteval explicit BaseCan(void (*const receive_callback_in)(const CanMsg &rx_msg))
      : receive_callback(receive_callback_in){};

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
    virtual std::expected<void, ErrorCode> can_transmit(const CanMsg &msg) const = 0;

    /**
     * Receive a CAN msg from the bus, returning whether or not a message is available.
     * This function also passes up the CanMsg to a callback function.
     * @param rx_fifo Which RX FIFO to receive a message from.
     * @return Whether or not the reception was successful.
     */
    virtual std::expected<CanMsg, ErrorCode> receive(uint32_t rx_fifo) const = 0;
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
    std::expected<void, ErrorCode> tx(const CAN_TxHeaderTypeDef &tx_header, const CanMsg &msg) const;

  public:
    ~can() override = default;
    consteval explicit can(CAN_HandleTypeDef &hcan_in, void (*const receive_callback_in)(const CanMsg &rx_msg))
      : BaseCan(receive_callback_in), hcan(&hcan_in){};

    constexpr CAN_HandleTypeDef *getHcan() const { return hcan; }

    void init() const override;

    void deinit() const override;

    std::expected<void, ErrorCode> can_transmit(const CanMsg &msg) const override;

    std::expected<CanMsg, ErrorCode> receive(uint32_t rx_fifo) const override;
};

const can &can_getHandle(const CAN_HandleTypeDef *hcan);
#elif defined(STM32H733xx) or defined(STM32H562xx)
class fdcan final : public BaseCan
{
    FDCAN_HandleTypeDef *const     hfdcan;
    std::expected<void, ErrorCode> tx(const FDCAN_TxHeaderTypeDef &tx_header, const CanMsg &msg) const;

  public:
    ~fdcan() override = default;
    consteval explicit fdcan(FDCAN_HandleTypeDef &hfdcan_in, void (*const receive_callback_in)(const CanMsg &rx_msg))
      : BaseCan(receive_callback_in), hfdcan(&hfdcan_in){};

    constexpr FDCAN_HandleTypeDef *getHfdcan() const { return hfdcan; }

    void init() const override;

    void deinit() const override;

    std::expected<void, ErrorCode> can_transmit(const CanMsg &msg) const override;

    std::expected<void, ErrorCode> fdcan_transmit(const CanMsg &msg) const;

    std::expected<CanMsg, ErrorCode> receive(uint32_t rx_fifo) const override;
};

const fdcan &fdcan_getHandle(const FDCAN_HandleTypeDef *hfdcan);
#endif
} // namespace hw