#include "hw_uart.hpp"
#include "hw_utils.hpp"

namespace hw
{
std::expected<void, ErrorCode> Uart::transmitDma(std::span<const uint8_t> data) const
{
    return hw_utils_convertHalStatus(HAL_UART_Transmit_DMA(handle, data.data(), data.size()));
}

std::expected<void, ErrorCode> Uart::receiveDma(std::span<uint8_t> data) const
{
    return hw_utils_convertHalStatus(HAL_UART_Receive_DMA(handle, data.data(), data.size()));
}

std::expected<void, ErrorCode> Uart::transmitPoll(std::span<const uint8_t> data, uint32_t timeout) const
{
    return hw_utils_convertHalStatus(HAL_UART_Transmit(handle, data.data(), data.size(), timeout));
}

std::expected<void, ErrorCode> Uart::receivePoll(std::span<uint8_t> data, uint32_t timeout) const
{
    return hw_utils_convertHalStatus(HAL_UART_Receive(handle, data.data(), data.size(), timeout));
}

std::expected<void, ErrorCode> Uart::transmitIt(std::span<const uint8_t> data) const
{
    return hw_utils_convertHalStatus(HAL_UART_Transmit_IT(handle, data.data(), data.size()));
}

std::expected<void, ErrorCode> Uart::receiveIt(std::span<uint8_t> data) const
{
    return hw_utils_convertHalStatus(HAL_UART_Receive_IT(handle, data.data(), data.size()));
}
} // namespace hw
