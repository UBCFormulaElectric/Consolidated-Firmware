#pragma once

#include <cstdint>
#include <array>
#include "util_errorCodes.hpp"

constexpr float   MAX_WIPER_VALUE     = 256.0f;
constexpr float   MIN_WIPER_VALUE     = 0.0f;
constexpr uint8_t POT_WIPER0_REGISTER = 0x0;
constexpr uint8_t POT_WIPER1_REGISTER = 0x1;

#ifdef TARGET_EMBEDDED

#include "hw_i2c.hpp"

namespace io
{
enum class POTENTIOMETER_WIPER : uint8_t
{
    WIPER0 = 0,
    WIPER1,
};

class Potentiometer
{
  private:
    const hw::i2c::I2CDevice  device;
    const POTENTIOMETER_WIPER wiper;

    enum class POTENTIOMETER_CMD : uint8_t
    {
        WRITE     = 0x00,
        INCREMENT = 0x01,
        DECREMENT = 0x02,
        READ      = 0x03,
    };

    [[nodiscard]] constexpr uint8_t wiperRegister() const
    {
        return ((wiper == POTENTIOMETER_WIPER::WIPER0) ? POT_WIPER0_REGISTER : POT_WIPER1_REGISTER);
    }

    [[nodiscard]] static constexpr uint8_t buildHeader(uint8_t address, POTENTIOMETER_CMD cmd)
    {
        return (static_cast<uint8_t>(address) << 4 | static_cast<uint8_t>(cmd) << 2);
    }

  public:
    explicit Potentiometer(hw::i2c::I2CDevice device_in, POTENTIOMETER_WIPER wiper_in)
      : device(device_in), wiper(wiper_in){};

    [[nodiscard]] std::expected<void, ErrorCode> readWiper(std::array<uint8_t, 2> &dest) const
    {
        std::array<uint8_t, 1> read_cmd{ buildHeader(wiperRegister(), POTENTIOMETER_CMD::READ) };
        RETURN_IF_ERR(device.transmit(read_cmd));
        RETURN_IF_ERR(device.receive(dest));
        return {};
    }

    [[nodiscard]] std::expected<void, ErrorCode> readPercentage(uint8_t &dest) const
    {
        std::array<uint8_t, 2> data{ 0 };
        RETURN_IF_ERR(readWiper(data));
        const uint16_t read_data{ static_cast<uint16_t>(data[0] << 8 | data[1]) };
        dest = static_cast<uint8_t>((read_data * 100.0f) / MAX_WIPER_VALUE);
        return {};
    }

    [[nodiscard]] std::expected<void, ErrorCode> writeWiper(uint8_t data) const
    {
        const std::array<uint8_t, 2> tx_cmd{ buildHeader(wiperRegister(), POTENTIOMETER_CMD::WRITE), data };
        RETURN_IF_ERR(device.transmit(tx_cmd));
        return {};
    }

    [[nodiscard]] std::expected<void, ErrorCode> writePercentage(uint8_t percentage) const
    {
        return (writeWiper(static_cast<uint8_t>(percentage / 100.0f * MAX_WIPER_VALUE)));
    }
};
} // namespace io
#endif
