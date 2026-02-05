#include "hw_uart.hpp"
#include "hw_utils.hpp"

namespace hw
{
std::expected<void, ErrorCode> Uart::transmitDma(const std::span<const uint8_t> data) const
{
    return hw_utils_convertHalStatus(HAL_UART_Transmit_DMA(handle, data.data(), static_cast<uint16_t>(data.size())));
}

std::expected<void, ErrorCode> Uart::receiveDma(std::span<uint8_t> data) const
{
    return hw_utils_convertHalStatus(HAL_UART_Receive_DMA(handle, data.data(), static_cast<uint16_t>(data.size())));
}

std::expected<void, ErrorCode> Uart::transmitPoll(const std::span<const uint8_t> tx_data, const uint32_t timeout) const
{
    return hw_utils_convertHalStatus(
        HAL_UART_Transmit(handle, tx_data.data(), static_cast<uint16_t>(tx_data.size()), timeout));
}

std::expected<void, ErrorCode> Uart::receivePoll(std::span<uint8_t> rx_data, const uint32_t timeout) const
{
    return hw_utils_convertHalStatus(
        HAL_UART_Receive(handle, rx_data.data(), static_cast<uint16_t>(rx_data.size()), timeout));
}

std::expected<void, ErrorCode> Uart::transmitIt(const std::span<const uint8_t> data) const
{
    return hw_utils_convertHalStatus(HAL_UART_Transmit_IT(handle, data.data(), static_cast<uint16_t>(data.size())));
}

std::expected<void, ErrorCode> Uart::receiveIt(std::span<uint8_t> rx_data) const
{
    return hw_utils_convertHalStatus(HAL_UART_Receive_IT(handle, rx_data.data(), static_cast<uint16_t>(rx_data.size())));
}
} // namespace hw
