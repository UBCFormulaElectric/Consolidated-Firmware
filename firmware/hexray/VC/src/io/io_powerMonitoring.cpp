#include "io_powerMonitoring.hpp"
#include "io_powerMonitoring_datatypes.hpp"

#include "hw_i2cs.hpp"
#include "io_log.hpp"
#include "io_time.hpp"
#include "util_errorCodes.hpp"

// lsb scaling
constexpr float VBUS_LSB = 4.88e-4f;
constexpr float VSENSE_LSB(1.5259e-6f / 0.003f);

// formula for power
constexpr float POWER_LSB = VBUS_LSB * VSENSE_LSB;
namespace io::powerMonitoring
{
static std::expected<void, ErrorCode> read_register(uint16_t reg, std::span<uint8_t> data)
{
    RETURN_IF_ERR_SILENT(hw::i2c::pwr_pump.memoryRead(reg, data));
    return {};
}

static std::expected<void, ErrorCode> write_register(uint16_t reg, std::span<const uint8_t> data)
{
    /*no need for this stuff i reviewed the functions the i2c trasnmit both BLOCKING and NONBLOCK functions store the
    register in the first byte when called, so what we are doing here in the c code was unnecesary: ive left it below to
    confirm*/

    /* uint8_t buf[1 + len];
    buf[0] = reg;
    memcpy(&buf[1], data, len);
    return (hw_i2c_memoryWrite(&pwr_mtr, reg, buf, len + 1) == EXIT_CODE_OK); */

    RETURN_IF_ERR(hw::i2c::pwr_pump.memoryWrite(reg, data));
    return {};
}

std::expected<void, ErrorCode> refresh(void)
{
    const uint8_t cmd = REG_REFRESH;
    LOG_IF_ERR(hw::i2c::pwr_pump.transmit((std::span{&cmd, 1})));
    io::time::delay(1);
    return {};
}

std::expected<void, ErrorCode> init(void)
{
    // 1) Check if peripheral is ready
    RETURN_IF_ERR(hw::i2c::pwr_pump.isTargetReady());

    // 2) Config: CTRL: 1024 SPS continuous, all CH enabled, ALERT1 enabled.
    uint16_t                     ctrl       = 0x0000; // 0b0000000000000000
    std::array<const uint8_t, 2> ctrl_bytes = { { (uint8_t)(ctrl >> 8), (uint8_t)(ctrl & 0xFF) } };
    RETURN_IF_ERR(write_register(REG_CTRL, ctrl_bytes));

    /*NOTE: the next two are already the default, configuration should be unneccsary*/
    // 3) FSR defaults
    std::array<const uint8_t, 2> fsr = { { 0, 0 } };
    RETURN_IF_ERR(write_register(REG_NEG_PWR_FSR, fsr));

    // 4) ENABLE VBUS AND VSENSE, all channels
    uint8_t acc_cfg = 0x00;
    RETURN_IF_ERR(write_register(REG_ACCUM_CONFIG, (std::span{ &acc_cfg, 1 })));

    // 5) OV and UV Protections
    /* OV = 24 × 1.15 = 27.6 V, UV = 24 × 0.85 = 20.4 V */
    uint16_t overvoltage = 0xDCED;
    uint16_t undervoltage = 0xA343;
    std::array<const uint8_t, 2> overvoltage_bytes = {{(uint8_t)(overvoltage >> 8), (uint8_t)(overvoltage & 0xFF)}};
    std::array<const uint8_t, 2> undervoltage_bytes = {{(uint8_t)(undervoltage >> 8), (uint8_t)(undervoltage & 0xFF)}};

    for(uint8_t i = 0; i < CHANNEL_NUM; i ++)
    {
        RETURN_IF_ERR(write_register(REG_PROTECTION_OV + i, overvoltage_bytes));
    }
    for(uint8_t i = 0; i < CHANNEL_NUM; i ++)
    {
        RETURN_IF_ERR(write_register(REG_PROTECTION_UV + i, undervoltage_bytes));
    }
    // 6) Ensure ALERT if OV/UV conditions are met
    std::array<uint8_t, 3> alert_enable_bytes = {{
        0x00, // bits 23:16, OC/UC disabled
        0xFF, // bits 15:8, OV/UV enabled all channels
        0x00  // bits 7:0, OP/ACC/CC disabled
    }};
    RETURN_IF_ERR(write_register(ALERT_EN, alert_enable_bytes));

    RETURN_IF_ERR(refresh());

    return {};
}

std::expected<float, ErrorCode> read_voltage(uint8_t ch)
{
    std::array<uint8_t, 2> buf;
    uint8_t reg = (uint8_t)(REG_VBUS + (ch - 1));
    RETURN_IF_ERR(read_register(reg, buf));

    // msb first
    uint16_t raw = (uint16_t)((buf[0] << 8) | buf[1]);
    return (raw * VBUS_LSB);
}

std::expected<float, ErrorCode> read_current(uint8_t ch)
{
    std::array<uint8_t, 2> buf;
    uint8_t reg = (uint8_t)(REG_VSENSE + (ch - 1));
    RETURN_IF_ERR(read_register(reg, buf));

    // MSB first
    uint16_t raw = (uint16_t)((buf[0] << 8) | buf[1]);
    return (raw * VSENSE_LSB);
}

std::expected<float, ErrorCode> read_power(uint8_t ch)
{
    std::array<uint8_t, 4> buf;
    uint8_t reg = (uint8_t)(REG_VPOWERN + (ch - 1));

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
} // namespace io::powerMonitoring
