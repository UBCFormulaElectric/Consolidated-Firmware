#include "io_powerMonitoring.hpp"

#include "hw_i2cs.hpp"
#include "io_log.hpp"
#include <cstring>
#include <array>
#include <expected>

//register addresses (p`g 45.)
constexpr std::uint16_t REG_REFRESH =      0x00;
constexpr std::uint16_t REG_CTRL =         0x01;
constexpr std::uint16_t REG_VBUS =         0x07;
constexpr std::uint16_t REG_VSENSE =       0x0B;
constexpr std::uint16_t REG_VPOWERN =      0x17;
constexpr std::uint16_t REG_NEG_PWR_FSR =  0x1D;
constexpr std::uint16_t REG_ACCUM_CONFIG = 0x25;
//lsb scaling
constexpr float VBUS_LSB = 4.88e-4f;
constexpr float VSENSE_LSB (1.5259e-6f / 0.003f);

//formula for power 
constexpr float POWER_LSB = VBUS_LSB * VSENSE_LSB;
namespace io::powerMonitoring
{
std::expected<void, ErrorCode> read_register(std::uint16_t reg, std::span<uint8_t> data)
{
    RETURN_IF_ERR_SILENT(hw::i2c::pwr_pump.memoryRead(reg, data));
    return {};
}

std::expected<void, ErrorCode> write_register(std::uint16_t reg, std::span<const std::uint8_t> data)
{
    /*no need for this stuff i reviewed the functions the i2c trasnmit both BLOCKING and NONBLOCK functions store the register 
    in the first byte when called, so what we are doing here in the c code was unnecesary: ive left it below to confirm*/

    /* uint8_t buf[1 + len];
    buf[0] = reg;
    memcpy(&buf[1], data, len);
    return (hw_i2c_memoryWrite(&pwr_mtr, reg, buf, len + 1) == EXIT_CODE_OK); */

    LOG_IF_ERR(hw::i2c::pwr_pump.memoryWrite(reg, data));
    return {};
}

void io_power_monitoring_refresh(void) 
{
    const std::uint8_t cmd = 0x00;
    LOG_IF_ERR(hw::i2c::pwr_pump.transmit((std::span{&cmd, 1})));
    vTaskDelay(1);
}

bool io_powerMonitoring_init(void) 
{
    // 1) Check if peripheral is ready
    if (!hw::i2c::pwr_pump.isTargetReady()) 
    {
        return false; 
    }   

    // 2) Config: CTRL: 1024 SPS continuous, all CH enabled, slow ALERT1 enabled.
    std::uint16_t ctrl =  0x700; //0b0000011100000000
    std::array<const uint8_t, 2> ctrl_bytes = {{(std::uint8_t)(ctrl >> 8), (std::uint8_t)(ctrl)}};
    if (!write_register(REG_CTRL, ctrl_bytes))
    {
        return false;
    }

    // 3) FSR defaults
    std::array<uint8_t, 2> fsr = {{ 0, 0 }};
    if (!write_register(REG_NEG_PWR_FSR, fsr))
    {
        return false;
    }

    /* 4) ENABLE VBUS AND VSENSE (idk why it was 0x03 when ch3 and ch4 wasnt in use 
    but now all of em are on so use 0x00) */
    std::uint8_t acc_cfg = 0x00;
    if (!write_register(REG_ACCUM_CONFIG, (std::span{&acc_cfg, 1}))) 
    {
        return false;
    }

    uint8_t cmd = REG_REFRESH;
    if (!write_register(REG_ACCUM_CONFIG, (std::span{&cmd, 1})))
    {
        return false;
    }

   return true;

}

void read_voltage(uint8_t ch, float* voltage) 
{
    std::array<uint8_t, 2> buf;
    uint8_t reg = (uint8_t)(REG_VBUS + (ch - 1));
    read_register(reg, buf);

    //msb first
    uint16_t raw = (uint16_t)((buf[0] << 8) | buf[1]);
    *voltage = raw * VBUS_LSB;
}

void read_current(uint8_t ch, float* current) 
{
    std::array<uint8_t, 2> buf;
    uint8_t reg = (uint8_t)(REG_VSENSE + (ch - 1));
    read_register(reg, buf);

    // MSB first
    uint16_t raw = (uint16_t)((buf[0] << 8) | buf[1]);
    *current = raw * VSENSE_LSB;
}

void read_power(uint8_t ch, float *power) 
{
    std::array<uint8_t, 4> buf;
    uint8_t reg = (uint8_t)(REG_VPOWERN + (ch - 1));

    read_register(reg, buf);

    // MSB first
    uint32_t raw30 =
        (((uint32_t)buf[0]) << 24) | (((uint32_t)buf[1]) << 16) | (((uint32_t)buf[2]) << 8) | ((uint32_t)buf[3]);

    //unimplemented first two bits
    raw30 &= 0xFFFFFFF3U;

    *power = (float) raw30 * POWER_LSB;
}

} // namespace io::powerMonitoring
