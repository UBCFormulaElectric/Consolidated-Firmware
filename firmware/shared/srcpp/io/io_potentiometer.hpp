/**
 * @file io_potentiometer.hpp
 * This driver is for the MCP4661-502E/ST potentiometer
 */
#pragma once

#include <cstdint>
#include <array>
#include "util_errorCodes.hpp"
#include <cassert>

#ifdef TARGET_EMBEDDED
#include "hw_i2c.hpp"
#endif

namespace io
{
enum class POTENTIOMETER_WIPER : uint8_t
{
    WIPER0 = 0,
    WIPER1,
};

class Potentiometer
{
#ifdef TARGET_EMBEDDED
    static constexpr float   MAX_WIPER_VALUE     = 256.0f;
    static constexpr float   MIN_WIPER_VALUE     = 0.0f;
    static constexpr uint8_t POT_WIPER0_REGISTER = 0x0;
    static constexpr uint8_t POT_WIPER1_REGISTER = 0x1;

    const hw::i2c::device     device;
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
        return (wiper == POTENTIOMETER_WIPER::WIPER0) ? POT_WIPER0_REGISTER : POT_WIPER1_REGISTER;
    }

    [[nodiscard]] static constexpr uint8_t buildHeader(const uint8_t address, POTENTIOMETER_CMD cmd)
    {
        return static_cast<uint8_t>((static_cast<uint8_t>(address << 4) | static_cast<uint8_t>(cmd) << 2) & 0xFC);
    }

    [[nodiscard]] result<void> readWiper(std::array<uint8_t, 2> &dest) const
    {
        std::array<uint8_t, 1> read_cmd{ { buildHeader(wiperRegister(), POTENTIOMETER_CMD::READ) } };
        RETURN_IF_ERR(device.transmit(read_cmd));
        RETURN_IF_ERR(device.receive(dest));
        return {};
    }

    [[nodiscard]] result<void> writeWiper(const uint8_t data) const
    {
        const std::array<uint8_t, 2> tx_cmd{ { buildHeader(wiperRegister(), POTENTIOMETER_CMD::WRITE), data } };
        assert(device.isTargetReady().has_value());
        RETURN_IF_ERR(device.transmit(tx_cmd, false));
        return {};
    }
#endif

  public:
#ifdef TARGET_EMBEDDED
    constexpr explicit Potentiometer(const hw::i2c::device &device_in, const POTENTIOMETER_WIPER wiper_in)
      : device(device_in), wiper(wiper_in){};
#elif TARGET_TEST
    constexpr explicit Potentiometer(){};
#endif

    [[nodiscard]] result<uint8_t> readPercentage() const
    {
#ifdef TARGET_EMBEDDED
        std::array<uint8_t, 2> data{};
        RETURN_IF_ERR(readWiper(data));
        const uint16_t read_data{ static_cast<uint16_t>(data[0] << 8 | data[1]) };
        return read_data * 100 / MAX_WIPER_VALUE;
#elif TARGET_TEST
        return 0;
#endif
    }
    [[nodiscard]] result<void> writePercentage(const uint8_t percentage) const
    {
#ifdef TARGET_EMBEDDED
        return writeWiper(static_cast<uint8_t>(percentage * MAX_WIPER_VALUE / 100));
#elif TARGET_TEST
        return {};
#endif
    }
};
} // namespace io
