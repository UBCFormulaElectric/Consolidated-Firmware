#include "io_powerMonitoring.hpp"

#include "hw_i2cs.hpp"
#include "io_log.hpp"
#include <cstring>
#include <array>

//register addresses (pg 45.)
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
void read_register(std::uint16_t reg, std::span<uint8_t> data)
{
    LOG_IF_ERR(hw::i2c::pwr_pump.memoryRead(reg, data));
}

void write_register(std::uint16_t reg, std::span<const std::uint8_t> data)
{
    //uint32_t len = *data.size();
    //std::span<uint8_t> *buf[len+1];
    //buf[0] = reg;
    //memcpy(&buf[1], *data, len);
    LOG_IF_ERR(hw::i2c::pwr_pump.memoryWrite(reg, data)); //need to use buf
}

void io_power_monitoring_refresh(void) 
{
    const std::uint8_t cmd = 0x00;
    LOG_IF_ERR(hw::i2c::pwr_pump.transmit(std::span<const std::uint8_t>{&cmd, 1}));
    vTaskDelay(1);
}

//TODO: init of power monitoring

void read_voltage(uint8_t ch, float* voltage) 
{
    std::array<uint8_t, 2> buf;
    uint8_t reg = (uint8_t)(REG_VBUS + (ch - 1));
    read_register(reg, buf);

    uint16_t raw = (uint16_t)((buf[0] << 8) | buf[1]);
    *voltage     = raw * VBUS_LSB;
}

void read_current(uint8_t ch, float* current) 
{
    std::array<uint8_t, 2> buf;
    uint8_t reg = (uint8_t)(REG_VSENSE + (ch - 1));
    read_register(reg, buf);

    uint16_t raw = (uint16_t)((buf[0] << 8) | buf[1]);
    *current = raw * VSENSE_LSB;
}

void read_power(uint8_t ch, float *power) 
{
    std::array<uint8_t, 4> buf;
    uint8_t reg = (uint8_t)(REG_VPOWERN + (ch - 1));

    read_register(reg, buf);

    uint32_t raw30 =
        (((uint32_t)buf[0]) << 24) | (((uint32_t)buf[1]) << 16) | (((uint32_t)buf[2]) << 8) | ((uint32_t)buf[3]);

    raw30 &= 0x3FFFFFFFU;

    *power = (float)raw30 * POWER_LSB;
}

} // namespace io::powerMonitoring
