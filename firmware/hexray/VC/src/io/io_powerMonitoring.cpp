#include "io_powerMonitoring.hpp"

#include "hw_i2cs.hpp"
#include "hw_gpios.hpp"
#include "io_log.hpp"
#include "io_semaphores.hpp"
#include "util_errorCodes.hpp"
#include "util_retry.hpp"

constexpr uint8_t CHANNEL_NUM = 4;
// commands
constexpr uint8_t REG_REFRESH = 0x00;

// register addresses (pg 38.)
constexpr uint16_t REG_CTRL         = 0x01;
constexpr uint16_t REG_VBUS         = 0x07;
constexpr uint16_t REG_VSENSE       = 0x0B;
constexpr uint16_t REG_VPOWERN      = 0x17;
constexpr uint16_t REG_NEG_PWR_FSR  = 0x1D;
constexpr uint16_t REG_ACCUM_CONFIG = 0x25;

// protection addresses
constexpr uint16_t REG_PROTECTION_OV = 0x3C;
constexpr uint16_t REG_PROTECTION_UV = 0x40;
constexpr uint16_t ALERT_EN          = 0x49;
constexpr uint16_t REG_ALERT_STATUS  = 0x26;
constexpr uint16_t REG_SLOW_ALERT1   = 0x27;

// channels
constexpr float CH_ON_MINV = 5;

// lsb scaling
constexpr float VBUS_LSB = 4.88e-4f;
constexpr float VSENSE_LSB(3.0518e-6f / 0.003f); // bipolar

