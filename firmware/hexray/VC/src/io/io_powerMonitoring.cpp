#include "io_powerMonitoring.hpp"

#include "hw_i2cs.hpp"
#include "hw_gpios.hpp"
#include "io_log.hpp"
#include "io_time.hpp"
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
constexpr float VSENSE_LSB(1.5259e-6f / 0.003f);

// formula for power
constexpr float POWER_LSB = VBUS_LSB * VSENSE_LSB;
namespace io::powerMonitoring
{
static std::expected<void, ErrorCode> read_register(uint16_t reg, std::span<uint8_t> data)
{
    auto result = util::retry([&]() { return pwr_pump.memoryRead(reg, data); }, 5);
    return result;
}

static std::expected<void, ErrorCode> write_register(uint16_t reg, std::span<const uint8_t> data)
{
    auto result = util::retry([&]() { return pwr_pump.memoryWrite(reg, data); }, 5);
    return result;
}

std::expected<void, ErrorCode> refresh()
{
    const uint8_t cmd    = REG_REFRESH;
    auto          result = util::retry([&]() { return pwr_pump.transmit(std::span{ &cmd, 1 }); }, 3);
    return result;
}

std::expected<void, ErrorCode> init()
{
    // 1) Check if peripheral is ready
    RETURN_IF_ERR(util::retry([&]() { return pwr_pump.isTargetReady(); }, 5));

    // 2) Config: CTRL: 1024 SPS continuous, all CH enabled, ALERT1 enabled.
    uint16_t               ctrl       = 0x0000; // 0b0000000000000000
    std::array<uint8_t, 2> ctrl_bytes = { { (uint8_t)(ctrl >> 8), (uint8_t)(ctrl & 0xFF) } };
    RETURN_IF_ERR(write_register(REG_CTRL, ctrl_bytes));

    /*NOTE: the next two are already the default, configuration should be unneccsary*/
    // 3) FSR defaults
    std::array<uint8_t, 2> fsr = { { 0, 0 } };
    RETURN_IF_ERR(write_register(REG_NEG_PWR_FSR, fsr));

    // 4) ENABLE VBUS AND VSENSE, all channels
    uint8_t acc_cfg = 0x00;
    RETURN_IF_ERR(write_register(REG_ACCUM_CONFIG, (std::span{ &acc_cfg, 1 })));

    // 5) OV Protections, UV handled elsewhere
    /* OV = 24 × 1.15 = 27.6 V, UV = 24 × 0.85 = 20.4 V */

    uint8_t alert_disable = 0x00;
    RETURN_IF_ERR(write_register(ALERT_EN, std::span{ &alert_disable, 1 }));

    uint16_t overvoltage  = 0x6E66;
    uint16_t undervoltage = 0x519A;

    std::array<const uint8_t, 2> overvoltage_bytes = { { (uint8_t)(overvoltage >> 8), (uint8_t)(overvoltage & 0xFF) } };
    std::array<const uint8_t, 2> undervoltage_bytes = { { (uint8_t)(undervoltage >> 8),
                                                          (uint8_t)(undervoltage & 0xFF) } };

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

std::expected<float, ErrorCode> read_voltage(Channel ch)
{
    std::array<uint8_t, 2> buf;
    uint8_t                reg = (uint8_t)(REG_VBUS + (ch - 1));
    RETURN_IF_ERR(read_register(reg, buf));

    // msb first
    uint16_t raw = (uint16_t)((buf[0] << 8) | buf[1]);
    return (raw * VBUS_LSB);
}

std::expected<float, ErrorCode> read_current(Channel ch)
{
    std::array<uint8_t, 2> buf;
    uint8_t                reg = (uint8_t)(REG_VSENSE + (ch - 1));
    RETURN_IF_ERR(read_register(reg, buf));

    // MSB first
    uint16_t raw = (uint16_t)((buf[0] << 8) | buf[1]);
    return (raw * VSENSE_LSB);
}

std::expected<float, ErrorCode> read_power(Channel ch)
{
    std::array<uint8_t, 4> buf;
    uint8_t                reg = (uint8_t)(REG_VPOWERN + (ch - 1));

    RETURN_IF_ERR(read_register(reg, buf));

    // MSB first
    uint32_t raw30 =
        (((uint32_t)buf[0]) << 24) | (((uint32_t)buf[1]) << 16) | (((uint32_t)buf[2]) << 8) | ((uint32_t)buf[3]);

    // unimplemented first two bits
    raw30 &= 0x3FFFFFFFU;

    return ((float)raw30 * POWER_LSB);
}

std::expected<uint8_t, ErrorCode> read_alert_status()
{
    std::array<uint8_t, 3> status{};
    RETURN_IF_ERR(read_register(REG_ALERT_STATUS, status));

    uint8_t OV_UV_mask = status[1];
    return OV_UV_mask;
}

/**
 * @brief Bootleg function to configure in the IC register which power source we are using
 * @param void
 * @return NA
 */
std::expected<void, ErrorCode> monitor_power_inputs()
{
    RETURN_IF_ERR(refresh());
    uint8_t uv_active_mask = 0;
    for (uint8_t ch = 1; ch <= CHANNEL_NUM; ch++)
    {
        Channel channel = static_cast<Channel>(ch);
        auto v = read_voltage(channel);
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
    uint8_t                ov_uv_mask  = static_cast<uint8_t>(0xF0 | uv_active_mask);
    std::array<uint8_t, 3> alert_bytes = { { 0x00, ov_uv_mask, 0x00 } };

    RETURN_IF_ERR(write_register(ALERT_EN, alert_bytes));
    RETURN_IF_ERR(refresh());

    return {};
}

std::expected<bool, ErrorCode> is_alert_asserted()
{
    return !pwr_mtr_nalert.readPin();
}
} // namespace io::powerMonitoring