// formula for power
constexpr float POWER_LSB = VBUS_LSB * VSENSE_LSB;
namespace io::powerMonitoring
{
static result<void> read_register(const uint16_t reg, const std::span<uint8_t> data)
{
    const io::unique_semaphore lock{ pwr_pump_i2c_bus_lock };
    auto                       result = util::retry([&]() { return pwr_mon.memoryRead(reg, data); }, 5);
    return result;
}

static result<void> write_register(const uint16_t reg, const std::span<const uint8_t> data)
{
    const io::unique_semaphore lock{ pwr_pump_i2c_bus_lock };
    auto                       result = util::retry([&]() { return pwr_mon.memoryWrite(reg, data); }, 5);
    return result;
}

result<void> refresh()
{
    constexpr uint8_t          cmd = REG_REFRESH;
    const io::unique_semaphore lock{ pwr_pump_i2c_bus_lock };
    auto                       result = util::retry([&]() { return pwr_mon.transmit(std::span{ &cmd, 1 }); }, 3);
    return result;
}

result<void> init()
{
    // 1) Check if peripheral is ready
    {
        const io::unique_semaphore lock{ pwr_pump_i2c_bus_lock };
        RETURN_IF_ERR(util::retry([&]() { return pwr_mon.isTargetReady(); }, 5));
    }

    // 2) Config: CTRL: 1024 SPS continuous, all CH enabled, ALERT1 enabled.
    constexpr uint16_t     ctrl       = 0x0000; // 0b0000000000000000
    std::array<uint8_t, 2> ctrl_bytes = { { static_cast<uint8_t>(ctrl >> 8), static_cast<uint8_t>(ctrl & 0xFF) } };
    RETURN_IF_ERR(write_register(REG_CTRL, ctrl_bytes));

    // 3) FSR defaults
    std::array<uint8_t, 2> fsr = { { 0x55, 0x00 } };
    RETURN_IF_ERR(write_register(REG_NEG_PWR_FSR, fsr));

    // 4) ENABLE VBUS AND VSENSE, all channels
    uint8_t acc_cfg = 0x00;
    RETURN_IF_ERR(write_register(REG_ACCUM_CONFIG, (std::span{ &acc_cfg, 1 })));

    // 5) OV Protections, UV handled elsewhere
    /* OV = 24 × 1.15 = 27.6 V, UV = 24 × 0.85 = 20.4 V */

    uint8_t alert_disable = 0x00;
    RETURN_IF_ERR(write_register(ALERT_EN, std::span{ &alert_disable, 1 }));

    constexpr uint16_t overvoltage  = 0x6E66;
    constexpr uint16_t undervoltage = 0x519A;

    std::array<const uint8_t, 2> overvoltage_bytes  = { { static_cast<uint8_t>(overvoltage >> 8),
                                                          static_cast<uint8_t>(overvoltage & 0xFF) } };
    std::array<const uint8_t, 2> undervoltage_bytes = { { static_cast<uint8_t>(undervoltage >> 8),
                                                          static_cast<uint8_t>(undervoltage & 0xFF) } };

    for (uint8_t i = 0; i < CHANNEL_NUM; i++)
    {
        RETURN_IF_ERR(write_register(REG_PROTECTION_OV + i, overvoltage_bytes));
    }
    for (uint8_t i = 0; i < CHANNEL_NUM; i++)
    {
        RETURN_IF_ERR(write_register(REG_PROTECTION_UV + i, undervoltage_bytes));
    }

    // 6) Ensure ALERT if OV/UV conditions are met
    std::array<uint8_t, 3> alert_enable_bytes = { {
        0x00, // bits 23:16, OC/UC disabled
        0xF0, // bits 15:8, OV enabled all channels, UV disabled for now
        0x00  // bits 7:0, OP/ACC/CC disabled
    } };

    RETURN_IF_ERR(write_register(ALERT_EN, alert_enable_bytes));
    RETURN_IF_ERR(write_register(REG_SLOW_ALERT1, alert_enable_bytes));

    RETURN_IF_ERR(refresh());

    // io::time::delay(2);

    return {};
}

result<float> read_voltage(const Channel ch)
{
    std::array<uint8_t, 2> buf;
    const uint8_t          reg = static_cast<uint8_t>(REG_VBUS + (ch - 1));
    RETURN_IF_ERR(read_register(reg, buf));

    // msb first
    const uint16_t raw = static_cast<uint16_t>((buf[0] << 8) | buf[1]);
    return (raw * VBUS_LSB);
}

result<float> read_current(const Channel ch)
{
    std::array<uint8_t, 2> buf;
    const uint8_t          reg = static_cast<uint16_t>(REG_VSENSE + (ch - 1));
    RETURN_IF_ERR(read_register(reg, buf));

    // MSB first, signed for bidirectional mode
    const int16_t raw = static_cast<uint16_t>((buf[0] << 8) | buf[1]);
    return (raw * VSENSE_LSB);
}

result<float> read_power(const Channel ch)
{
    std::array<uint8_t, 4> buf;
    const uint8_t          reg = static_cast<uint8_t>(REG_VPOWERN + (ch - 1));

    RETURN_IF_ERR(read_register(reg, buf));

    // MSB first
    int32_t raw = (static_cast<uint32_t>(buf[0]) << 24) | (static_cast<uint32_t>(buf[1]) << 16) |
                  (static_cast<uint32_t>(buf[2]) << 8) | (static_cast<uint32_t>(buf[3]));

    // clear bits 31:30 garbage, sign extend from bit 29
    raw = (raw << 2) >> 2;

    return (static_cast<float>(raw) * POWER_LSB);
}

result<uint8_t> read_alert_status()
{
    std::array<uint8_t, 3> status{};
    RETURN_IF_ERR(read_register(REG_ALERT_STATUS, status));

    uint8_t OV_UV_mask = status[1];
    return OV_UV_mask;
}

/**
 * @brief Bootleg function to configure in the IC register which power source we are using
 * @return NA
 */
result<void> monitor_power_inputs()
{
    RETURN_IF_ERR(refresh());
    uint8_t uv_active_mask = 0;
    for (uint8_t ch = 1; ch <= CHANNEL_NUM; ch++)
    {
        const auto channel = static_cast<Channel>(ch);
        auto       v       = read_voltage(channel);
        if (!v.has_value())
        {
            return std::unexpected(v.error());
        }
        if (v.value() > CH_ON_MINV)
        {
            uv_active_mask |= static_cast<uint8_t>(1u << (CHANNEL_NUM - ch));
        }
    }

    // OV all channels, UV only active channel
    const uint8_t          ov_uv_mask  = static_cast<uint8_t>(0xF0 | uv_active_mask);
    std::array<uint8_t, 3> alert_bytes = { { 0x00, ov_uv_mask, 0x00 } };

    RETURN_IF_ERR(write_register(ALERT_EN, alert_bytes));
    RETURN_IF_ERR(refresh());

    return {};
}

result<bool> is_alert_asserted()
{
    return !pwr_mtr_nalert.readPin();
}
} // namespace io::powerMonitoring
